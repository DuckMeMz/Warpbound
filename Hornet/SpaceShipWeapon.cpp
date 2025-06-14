#include "SpaceShipWeapon.h"
#include "ObjectManager.h"

SpaceShipWeapon::SpaceShipWeapon(const std::string& image, GameObject* owner)
{
    LoadImage(image.c_str());
    m_owner = owner;
    if (m_owner->GetType() == ObjectType::SPACESHIP) m_ownerIsPlayer = true;
}

SpaceShipWeapon::~SpaceShipWeapon()
{
}

void SpaceShipWeapon::Initialise()
{

}

void SpaceShipWeapon::Update(double frametime)
{
    m_position = m_owner->GetPosition();
    m_angle = m_owner->GetAngle();
    //Deactivate if owner is nullptr, or deactivated
    if (!m_owner)
    {
        Deactivate();
    }
    else if (!m_owner->IsActive())
    {
        Deactivate();
    }
    HandleWeapon(frametime);
}

void SpaceShipWeapon::InitSound(const std::string& audioPath, bool isLoop)
{
    m_fireSFX = HtAudio::instance.LoadSound(audioPath);
    m_sfxIsLoop = isLoop;
}

void SpaceShipWeapon::InitProjectile(const int recoil, const int damage, const int projectileSpeed, const int forwardSpawnOffset)
{
    if (recoil > 0) m_recoil = recoil;
    SetDamage(damage/2);
    if (projectileSpeed > 0) m_projectileSpeed = projectileSpeed;
    m_forwardSpawnOffset = forwardSpawnOffset;
}
