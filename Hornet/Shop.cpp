//Engine Core
#include "HtCamera.h"
#include "HtKeyboard.h"
#include "ErrorLogger.h"
#include "ObjectManager.h"

//Dependencies
#include "Shop.h"
#include "Cursor.h"
#include "HUDFont.h"
#include "HUDAnchors.h"

Shop::Shop() : GameObject(ObjectType::SHOP)
{
    SetHandleEvents();
}
Shop::~Shop()
{
    for (ShopItem* item : m_items)
    {
        delete item;
    }
    m_items.clear();
}
void Shop::Initialise(SpaceShip* player, Galaxy* galaxy)
{
    double fontScale = 0.75;
    double playerRestockXOffset[5] = {
        -700, //Ammo
        -350, //Missile
        0, //Fuel
        350, //Health
        700  //Armor
    };
    double playerRestockYOffset = 200;
    Vector2D PlayerRestockAnchor = Vector2D(0, 0);
    const int BUTTON_WIDTH = 60;
    const int BUTTON_HEIGHT = 40;
    AmmoShopItem* ammoShopItem = new AmmoShopItem("Ammo", "assets/shop/ammo-icon.png", PlayerRestockAnchor + Vector2D(playerRestockXOffset[0], playerRestockYOffset), player, galaxy);
    ammoShopItem->Initialise(4);
    ammoShopItem->ButtonInit(BUTTON_WIDTH, BUTTON_HEIGHT, HtGraphics::instance.GREY, HtGraphics::instance.LIGHTGREEN, HUDFont::Font, fontScale);
    m_items.push_back(ammoShopItem);

    MissileShopItem* missileShopItem = new MissileShopItem("Missile", "assets/shop/missile-icon.png", PlayerRestockAnchor + Vector2D(playerRestockXOffset[1], playerRestockYOffset), player, galaxy);
    missileShopItem->Initialise(120);
    missileShopItem->ButtonInit(BUTTON_WIDTH, BUTTON_HEIGHT, HtGraphics::instance.GREY, HtGraphics::instance.LIGHTGREEN, HUDFont::Font, fontScale);
    m_items.push_back(missileShopItem);

    FuelShopItem* fuelShopItem = new FuelShopItem("Fuel", "assets/shop/fuel-icon.png", PlayerRestockAnchor + Vector2D(playerRestockXOffset[2], playerRestockYOffset), player, galaxy);
    fuelShopItem->Initialise(10000);
    fuelShopItem->ButtonInit(120, BUTTON_HEIGHT, HtGraphics::instance.GREY, HtGraphics::instance.LIGHTGREEN, HUDFont::Font, fontScale);
    m_items.push_back(fuelShopItem);

    HealthShopItem* healthShopItem = new HealthShopItem("Health", "assets/shop/health-icon.png", PlayerRestockAnchor + Vector2D(playerRestockXOffset[3], playerRestockYOffset), player, galaxy);
    healthShopItem->Initialise(5);
    healthShopItem->ButtonInit(BUTTON_WIDTH, BUTTON_HEIGHT, HtGraphics::instance.GREY, HtGraphics::instance.LIGHTGREEN, HUDFont::Font, fontScale);
    m_items.push_back(healthShopItem);

    ArmorShopItem* armorShopItem = new ArmorShopItem("Armor", "assets/shop/armor-icon.png", PlayerRestockAnchor + Vector2D(playerRestockXOffset[4], playerRestockYOffset), player, galaxy);
    armorShopItem->Initialise(10);
    armorShopItem->ButtonInit(BUTTON_WIDTH, BUTTON_HEIGHT, HtGraphics::instance.GREY, HtGraphics::instance.LIGHTGREEN, HUDFont::Font, fontScale);
    m_items.push_back(armorShopItem);

    m_outline.PlaceAt(
        Vector2D(HUDAnchors::BottomLeft + Vector2D(0, 100)),
        Vector2D(HUDAnchors::TopRight + Vector2D(0, 100)));

    Cursor::instance.Initalise("assets/shop/crosshair.png");
}

void Shop::Update(double frametime)
{
    if (!m_uponEnterFlag)
    {
        m_uponEnterFlag = true;
        UponEnter();
    }
    HtGraphics::instance.WriteTextCentered(HUDAnchors::TopCentre, "Press  E  to leave space station", HtGraphics::instance.CYAN, HUDFont::Font);
    if (HtKeyboard::instance.NewKeyPressed(SDL_SCANCODE_E))
    {
        UponLeave();
    }
    for (ShopItem* item : m_items)
    {
        item->Update(frametime);
    }

    HtGraphics::instance.WriteTextCentered(PlayerRestockAnchor + Vector2D(0, 150), "Restock", HtGraphics::instance.GREY, HUDFont::Font, 0, 2);
}

void Shop::Render()
{
    for (ShopItem* item : m_items)
    {
        item->Render();
    }
    HtGraphics::instance.DrawRect(m_outline, HtGraphics::instance.GREY, 2);
}

void Shop::UponEnter()
{
    Cursor::instance.ToggleVisiblity(true);
    HtCamera::instance.PlaceAt(Vector2D(0, 0));
    for (ShopItem* item : m_items)
    {
        item->UponEnter();
    }
}

void Shop::UponLeave()
{
    m_uponEnterFlag = false;
    ObjectManager::instance.SetCurrentScene(0);
}

void Shop::ForceLeave()
{
    UponLeave();
}
