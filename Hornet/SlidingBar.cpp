#include "SlidingBar.h"
#include "Spaceship.h"
#include "ErrorLogger.h"
#include "HUDFont.h"
#include <algorithm>

SlidingBar::SlidingBar(const Vector2D& position, const Vector2D& offset, const int value)
    : HUDElement(position, offset), m_value(value)
{ 
}

void SlidingBar::Update()
{

}

void SlidingBar::Render()
{
    HtGraphics::instance.FillRect(m_background, m_backgroundColour);
    HtGraphics::instance.FillRect(m_fill, m_fillColour);
    HtGraphics::instance.DrawRect(m_outline, m_outlineColour, 1);
}

void SlidingBar::SetByValue(int value, int maxValue)
{
    if (maxValue <= 0)
    {
        ErrorLogger::Write("Slide::SetByValue | MaxValue must be > 0 ");
        return;
    }

    m_value = (std::clamp(value, 0, maxValue) * 100) / maxValue;
    int fillWidth = (m_width * m_value) / 100;

    m_fill.PlaceAt(
        Vector2D(m_offset.XValue - (m_width / 2), m_offset.YValue - (m_height / 2)),
        Vector2D((m_offset.XValue - (m_width / 2)) + fillWidth, m_offset.YValue + (m_height / 2)));
}

void SlidingBar::SetByPercent(int value)
{
    if (value < 0)
    {
        ErrorLogger::Write("Slide::SetByValue | Value must be > 0");
        return;
    }
    if (value > 100)
    {
        ErrorLogger::Write("Slide::SetByValue | Value mut be < 100");
        return;
    }

    m_value = value;

    int fillWidth = (m_width * m_value) / 100;

    m_fill.PlaceAt(
        Vector2D(m_offset.XValue - (m_width / 2), m_offset.YValue - (m_height / 2)),
        Vector2D((m_offset.XValue - (m_width / 2)) + fillWidth, m_offset.YValue + (m_height / 2)));
}

void SlidingBar::SetColours(const Colour& fill, const Colour& outline, const Colour& background, const Colour& text)
{
    m_outlineColour = outline;
    m_fillColour = fill;
    m_backgroundColour = background;
    m_textColour = text;
}

void SlidingBar::SetDimensions(const int& width, const int& height)
{
    m_width = width;
    m_height = height;
    Vector2D bottomLeftPosition = Vector2D(m_offset.XValue - (m_width / 2), m_offset.YValue - (m_height / 2));
    Vector2D topRightPosition = Vector2D(m_offset.XValue + (m_width / 2), m_offset.YValue + (m_height / 2));
    m_outline.PlaceAt(bottomLeftPosition - Vector2D(1,1), topRightPosition + Vector2D(2, 1));
    m_fill.PlaceAt(bottomLeftPosition, topRightPosition);
    m_background.PlaceAt(bottomLeftPosition, topRightPosition);
}

//SubClasses

ResourceBar::ResourceBar(const Vector2D& position, const Vector2D offset, const int value, SpaceShip* player, const std::string& iconPath) : SlidingBar(position, offset, value), m_icon(HtGraphics::instance.LoadPicture(iconPath))
{
    if (player) m_player = player;
    else ErrorLogger::Write("ResourceBar::ResourceBar | Tried constructing with nullptr");
}

void ResourceBar::Update()
{
    SetByValue(GetCustomCurrent(), GetCustomMax());
}

void ResourceBar::Render()
{
    //Bar
    SlidingBar::Render();
    //Text Overlay
    HtGraphics::instance.WriteTextCentered(m_offset + GetCurrentOffset(), std::to_string(GetCustomCurrent()), HtGraphics::instance.WHITE, HUDFont::MonoFont, 0, 0.65);
    HtGraphics::instance.WriteTextCentered(m_offset + GetSplitterOffset(), " | ", HtGraphics::instance.WHITE, HUDFont::MonoFont, 0, .75);
    HtGraphics::instance.WriteTextCentered(m_offset + GetMaxOffset(), std::to_string(GetCustomMax()), HtGraphics::instance.WHITE, HUDFont::MonoFont, 0, .75);
    //Icon
    HtGraphics::instance.DrawAt(m_offset + GetIconOffset(), m_icon, 2.25);
}

SpaceShip* ResourceBar::m_player;

int HealthBar::GetCustomCurrent() { return m_player->GetCurrentHealth(); }
int HealthBar::GetCustomMax() { return m_player->GetMaxHealth(); }
 
Vector2D HealthBar::GetCurrentOffset() { return CURRENT_OFFSET; }
Vector2D HealthBar::GetSplitterOffset() { return SPLITTER_OFFSET; }
Vector2D HealthBar::GetMaxOffset() { return MAX_OFFSET; }
Vector2D HealthBar::GetIconOffset() { return ICON_OFFSET; }

const Vector2D HealthBar::CURRENT_OFFSET = Vector2D(-50, 0);
const Vector2D HealthBar::SPLITTER_OFFSET = Vector2D(0, 0);
const Vector2D HealthBar::MAX_OFFSET = Vector2D(55, 0);
const Vector2D HealthBar::ICON_OFFSET = Vector2D(-175, 0);

int ArmorBar::GetCustomCurrent() { return m_player->GetCurrentArmor(); }
int ArmorBar::GetCustomMax() { return m_player->GetMaxArmor(); }

Vector2D ArmorBar::GetCurrentOffset() { return CURRENT_OFFSET; }
Vector2D ArmorBar::GetSplitterOffset() { return SPLITTER_OFFSET; }
Vector2D ArmorBar::GetMaxOffset() { return MAX_OFFSET; }
Vector2D ArmorBar::GetIconOffset() { return ICON_OFFSET; }

const Vector2D ArmorBar::CURRENT_OFFSET = Vector2D(-50, 0);
const Vector2D ArmorBar::SPLITTER_OFFSET = Vector2D(0, 0);
const Vector2D ArmorBar::MAX_OFFSET = Vector2D(50, 0);
const Vector2D ArmorBar::ICON_OFFSET = Vector2D(-175 , 0);