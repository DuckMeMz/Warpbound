#pragma once
#include "Projectile.h"

class Enemy; 
class SpaceShip;

class Missile : public Projectile
{
public:
    Missile(const Vector2D& position, const Vector2D& velocity, int damage, const std::string& imagePath, GameObject* owner);

    void Update(double frametime) override;

    virtual void Navigate();

    void SetPlayer(SpaceShip* player) { m_player = player; }

    GameObject* GetTarget() { return m_target; }
    void SetTarget(GameObject* target);
    virtual void FindTarget();

    double GetMovementSpeed() override { return MOVEMENT_SPEED; }
    double GetLifetime() override { return LIFETIME; }

protected:
    GameObject* m_target;
    SpaceShip* m_player;

    const double MOVEMENT_SPEED = 500;
    const double LIFETIME = 5;
    const double TURN_RATE = 0.0125;

};