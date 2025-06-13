#pragma once
#include "HtGraphics.h"
#include "HtAudio.h"

class Button;
class SpaceShip;
class Galaxy;



class ShopItem
{
public:
    ShopItem(const std::string& name, const std::string& iconPath, const Vector2D& position, SpaceShip* player, Galaxy* galaxy); 
    virtual ~ShopItem();

    virtual void Initialise(int baseCost);
    virtual void Update(double frametime);
    virtual void Render();

    virtual void ButtonInit(int buttonWidth, int buttonHeight, const Colour& buttonColour, const Colour& buttonTextColour, const FontIndex& buttonFont, double fontScale = 1.0);

    virtual int GetCustomMissing() = 0;
    virtual void SetCustomRefill(int value) = 0;

    virtual void OnHalfButtonClick();
    virtual void OnMaxButtonClick();

    virtual void UponEnter();
    virtual void UponLeave() {}



protected:
    
    //Calculates the per unit price based on galaxy difficulty
    int CalculateDifficultyCost();
    //Calculates the refill price based on the per unit price and amount missing
    int CalculateRefillCost();

    void Refill(bool fullAmount);


    std::string m_name;

    Button* maxButton = nullptr;
    Button* halfButton = nullptr;
    PictureIndex m_icon;
    Rectangle2D m_outline;
    Vector2D m_position;
    int m_baseCost;
    int m_originalMissing;

    SpaceShip* m_player = nullptr;

    Galaxy* m_galaxy = nullptr;

    std::string m_cost;
    std::string m_halfCost;
    //0.5 means that the minimum items can cost is half of their base price. Assuming 0 difficulty.
    static const float MIN_COST_MULTIPLIER;

    static const std::string MAX_BUTTON_TEXT;
    static const  std::string HALF_BUTTON_TEXT;

    //If -1 timer won't run. Set to 0.0 to start.
    double m_fundTimer = -1.0;
    //How many seconds the insufficient funds text shows up.
    static const double INSUFFICIENT_FUNDS_TIMER_DEFAULT;

    Vector2D TopCentre;
    Vector2D Centre;
    Vector2D BottomCentre;

    static const int Y_COST_PRICE_OFFSET;

    static const int OUTLINE_WIDTH;
    static const int OUTLINE_HEIGHT;
};




//SubClasses
class AmmoShopItem :
    public ShopItem
{
    using ShopItem::ShopItem;
    int GetCustomMissing() override;
    void SetCustomRefill(int amount) override;
};

class MissileShopItem :
    public ShopItem
{
    using ShopItem::ShopItem;
    int GetCustomMissing() override;
    void SetCustomRefill(int amount) override;
};

class FuelShopItem :
    public ShopItem
{
    using ShopItem::ShopItem;
    int GetCustomMissing() override;
    void SetCustomRefill(int amount) override;

    void Render() override;

    void OnMaxButtonClick() override;
public:
    void ButtonInit(int buttonWidth, int buttonHeight, const Colour& buttonColour, const Colour& buttonTextColour, const FontIndex& buttonFont, double fontScale = 1.0) override;
};

class HealthShopItem :
    public ShopItem
{
    using ShopItem::ShopItem;
    int GetCustomMissing() override;
    void SetCustomRefill(int amount) override;
};

class ArmorShopItem :
    public ShopItem
{
    using ShopItem::ShopItem;
    int GetCustomMissing() override;
    void SetCustomRefill(int amount) override;
};