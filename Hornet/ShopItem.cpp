#include "ShopItem.h"
#include "Galaxy.h"
#include "Button.h"
#include "HUDFont.h"
#include "ErrorLogger.h"
#include "Spaceship.h"

ShopItem::ShopItem(const std::string& name, const std::string& iconPath, const Vector2D& position, SpaceShip* player, Galaxy* galaxy) : m_name(name), m_icon(HtGraphics::instance.LoadPicture(iconPath)), m_position(position), m_player(player), m_galaxy(galaxy)
{
    maxButton = nullptr;
    halfButton = nullptr;
    m_baseCost = 1;
    m_originalMissing = 0;
}

ShopItem::~ShopItem()
{
    delete maxButton;
    maxButton = nullptr;
    delete halfButton;
    halfButton = nullptr;
}

void ShopItem::Initialise(int baseCost)
{
    if (baseCost < 0)
    {
        ErrorLogger::Write("ShopItem::Initialise | baseCost can't be < 0");
        return;
    }
    m_baseCost = baseCost;

    m_outline.PlaceAt(
        Vector2D(m_position.XValue - (OUTLINE_WIDTH / 2), m_position.YValue - (OUTLINE_HEIGHT / 2)),
        Vector2D(m_position.XValue + (OUTLINE_WIDTH / 2), m_position.YValue + (OUTLINE_HEIGHT / 2 )));

    TopCentre = Vector2D(m_position.XValue, m_outline.GetTopRight().YValue);
    Centre = m_position;
    BottomCentre = Vector2D(m_position.XValue, m_outline.GetBottomLeft().YValue);
}

void ShopItem::Update(double frametime)
{
    if(m_fundTimer >= 0.0)
    {
        m_fundTimer += frametime;
        if (m_fundTimer > INSUFFICIENT_FUNDS_TIMER_DEFAULT)
        {
            HtGraphics::instance.WriteTextCentered(Vector2D(0, 0), "Insufficient Funds", HtGraphics::instance.RED, HUDFont::Font, 0, 2.5);
            m_fundTimer = -1.0;
        }
    }
    maxButton->Update();
    halfButton->Update();
    if (maxButton->IsClicked())
    {
        OnMaxButtonClick();
    }
    if (halfButton->IsClicked())
    {
        OnHalfButtonClick();
    }

    if (GetCustomMissing() <= m_originalMissing / 4)
    {
        halfButton->SetText("100%");
        m_halfCost = std::to_string(CalculateRefillCost());
        m_cost = std::to_string(CalculateRefillCost());
    }
    else
    {
        m_cost = std::to_string(CalculateRefillCost());
        m_halfCost = std::to_string(CalculateRefillCost() / 2);
    }
}

void ShopItem::Render()
{

    //Buttons
    maxButton->Render();
    halfButton->Render();

    //Outline
    int outlineThickness = (maxButton->IsMouseOver() || halfButton->IsMouseOver()) ? 2 : 1;
    HtGraphics::instance.DrawRect(m_outline, HtGraphics::instance.GREY, outlineThickness);


    //Name
    HtGraphics::instance.WriteTextCentered(Vector2D(m_position.XValue, m_outline.GetTopRight().YValue - 35), m_name, HtGraphics::instance.GREY, HUDFont::Font);

    //Cost
    HtGraphics::instance.WriteTextCentered(Vector2D(BottomCentre.XValue, BottomCentre.YValue + Y_COST_PRICE_OFFSET), "Cost:", HtGraphics::instance.LIGHTBLUE, HUDFont::Font, 0, 0.75);
    HtGraphics::instance.WriteTextCentered(Vector2D(BottomCentre.XValue + 70, BottomCentre.YValue + Y_COST_PRICE_OFFSET - 50), m_cost, HtGraphics::instance.LIGHTBLUE, HUDFont::Font, 0, 0.75);
    HtGraphics::instance.WriteTextCentered(Vector2D(BottomCentre.XValue - 70, BottomCentre.YValue + Y_COST_PRICE_OFFSET - 50), m_halfCost, HtGraphics::instance.LIGHTBLUE, HUDFont::Font, 0, 0.75);
    
    if (m_name == "Fuel")
    {
        HtGraphics::instance.DrawAt(Centre + Vector2D(0, 50), m_icon, 5);
    }
    else
    {
        HtGraphics::instance.DrawAt(Centre + Vector2D(0, 50), m_icon, 6);
    }
  
}

void ShopItem::ButtonInit(int buttonWidth, int buttonHeight, const Colour& buttonColour, const Colour& buttonTextColour, const FontIndex& buttonFont, double fontScale)
{
    maxButton = new Button();
    maxButton->Initialise(BottomCentre + Vector2D(70, 50), buttonWidth, buttonHeight, MAX_BUTTON_TEXT, buttonColour, buttonTextColour, HUDFont::Font, fontScale);
    halfButton = new Button();
    halfButton->Initialise(BottomCentre + Vector2D(-70, 50), buttonWidth, buttonHeight, HALF_BUTTON_TEXT, buttonColour, buttonTextColour, HUDFont::Font, fontScale);
}

int ShopItem::CalculateDifficultyCost()
{
    return static_cast<int>(std::round(static_cast<float>(m_baseCost) * (m_galaxy->GetDifficulty() + MIN_COST_MULTIPLIER)));
}

int ShopItem::CalculateRefillCost()
{
    return CalculateDifficultyCost() * GetCustomMissing();
}

void ShopItem::Refill(bool fullAmount)
{

    int refillCost = CalculateRefillCost();
    int missingAmount = GetCustomMissing();

    if (fullAmount)
    {
        if (m_player->SpendMoney(refillCost))
        {
            SetCustomRefill(missingAmount);
        }
        else
        {
            m_fundTimer = 0.0;
            return;
        }
    }
    else
    {
        //If <= 25% is remaning to buy. Buy all remaining.
        if (GetCustomMissing() <= m_originalMissing / 4)
        {
            if (m_player->SpendMoney(refillCost))
            {
                SetCustomRefill(missingAmount);
            }
            else
            {
                m_fundTimer = 0.0;
                return;
            }
        }
        else
        {
            if (m_player->SpendMoney(refillCost / 2))
            {
                SetCustomRefill(missingAmount / 2);
            }
            else
            {
                m_fundTimer = 0.0;
                return;
            }
        }
        refillCost = CalculateRefillCost();
        missingAmount = GetCustomMissing();
    }
}

void ShopItem::OnHalfButtonClick()
{
    Refill(false);
}

void ShopItem::OnMaxButtonClick()
{
    Refill(true);
}

void ShopItem::UponEnter()
{
    m_originalMissing = GetCustomMissing();
}

int AmmoShopItem::GetCustomMissing() { return m_player->GetMissingAmmo(); }
void AmmoShopItem::SetCustomRefill(int amount) { m_player->RefillAmmo(amount); }

int MissileShopItem::GetCustomMissing() { return m_player->GetMissingMissile(); }
void MissileShopItem::SetCustomRefill(int amount) { m_player->RefillMissile(amount); }

int FuelShopItem::GetCustomMissing() { return m_player->GetMissingFuel(); }
void FuelShopItem::SetCustomRefill(int amount) { m_player->RefillFuel(amount); }

void FuelShopItem::ButtonInit(int buttonWidth, int buttonHeight, const Colour& buttonColour, const Colour& buttonTextColour, const FontIndex& buttonFont, double fontScale)
{
    maxButton = new Button();
    maxButton->Initialise(m_position + Vector2D(0,-200), buttonWidth, buttonHeight, "Warp", buttonColour, buttonTextColour, HUDFont::Font, fontScale);
    halfButton = new Button();
    halfButton->Initialise(m_position + Vector2D(0, -1000), buttonWidth, buttonHeight, "Warp", buttonColour, buttonTextColour, HUDFont::Font, fontScale);
}

void FuelShopItem::Render()
{
    maxButton->Render();

    //Outline
    int outlineThickness = (maxButton->IsMouseOver() || halfButton->IsMouseOver()) ? 2 : 1;
    HtGraphics::instance.DrawRect(m_outline, HtGraphics::instance.GREY, outlineThickness);


    //Name
    HtGraphics::instance.WriteTextCentered(Vector2D(m_position.XValue, m_outline.GetTopRight().YValue - 35), m_name, HtGraphics::instance.GREY, HUDFont::Font);

    //Cost
    HtGraphics::instance.WriteTextCentered(Vector2D(BottomCentre.XValue, BottomCentre.YValue + Y_COST_PRICE_OFFSET), "Cost:", HtGraphics::instance.LIGHTBLUE, HUDFont::Font, 0, 0.75);
    HtGraphics::instance.WriteTextCentered(Vector2D(BottomCentre.XValue, BottomCentre.YValue + Y_COST_PRICE_OFFSET - 50), m_cost, HtGraphics::instance.LIGHTBLUE, HUDFont::Font, 0, 0.75);

    if (m_name == "Fuel")
    {
        HtGraphics::instance.DrawAt(Centre + Vector2D(0, 50), m_icon, 5);
    }
    else
    {
        HtGraphics::instance.DrawAt(Centre + Vector2D(0, 50), m_icon, 6);
    }
}

void FuelShopItem::OnMaxButtonClick()
{   
    if (m_player->GetMoney() >= 10000 * m_galaxy->GetDifficulty())
    {
        Refill(true);
        m_galaxy->NextGalaxy();
    }
}

int HealthShopItem::GetCustomMissing() { return m_player->GetMissingHealth(); }
void HealthShopItem::SetCustomRefill(int amount) { m_player->RefillHealth(amount); }

int ArmorShopItem::GetCustomMissing() { return m_player->GetMissingArmor(); }
void ArmorShopItem::SetCustomRefill(int amount) { m_player->RefillArmor(amount); }


const float ShopItem::MIN_COST_MULTIPLIER = 0.5f;

const std::string ShopItem::MAX_BUTTON_TEXT = "100%";
const std::string ShopItem::HALF_BUTTON_TEXT = "50%";

const double ShopItem::INSUFFICIENT_FUNDS_TIMER_DEFAULT = 3.0;

const int ShopItem::Y_COST_PRICE_OFFSET = 170;


const int ShopItem::OUTLINE_WIDTH = 300;
const int ShopItem::OUTLINE_HEIGHT = 500;

