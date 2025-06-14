#include "Enemy.h"
#include "HtAudio.h"
#include "MainGun.h"
#include "MissileLauncher.h"
#include "Projectile.h"
#include "Spaceship.h"
#include "ObjectManager.h"
#include "ErrorLogger.h"
#include "HUDFont.h"
#include <random>

const double Enemy::MOVEMENT_SPEED = 100;

Enemy::Enemy(const Vector2D& position, const int health, SpaceShip* player) : GameObject(ObjectType::ENEMY), m_player(player), m_health(health)
{
    m_position = position;
    m_collisionShape.PlaceAt(m_position, m_collisionSize);
    m_scale = 3;
    MainGun* mainGun = new MainGun("assets/empty.png", this);
    mainGun->SetCooldown(BULLET_COOLDOWN);
    mainGun->InitProjectile(0, BULLET_DAMAGE, 2000, 30);
    ObjectManager::instance.AddItem(mainGun);
    m_mainGun = mainGun;

    MissileLauncher* missileLauncher = new MissileLauncher("assets/empty.png", this);
    missileLauncher->SetCooldown(MISSILE_COOLDOWN);
    missileLauncher->SetBarrageParams(1, MISSILE_COOLDOWN); //If only firing 1 2nd parameter doesn't matter
    missileLauncher->InitProjectile(0, MISSILE_DAMAGE, MISSILE_SPEED, 30);
    ObjectManager::instance.AddItem(missileLauncher);
    m_missileLauncher = missileLauncher;
   

    LoadImage("assets/player/ship/ship-25.png");
    LoadImage("assets/player/ship/ship-hit.png");

    //Explosion
    for (int i = 1; i <= EXPLOSION_FRAME_COUNT; i++)
    {
        LoadImage(("assets/explosionA" + std::to_string(i) + ".bmp").c_str()); //Explosion frames 2-10
    }

    //Explosion Sounds
    std::vector<std::string> explosionStrings = {"assets/explosion2.wav", "assets/explosion3.wav", "assets/explosion4.wav"};
    int explosionIndex = std::rand() % explosionStrings.size();
    m_explosionSFX = HtAudio::instance.LoadSound(explosionStrings[explosionIndex]);
}

void Enemy::Update(double frametime)
{
    m_collisionShape.PlaceAt(m_position, m_isExploding ? m_collisionSize : 0);

    if (m_health <= 0 && !m_isExploding)
    {
        UponDeath();
    }
    

    m_collisionShape.PlaceAt(m_position, m_collisionSize);

    if(!WithinRange())
    {
        Navigate();
    }
    else
    {
        m_velocity = Vector2D(0,0);
    }

    m_position += m_velocity * frametime;

    if (m_player)
    {
        Vector2D toPlayer = (m_player->GetPosition() - m_position);
        m_angle = toPlayer.angle();
    }

    //Hit Flash
    if (m_hitFlashTimer >= 0.0 && !m_isExploding) //If it is flashing. Maybe make a function
    {
        m_hitFlashTimer += frametime;
        if (m_hitFlashTimer >= HIT_FLASH_TIMER_DEFAULT)
        {
            m_hitFlashTimer = HIT_FLASH_TIMER_DEFAULT;
            m_imageNumber = 0;
        }
    }

    //Damage Numbers
    if (m_damageNumberTimer > 0.0)
    {
        m_damageNumberTimer -= frametime;
        if(!m_isExploding) DrawDamageNumbers(m_accumulatedDamage);

        if (m_damageNumberTimer <= 0.0)
        {
            m_accumulatedDamage = 0;
            m_damageNumberTimer = 0.0;
        }
    }

    //Explosion Animation
    if (m_explosionTimer >= 0.0)
    {
        if (!m_isExploding)
        {
            m_mainGun->Deactivate();
            m_mainGun = nullptr;
            m_missileLauncher->Deactivate();
            m_missileLauncher = nullptr;
            int channel = HtAudio::instance.Play(m_explosionSFX, false);
            HtAudio::instance.SetChannelVolume(channel, 0.15);
        }
        m_isExploding = true;
        m_explosionTimer += frametime;


        if (m_explosionTimer >= EXPLOSION_FRAME_DURATION)
        {
            m_explosionTimer -= EXPLOSION_FRAME_DURATION;
            m_currentFrame++;
            m_imageNumber = 2 + m_currentFrame;

            if (m_currentFrame >= EXPLOSION_FRAME_COUNT)
            {
                Deactivate();
            }
        }
    }
}

void Enemy::Navigate()
{
    if (!m_player)
    {
        ErrorLogger::Write("Enemy::Navigate | m_player nullptr");
        return;
    }

    Vector2D playerDirection = (m_player->GetPosition() - GetPosition()).unitVector();

    m_velocity = playerDirection * MOVEMENT_SPEED;
}

void Enemy::DrawDamageNumbers(int damage)
{
    double scale = 1.0;
    if (m_damageNumberTimer > 0.0) scale += (m_damageNumberTimer / DAMAGE_NUMBER_DISPLAY_TIME) * 0.5;
    HtGraphics::instance.WriteTextCentered(m_position + DAMAGE_NUMBER_OFFSET, std::to_string(damage), HtGraphics::instance.RED, HUDFont::Font, 0, scale);
}

bool Enemy::WithinRange()
{
    double distance = (GetPosition() - m_player->GetPosition()).magnitude();
    return distance < WITHIN_RANGE_DISTANCE;
}

void Enemy::Damage(int damage)
{
    m_health -= damage;

    m_accumulatedDamage += damage;
    m_damageNumberTimer = DAMAGE_NUMBER_DISPLAY_TIME;

    if (m_imageNumber == 0)
    {
        m_imageNumber = 1;
        m_hitFlashTimer = 0.0;
    }
}

void Enemy::UponDeath()
{
    m_velocity = Vector2D(0, 0);
    m_imageNumber = 2;
    m_explosionTimer = 0.0;
    m_player->AddMoney(CREDITS_ON_DEATH);
}


void Enemy::ProcessCollision(GameObject& otherObject)
{
    for (ObjectType object : m_allowedCollision)
    {
        if (otherObject.GetType() == object)
        {
            break;
        }
    }
    Projectile* projectile = dynamic_cast<Projectile*>(&otherObject);
    if (!projectile) return;
    if (projectile->GetOwner()->GetType() == this->GetType()) return;
    Damage(projectile->GetDamage());
}

