#pragma once
#include "GameObject.h"
#include "HtAudio.h"

class SpaceShip;
class MainGun;
class MissileLauncher;

class Enemy :
    public GameObject
{
public:
    Enemy(const Vector2D& position, const int health, SpaceShip* player);
    Vector2D GetVelocity() { return m_velocity; }
    void SetVelocity(Vector2D velocity) { m_velocity = velocity; }

    void Update(double frametime) override;

    bool WithinRange();

    void Navigate();
    
    void DrawDamageNumbers(int damage);

    //Collision + Damage
    void ProcessCollision(GameObject& otherObject) override;
    IShape2D& GetCollisionShape() override { return m_collisionShape; }
    void Damage(int damage);
    void UponDeath();

private:
    Vector2D m_velocity;
    int m_health = 100;
    SpaceShip* m_player;

    MainGun* m_mainGun = nullptr;
    MissileLauncher* m_missileLauncher = nullptr;

    static const double MOVEMENT_SPEED;

    //Collision
    Circle2D m_collisionShape;
    int m_collisionSize = 30;
    ObjectType m_allowedCollision[2] = { ObjectType::BULLET, ObjectType::MISSILE };

    double m_hitFlashTimer = -1.0;
    const double HIT_FLASH_TIMER_DEFAULT = 0.05;

    //Damage Numbers

    int m_accumulatedDamage = 0;
    double m_damageNumberTimer = -1.0;
    const int DAMAGE_NUMBER_DISPLAY_TIME = 3; 

    const int WITHIN_RANGE_DISTANCE = 500;

    //Weapon Params
    //Main Gun
    const int BULLET_DAMAGE = 10;
    const double BULLET_COOLDOWN = 1.25;
    //Missile
    const double MISSILE_COOLDOWN = 10;
    const double TIMER_PER_MISSILE = 1;
    const int MISSILE_DAMAGE = 50;
    const int MISSILE_SPEED = 200;

    const Vector2D DAMAGE_NUMBER_OFFSET = Vector2D(0, 75);

    const int CREDITS_ON_DEATH = 25;

    //Explosion
    bool m_isExploding = false;
    double m_explosionTimer = -1.0;
    const double EXPLOSION_FRAME_DURATION = 0.1;
    const int EXPLOSION_FRAME_COUNT = 8;
    int m_currentFrame = 0;
    SoundIndex m_explosionSFX;

 

};

