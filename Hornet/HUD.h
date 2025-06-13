#pragma once
#include "HtGraphics.h"
#include "HUDElement.h"

class SpaceShip;

class HUD
{
public:

    static HUD instance;

    HUD();

    ~HUD();

    void Initialise(SpaceShip* pPlayer);

    void Update(double frametime);

    void Render();
    void RenderCredits();

    void SetActive(bool active) { m_isActive = active; }
    
    void Shutdown();

private:

    bool m_isActive = true;

    const Vector2D HEALTH_BAR_OFFSET = Vector2D(50,0);
    const Vector2D ARMOR_BAR_OFFSET = Vector2D(50,-65);
    const Vector2D AMMO_COUNTER_OFFSET = Vector2D(450, 0);
    const Vector2D MISSILE_COUNTER_OFFSET = Vector2D(450 , -65);
    const Vector2D CREDITS_OFFSET = Vector2D(0, 150);

    //Panel
    Rectangle2D m_panelBackground;
    Rectangle2D m_panelOutline;
  
    std::vector<HUDElement*> m_hudElements;

    SpaceShip* m_player = nullptr;
};