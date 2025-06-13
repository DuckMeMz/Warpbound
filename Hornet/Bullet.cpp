#include "Bullet.h"

const double Bullet::LIFETIME = 2.5;
const double Bullet::MOVEMENT_SPEED = 5;

Bullet::Bullet(const Vector2D& position, const Vector2D& velocity, const std::string& imagePath, int damage, GameObject* owner) : Projectile(position, velocity, damage, imagePath,ObjectType::BULLET, owner)
{
    SetCollidable();
    m_lifetimeTimer = GetLifetime();
    m_scale = 1.5;
}
