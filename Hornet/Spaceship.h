#pragma once
#include "GameObject.h"
#include "ErrorLogger.h"
#include "HtAudio.h"


struct Resource
{
    int current;
    int max;

    void SetMax(int inpMax);
    void DecrementCurrent(int value);
    void AddToCurrent(int value);
    void RefillMax();
    int GetMissing();
};

class Projectile;
class Galaxy;
class MainGun;
class MissileLauncher;

class SpaceShip :
    public GameObject
{
public:
    SpaceShip(Galaxy* galaxy);
    ~SpaceShip();

    void Update(double frametime) override;

    void Initialise();
    
    //Collision Handling
    void ProcessCollision(GameObject& otherObject) override;
    IShape2D& GetCollisionShape() override;

    Vector2D GetVelocity() { return m_velocity; }
    void SetVelocity(Vector2D velocity) { m_velocity = velocity; }
    
    //Money
    void AddMoney(int money);
    bool SpendMoney(int amountSpent);
    int GetMoney() { return m_money; }

private:

    Resource m_ammo = {500,1000};
    Resource m_missile = {20,40};
    Resource m_health = { 500,1000 };
    Resource m_armor = { 200,400 };
    Resource m_fuel = { 0,1 };

public:

    void DamagePlayer(int value);
    // Ammo
    void SetMaxAmmo(int value) { m_ammo.SetMax(value); }
    void DecrementAmmo(int value = 1) { m_ammo.DecrementCurrent(value); }
    void RefillAmmo(int amount) { m_ammo.AddToCurrent(amount); }
    void RefillAmmoMax() { m_ammo.RefillMax(); }
    int GetMaxAmmo() { return m_ammo.max; }
    int GetCurrentAmmo() { return m_ammo.current; }
    int GetMissingAmmo() { return m_ammo.GetMissing(); }

    // Missile
    void SetMaxMissile(int value) { m_missile.SetMax(value); }
    void DecrementMissile(int value = 1) { m_missile.DecrementCurrent(value); }
    void RefillMissile(int amount) { m_missile.AddToCurrent(amount); }
    void RefillMissileMax() { m_missile.RefillMax(); }
    int GetMaxMissile() { return m_missile.max; }
    int GetCurrentMissile() { return m_missile.current; }
    int GetMissingMissile() { return m_missile.GetMissing(); }

    // Health
    void SetMaxHealth(int value) { m_health.SetMax(value); }
    void RefillHealth(int amount) { m_health.AddToCurrent(amount); }
    void RefillHealthMax() { m_health.RefillMax(); }
    void DamageHealth(int value) { m_health.DecrementCurrent(value); }
    int GetMaxHealth() { return m_health.max; }
    int GetCurrentHealth() { return m_health.current; }
    int GetMissingHealth() { return m_health.GetMissing(); }

    // Armor
    void SetMaxArmor(int value) { m_armor.SetMax(value); }
    void RefillArmor(int amount) { m_armor.AddToCurrent(amount); }
    void RefillArmorMax() { m_armor.RefillMax(); }
    void DamageArmor(int value) { m_armor.DecrementCurrent(value); }
    int GetMaxArmor() { return m_armor.max; }
    int GetCurrentArmor() { return m_armor.current; }
    int GetMissingArmor() { return m_armor.GetMissing(); }

    //Fuel
    void SetMaxFuel(int value) { m_fuel.SetMax(value); }
    void RefillFuel(int amount) { m_fuel.AddToCurrent(amount); }
    void RefillFuelMax() { m_fuel.RefillMax(); }
    int GetMaxFuel() { return m_fuel.max; }
    int GetCurrentFuel() { return m_fuel.current; }
    int GetMissingFuel() { return m_fuel.GetMissing(); }


private:

    //Movement
    Vector2D m_velocity;
    double m_friction = 125;
    double m_maxSpeed = 500;

    //Collision
    Circle2D m_collisionShape;
    const int COLLISION_SHAPE_SIZE = 30;
    ObjectType m_allowedCollision[2] = { ObjectType::BULLET, ObjectType::MISSILE };
    std::vector<GameObject*> m_recentCollisions;

    //Money
    int m_money = 10000;

    Galaxy* m_currentGalaxy;

    //Main Gun
    MainGun* mainGun;
    const int MAINGUN_DAMAGE = 5;
    const double MAINGUN_COOLDOWN = 0.0353;

    //Missile Launcher
    MissileLauncher* missileLauncher;
    const int MISSILE_DAMAGE = 100;
    const double BARRAGE_COOLDOWN = 5;
};
