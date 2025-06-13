//Engine Core
#include "HtCamera.h"
#include "HtGraphics.h"
#include "ErrorLogger.h"
#include "HtKeyboard.h"

//Dependencies
#include "HUD.h"
#include "SlidingBar.h"
#include "Counter.h"
#include "HUDAnchors.h"
#include "HUDFont.h"
#include "Spaceship.h"
#include "ObjectManager.h"

HUD HUD::instance;

HUD::HUD()
{

}
HUD::~HUD()
{
}

void HUD::Shutdown()
{
    for (HUDElement* element : m_hudElements)
    {
        delete element;
    }
    m_hudElements.clear();
    m_hudElements.shrink_to_fit();
}

void HUD::Initialise(SpaceShip* pPlayer)
{
    Shutdown();

    //Init HUD Packages
    HUDAnchors::Initialise();
    HUDFont::Initialise();

    //Health Bar Init
    HealthBar* healthBar = new HealthBar(HUDAnchors::BottomLeft, HEALTH_BAR_OFFSET, 100, pPlayer, "assets/shop/health-icon.png");
    healthBar->SetColours(HtGraphics::instance.RED, HtGraphics::instance.CYAN, HtGraphics::instance.GREY, HtGraphics::instance.WHITE);
    healthBar->SetDimensions(250, 45);
    m_hudElements.push_back(healthBar);

    ArmorBar* armorBar = new ArmorBar(HUDAnchors::BottomLeft, ARMOR_BAR_OFFSET, 100, pPlayer, "assets/shop/armor-icon.png");
    armorBar->SetColours(HtGraphics::instance.BLUE, HtGraphics::instance.CYAN, HtGraphics::instance.GREY, HtGraphics::instance.WHITE);
    armorBar->SetDimensions(250, 45);
    m_hudElements.push_back(armorBar);

    AmmoCounter* ammoCount = new AmmoCounter(HUDAnchors::BottomLeft, AMMO_COUNTER_OFFSET, 50, 100, pPlayer, "assets/shop/ammo-icon.png");
    ammoCount->SetColour(HtGraphics::instance.WHITE);
    m_hudElements.push_back(ammoCount);

    MissileCounter* missileCount = new MissileCounter(HUDAnchors::BottomLeft, MISSILE_COUNTER_OFFSET, 50, 100, pPlayer, "assets/shop/missile-icon.png");
    missileCount->SetColour(HtGraphics::instance.WHITE);
    m_hudElements.push_back(missileCount);

    m_panelBackground.PlaceAt(
        HUDAnchors::BottomLeft + Vector2D(-175, -125),
        HUDAnchors::BottomLeft + Vector2D(600, 50)
    );

    m_panelOutline.PlaceAt(
        HUDAnchors::BottomLeft + Vector2D(-175, -125),
        HUDAnchors::BottomLeft + Vector2D(600, 50)
    );
 


    if (!pPlayer)
    {
        ErrorLogger::Write("HUD::Initialise | pPlayer nullptr");
        return;
    }

    m_player = pPlayer;
}

void HUD::Update(double frametime)
{
    for (HUDElement* element : m_hudElements)
    {
        element->Update();
    }
}

void HUD::Render()
{ 
    if (m_isActive)
    {
        HtCamera::instance.UseCamera(false);
        HtGraphics::instance.FillRect(m_panelBackground, Colour(70, 128, 128, 128));
        HtGraphics::instance.DrawRect(m_panelOutline, HtGraphics::instance.GREY, 1);
        for (HUDElement* element : m_hudElements)
        {
            element->Render();
        }
        HtCamera::instance.UseCamera(true);
    }
}