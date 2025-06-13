#include "Spaceship.h"
#include "HtKeyboard.h"
#include "ErrorLogger.h"
#include "ObjectManager.h"
#include "MainGun.h"
#include "MissileLauncher.h"
#include "Projectile.h"
#include "HtCamera.h"
#include "HtMouse.h"
#include "Galaxy.h"
#include <iostream>
#include <algorithm>



SpaceShip::SpaceShip(Galaxy* galaxy) : GameObject(ObjectType::SPACESHIP), m_currentGalaxy(galaxy)
{
    m_position = { 0,0 };
    SetCollidable();
    m_scale = 5;


    LoadImage("assets/player/ship/ship-25.png");
    LoadImage("assets/player/ship/ship-50.png");
    LoadImage("assets/player/ship/ship-75.png");
    LoadImage("assets/player/ship/ship-100.png");
 
    //Set to full health image. 
    m_imageNumber = 3;

    mainGun = new MainGun("assets/empty.png", this);
    mainGun->InitSound("assets/sfx/machine-gun.wav", true);
    mainGun->SetEmptySFX("assets/sfx/empty-machine-gun.mp3", true);
    mainGun->SetCooldown(MAINGUN_COOLDOWN);
    mainGun->InitProjectile(10, 10, 2000, 30);
    ObjectManager::instance.AddItem(mainGun);

    missileLauncher = new MissileLauncher("assets/empty.png", this);
    missileLauncher->InitSound("assets/sfx/missile-launch.mp3", false);
    missileLauncher->SetCooldown(BARRAGE_COOLDOWN);
    missileLauncher->InitProjectile(100, 300, 200, 30);
    ObjectManager::instance.AddItem(missileLauncher);
    
}

SpaceShip::~SpaceShip()
{
}

void SpaceShip::Initialise()
{
    HtMouse::instance.FakeLeftClick();
}

void SpaceShip::Update(double frametime)
{
    Vector2D acceleration;
    //Point to cursor
    Vector2D pointer = HtMouse::instance.GetPointerGamePosition();
    Vector2D displacement = { pointer.XValue - m_position.XValue, pointer.YValue - m_position.YValue };
    m_angle = (atan2(displacement.XValue, displacement.YValue)) * 180 / M_PI;

    if (m_currentGalaxy->m_waveCooldownActive)
    {
        Segment2D PointOfReference;
        PointOfReference.PlaceAt(Vector2D(0, 0), m_position);
        HtGraphics::instance.DrawSegment(PointOfReference, Colour(255, 0, 255, 0));
        /*Segment2D MouseToPlayer;
        MouseToPlayer.PlaceAt(m_position, pointer);
        HtGraphics::instance.DrawSegment(MouseToPlayer, Colour(255, 255, 255, 0));*/
    }

    bool isAccelerating = false;
    if (HtKeyboard::instance.KeyPressed(SDL_SCANCODE_W))
    {
        acceleration.setBearing(m_angle, m_maxSpeed);
        isAccelerating = true;
    }
    if (HtKeyboard::instance.KeyPressed(SDL_SCANCODE_S))
    {
        acceleration.setBearing(m_angle, -m_maxSpeed);
        isAccelerating = true;
    }
    if (HtKeyboard::instance.KeyPressed(SDL_SCANCODE_A))
    {
        Vector2D left;
        left.setBearing(m_angle - 90.0, m_maxSpeed);
        acceleration = acceleration + left;
        isAccelerating = true;
    }
    if (HtKeyboard::instance.KeyPressed(SDL_SCANCODE_D))
    {
        Vector2D right;
        right.setBearing(m_angle + 90.0, m_maxSpeed);
        acceleration = acceleration + right;
        isAccelerating = true;
    }

    //Add friction when not accelerating
    if (isAccelerating)
    {
        m_velocity = m_velocity + acceleration * frametime;
    }
    else
    {
        m_velocity = m_velocity + (m_velocity.unitVector() * -m_friction) * frametime;
    }

    //Cap Speed
    if (m_velocity.magnitude() > m_maxSpeed)
    {
        m_velocity = m_velocity.unitVector() * m_maxSpeed;
    }

    //Position + Boundry
    m_position = m_position + m_velocity * frametime;
    if (m_position.XValue < -10000.0)
    {
        m_position.XValue = -9990.0;
        if (m_velocity.XValue < 0) m_velocity.XValue = 0;
    }
    else if (m_position.XValue > 10000.0)
    {
        m_position.XValue = 9990.0;
        if (m_velocity.XValue > 0) m_velocity.XValue = 0;
    }

    if (m_position.YValue < -5000.0)
    {
        m_position.YValue = -4990.0;
        if (m_velocity.YValue < 0) m_velocity.YValue = 0;
    }
    else if (m_position.YValue > 5000.0)
    {
        m_position.YValue = 4990.0;
        if (m_velocity.YValue > 0) m_velocity.YValue = 0;
    }
    //Camera Position
    HtCamera::instance.PlaceAt(m_position);

    //Stop movement when too slow
    if (!isAccelerating && m_velocity.magnitude() < 5.0)
    {
        m_velocity = Vector2D(0, 0);
    }
    
    m_collisionShape.PlaceAt(m_position, COLLISION_SHAPE_SIZE);

    if (GetMaxHealth() > 0)
    {
        //Set the damage state. 4 States, 25% per state.
        m_imageNumber = (GetCurrentHealth() * 4) / GetMaxHealth() -1 ;
        if (m_imageNumber == -1) m_imageNumber = 0;
    }

    if (!ObjectManager::instance.FindClosestObject(Vector2D(0, 0), ObjectType::MISSILE) && !ObjectManager::instance.FindClosestObject(Vector2D(0,0), ObjectType::BULLET))
    {
        m_recentCollisions.clear();
    }
}

void SpaceShip::AddMoney(int money)
{
    double scaledMoney = std::lerp(m_currentGalaxy->MIN_MONEY, m_currentGalaxy->MAX_MONEY, m_currentGalaxy->GetDifficulty());
    m_money += static_cast<int>(scaledMoney) + money;
}

bool SpaceShip::SpendMoney(int amountSpent)
{
    if (m_money < amountSpent)
    {
        return false;
    }

    m_money -= amountSpent;
    return true;
}

void SpaceShip::DamagePlayer(int damage)
{
    int currentArmor = GetCurrentArmor();
    if (currentArmor >= damage)
    {
        DamageArmor(damage);
    }
    else
    {
        int leftoverDamage = damage - currentArmor;
        DamageArmor(currentArmor); 
        DamageHealth(leftoverDamage);
    }
}

void SpaceShip::ProcessCollision(GameObject& otherObject)
{
    bool isAllowed = false;
    for (ObjectType type : m_allowedCollision)
    {
        if (type == otherObject.GetType())
        {
            isAllowed = true;
            break;
        }
    }
    if (!isAllowed) return;

    Projectile* projectile = dynamic_cast<Projectile*>(&otherObject);
    if (!projectile) return;

    if (projectile->GetOwner() == this) return;

    //if it's already collided this wave, don't collided again
    if (std::find(m_recentCollisions.begin(), m_recentCollisions.end(), &otherObject) != m_recentCollisions.end()) return;
  
    m_recentCollisions.push_back(&otherObject);

    DamagePlayer(projectile->GetDamage());
}

IShape2D& SpaceShip::GetCollisionShape()
{
    return m_collisionShape;
}

//Resource Struct

void Resource::SetMax(int inpMax)
{
    if (inpMax < 0)
    {
        ErrorLogger::Write("Resource::SetMax | inpMax can't be < 0 ");
        return;
    }
    max = inpMax;

    if (current > max) current = max;
}

void Resource::DecrementCurrent(int value)
{
    if (value < 0)
    {
        ErrorLogger::Write("Resource::DecrementCurrent | value can't be < 0");
        return;
    }
    if (current - value < 0)
    {
        current = 0;
    }
    else
    {
        current -= value;
    }
}

void Resource::AddToCurrent(int value)
{
    if (value < 0)
    {
        ErrorLogger::Write("Resource::AddToCurrent | Value can't be < 0 ");
        return;
    }
    if (value > max)
    {
        ErrorLogger::Write("Resource::AddToCurrent | Value can't be > Max");
        return;
    }
    if (current + value > max)
    {
        ErrorLogger::Write("Resource::AddToCurrent | Current + Value can't be > Max");
        return;
    }
    current = current + value;
}

void Resource::RefillMax()
{
    current = max;
}

int Resource::GetMissing()
{
    return max - current;
}