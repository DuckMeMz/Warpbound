#include "Missile.h"
#include "ObjectManager.h"
#include "Enemy.h"
#include "ErrorLogger.h"
#include "Spaceship.h"

Missile::Missile(const Vector2D& position, const Vector2D& velocity,  int damage, const std::string& imagePath, GameObject* owner) : Projectile(position, velocity, damage, imagePath,ObjectType::MISSILE, owner)
{
    SetCollidable();
    m_lifetimeTimer = GetLifetime();
    m_scale = 1.5;
    FindTarget();
}

void Missile::Update(double frametime)
{
    Projectile::Update(frametime);
    Navigate();
    if (m_target && !m_target->IsActive()) Deactivate();
}

void Missile::Navigate()
{
    if (!m_target) FindTarget();
    if (!m_target) return;
        

    Vector2D targetVelocity;

    if (m_target->GetType() == ObjectType::SPACESHIP)
    {
        SpaceShip* target = static_cast<SpaceShip*>(m_target);
        targetVelocity = target->GetVelocity();
    }
    else if (m_target->GetType() == ObjectType::ENEMY)
    {
        Enemy* target = static_cast<Enemy*>(m_target);
        targetVelocity = target->GetVelocity();
    }
    else
    {
        return;
    }
   

    Vector2D enemyPos = m_target->GetPosition();
    double distance = (enemyPos - GetPosition()).magnitude();
    double time = distance / m_velocity.magnitude();

    Vector2D targetPos = enemyPos + targetVelocity * time;
    Vector2D interceptDirection = (targetPos - GetPosition()).unitVector();

    Vector2D turningDirection = m_velocity.unitVector().lerp(interceptDirection, static_cast<float>(TURN_RATE)).unitVector();

    m_velocity = turningDirection * MOVEMENT_SPEED;
}

void Missile::SetTarget(GameObject* target)
{
    if (target) m_target = target;
    else ErrorLogger::Write("Missile::SetTarget | target is nullptr");
}

void Missile::FindTarget()
{
    if (m_owner->GetType() == ObjectType::SPACESHIP)
    {
        std::vector<GameObject*> enemies = ObjectManager::instance.GetAllObjectsOfType(ObjectType::ENEMY);

        if (enemies.empty())
        {
            m_target = nullptr;
            return;
        }
        for (GameObject* enemy : enemies)
        {
            bool alreadyTargeted = false;

            std::vector<GameObject*> allObjects = ObjectManager::instance.GetAllObjectsOfType(ObjectType::MISSILE);
            for (GameObject* gameObject : allObjects) //Works for now maybe a better way 
            {
                Missile* missile = dynamic_cast<Missile*>(gameObject);
                if (missile != this && missile->GetTarget() == enemy)
                {
                    alreadyTargeted = true;
                    break;
                }
            }
            if (!alreadyTargeted)
            {
                m_target = enemy;
                return;
            }
        }
        m_target = enemies[0];
    }
    else
    {
        m_target = ObjectManager::instance.FindClosestObject(Vector2D(0, 0), ObjectType::SPACESHIP);
    }
   
}