#pragma once
#include "SpaceShipWeapon.h"

class MainGun : public SpaceShipWeapon
{
public:

    MainGun(const std::string& image, GameObject* owner);

    void Update(double frametime) override;


    void FireProjectile() override;
    void HandleWeapon(double frametime) override;

    void SetEmptySFX(const std::string& audioPath, const bool isLoop);

private:
    SoundIndex m_emptySFX = SoundIndex(-1);
    int m_emptyGunChannel = -1;
    bool m_emptySFXIsLoop = false;
    bool m_emptyGunSFXPlaying = false;
};

