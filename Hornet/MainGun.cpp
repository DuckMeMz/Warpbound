#include "MainGun.h"
#include "Spaceship.h"
#include "HtMouse.h"
#include "ObjectManager.h"
#include "Bullet.h"


MainGun::MainGun(const std::string& image, GameObject* owner) : SpaceShipWeapon(image, owner)
{
}

void MainGun::Update(double frametime)
{
    SpaceShipWeapon::Update(frametime);
}

void MainGun::FireProjectile()
{
    //Recoil if player
    if (m_ownerIsPlayer)
    {
        SpaceShip* player = static_cast<SpaceShip*>(m_owner);
        Vector2D recoil;
        //Apply recoil backwards
        recoil.setBearing(m_angle + 180, m_recoil);
        player->SetVelocity(player->GetVelocity() + recoil);
    }

    Vector2D bulletOffset, bulletVelocity;

    bulletOffset.setBearing(m_angle, m_forwardSpawnOffset);
    bulletVelocity.setBearing(m_angle, m_projectileSpeed);

    Bullet* pBullet = new Bullet(m_position, bulletVelocity, "assets/shop/bullet-icon.png", GetDamage(), m_owner);
    ObjectManager::instance.AddItem(pBullet);
}

void MainGun::SetEmptySFX(const std::string& audioPath, const bool isLoop)
{
    m_emptySFX = HtAudio::instance.LoadSound(audioPath);
    m_emptySFXIsLoop = isLoop;
}

void MainGun::HandleWeapon(double frametime)
{
    if (m_cooldown >= 0.0) m_cooldown += frametime;
    if (m_cooldown >= GetCooldown()) m_cooldown = -1.0;
        
    bool isFiring = false;
    if (m_ownerIsPlayer)
    {
        isFiring = HtMouse::instance.IsMouseDown(HtMouseButton::LEFT);
    }
    else
    {
        isFiring = true;
    }

    if (isFiring && m_cooldown == -1.0)
    {
        int ammo = 1;
        if (m_ownerIsPlayer)
        {
            SpaceShip* player = static_cast<SpaceShip*>(m_owner);
            ammo = player->GetCurrentAmmo();
            if (ammo > 0) player->DecrementAmmo();
        }
        if (ammo > 0)
        {
            FireProjectile();
            m_cooldown = 0.0; //Start Timer
            if (!m_SFXIsPlaying && m_ownerIsPlayer)
            {
                m_fireChannel = HtAudio::instance.Play(m_fireSFX, m_sfxIsLoop);
                HtAudio::instance.SetChannelVolume(m_fireChannel, 0.20);
                m_SFXIsPlaying = true;
            }
        }
        else
        {
            if (!m_emptyGunSFXPlaying && m_ownerIsPlayer)
            {
                m_emptyGunChannel = HtAudio::instance.Play(m_emptySFX, m_emptySFXIsLoop);
                HtAudio::instance.SetChannelVolume(m_emptyGunChannel, 0.20);
                m_emptyGunSFXPlaying = true;
            }

        }
    }
    if (!isFiring)
    {
        if (m_SFXIsPlaying)
        {
            HtAudio::instance.Stop(m_fireChannel);
            m_SFXIsPlaying = false;
        }
        if (m_emptyGunSFXPlaying)
        {
            HtAudio::instance.Stop(m_emptyGunChannel);
            m_emptyGunSFXPlaying = false;
        }
    }
}