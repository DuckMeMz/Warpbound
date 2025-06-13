#include "Projectile.h"
#include "Enemy.h"
#include "Spaceship.h"
#include "ObjectManager.h"


Projectile::Projectile(const Vector2D& position, const Vector2D& velocity, int damage,
    const std::string& imagePath, ObjectType objType, GameObject* owner)
    : GameObject(objType), m_velocity(velocity), m_damage(damage), m_owner(owner)
{
    SetCollidable();
    LoadImage(imagePath.c_str());
    m_position = position;
    m_lifetimeTimer = 1.0;
}

void Projectile::Update(double frametime)
{
    m_position += m_velocity * frametime;

    m_angle = m_velocity.angle();

    m_collisionShape.PlaceAt(GetPosition(), 30);

    m_lifetimeTimer -= frametime;

    if (m_lifetimeTimer <= 0.0)
    {
        Deactivate();
    }
}


void Projectile::ProcessCollision(GameObject& otherObject)
{
    if (&otherObject == m_owner) return; 
    //Projectiles can't shoot other projectiles
    if (this->GetType() == otherObject.GetType()) return;
    ObjectType allowedCollision = ObjectType::UNKNOWN;
    ObjectType owner = m_owner->GetType();
  
    if (otherObject.GetType() == ObjectType::MISSILE && GetType() == ObjectType::BULLET)
    {
        Projectile* missile = static_cast<Projectile*>(&otherObject);
        //Can't shoot own missiles
        if (GetOwner() == missile->GetOwner()) return;
        //Enemies can't shoot spaceship missiles 
        if (missile->GetOwner()->GetType() == ObjectType::SPACESHIP) return;
        if (!m_hasHit)
        {
            m_hasHit = true;
            otherObject.Deactivate();
            m_damage = 0;
        }
        Deactivate();
        return;
    }

    //If enemy shot bullet only damage player and vice versa.
    if (owner == ObjectType::SPACESHIP) allowedCollision = ObjectType::ENEMY;
    else if (owner == ObjectType::ENEMY) allowedCollision = ObjectType::SPACESHIP;

    if (otherObject.GetType() == allowedCollision)
    {
        if (!m_hasHit)
        {
            m_hasHit = true;
            otherObject.ProcessCollision(*this);
            m_damage = 0;
        }
        Deactivate();
    }
}