#pragma once
#include "SpaceShipWeapon.h"

class MissileLauncher : public SpaceShipWeapon
{
public:
    MissileLauncher(const std::string& image, GameObject* owner);

    void Update(double frametime) override;

    void FireProjectile() override;
    void HandleWeapon(double frametime) override;

    void SetBarrageParams(int missilesPerBarrage, double timerPerMissile);

private:
    
    int m_missilesLeftInBarrage = 0;
    int m_missilePerBarrage = 5;
    double m_perMissileTimer = -1.0;
    double m_perMissileTimerDefault = 0.5; 
    

};

