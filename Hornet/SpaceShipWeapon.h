#pragma once
#include "GameObject.h"
#include "HtAudio.h"

class SpaceShipWeapon : public GameObject
{
public:
    SpaceShipWeapon(const std::string& image, GameObject* owner);

    virtual void Initialise();
    virtual void Update(double frametime) override;

    virtual void FireProjectile() = 0;

    //To keep Update clean
    virtual void HandleWeapon(double frametime) = 0;

    void SetDamage(int damage) { if (damage > 0) m_damage = damage;}
    int GetDamage() { return m_damage; }

    void SetCooldown(double cooldown) { if (cooldown > 0.0) m_defaultCooldown = cooldown; }
    double GetCooldown() { return m_defaultCooldown; }

    void InitSound(const std::string& audioPath, bool isLoop);
    void InitProjectile(const int recoil, const int damage, const int projectileSpeed, const int forwardSpawnOffset);

protected:

    GameObject* m_owner;
    //Allows for different weapon behaviour between player and enemy.
    bool m_ownerIsPlayer = false;

    bool m_isFiring = false;
    //Projectile Params
    int m_recoil = 10;
    int m_projectileSpeed = 2000;
    int m_forwardSpawnOffset = 30;

    //Cooldown
    double m_cooldown = -1.0;


    //Sound
    SoundIndex m_fireSFX = SoundIndex(-1);
    int m_fireChannel = -1;
    bool m_sfxIsLoop = false;
    bool m_SFXIsPlaying = false;

private: 

    int m_damage = 1;
    double m_defaultCooldown = 3;
};

