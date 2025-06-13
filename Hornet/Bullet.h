#pragma once
#include "Projectile.h"

class Bullet : public Projectile
{
public:
    Bullet(const Vector2D& position, const Vector2D& velocity, const std::string& imagePath, int damage, GameObject* owner);

    double GetMovementSpeed() override { return MOVEMENT_SPEED; }
    double GetLifetime() override { return LIFETIME; }

private:
    static PictureIndex m_image;

    static const double LIFETIME;
    static const double MOVEMENT_SPEED;

};