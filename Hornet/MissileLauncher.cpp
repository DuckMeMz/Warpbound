#include "MissileLauncher.h"
#include "ObjectManager.h"
#include "HtAudio.h"
#include "HtMouse.h"
#include "Missile.h"
#include "Spaceship.h"

MissileLauncher::MissileLauncher(const std::string& image, GameObject* owner)
    : SpaceShipWeapon(image, owner)
{
}

MissileLauncher::~MissileLauncher()
{

}

void MissileLauncher::Update(double frametime)
{
    SpaceShipWeapon::Update(frametime);
    HandleWeapon(frametime);
}

void MissileLauncher::HandleWeapon(double frametime)
{
    if (m_cooldown >= 0.0) m_cooldown += frametime;
    if (m_cooldown >= GetCooldown()) m_cooldown = -1.0;

    bool isFiring = false;
    if (m_ownerIsPlayer)
    {
        isFiring = HtMouse::instance.IsMouseDown(HtMouseButton::RIGHT);
    }
    else
    {
        isFiring = true;
    }

    if (isFiring && m_cooldown == -1.0)
    {
        m_cooldown = 0.0;
        m_missilesLeftInBarrage = m_missilePerBarrage;
        m_perMissileTimer = 0.0;
    }

    if (m_missilesLeftInBarrage > 0)
    {
        m_perMissileTimer += frametime;

        if (m_perMissileTimer >= m_perMissileTimerDefault && m_missilesLeftInBarrage > 0)
        {
            SpaceShip* player = static_cast<SpaceShip*>(m_owner);
            if (player->GetCurrentMissile() <= 0) return;
            FireProjectile();
            if (m_ownerIsPlayer)
            {
                m_fireChannel = HtAudio::instance.Play(m_fireSFX, m_sfxIsLoop);
                HtAudio::instance.SetChannelVolume(m_fireChannel, 0.5);
                m_missilesLeftInBarrage--;
                m_perMissileTimer -= m_perMissileTimerDefault;
            }
        }
    }
}

void MissileLauncher::SetBarrageParams(int missilesPerBarrage, double timerPerMissile)
{
    if (missilesPerBarrage > 0) m_missilePerBarrage = missilesPerBarrage;
    if (timerPerMissile > 0) m_perMissileTimerDefault = timerPerMissile;
}

void MissileLauncher::FireProjectile()
{
    if (m_ownerIsPlayer)
    {
        SpaceShip* player = static_cast<SpaceShip*>(m_owner);
        player->DecrementMissile();
        Vector2D recoil;
        recoil.setBearing(m_angle + 180, m_recoil);
        player->SetVelocity(player->GetVelocity() + recoil);
    }

    Vector2D missileOffset, missileVelocity;

    missileOffset.setBearing(m_angle, m_forwardSpawnOffset);
    missileVelocity.setBearing(m_angle, m_projectileSpeed);

    Missile* pMissile = new Missile(m_position, missileVelocity, GetDamage(), "assets/shop/missile-icon-upright.png", m_owner);
    ObjectManager::instance.AddItem(pMissile);
}
