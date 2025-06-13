#include "Button.h"
#include "Shapes.h"
#include "HtMouse.h"

Button::Button()
{
    m_position = Vector2D(0, 0);
    m_width = 0;
    m_height = 0;
    m_text = "";
    m_colour = Colour(HtGraphics::instance.GREEN);
    m_colour = Colour(HtGraphics::instance.CYAN);
    m_font = 0;
    m_thickness = 1;
    m_fontScale = 1;
}

Button::~Button()
{
}

void Button::Initialise(Vector2D inpPosition, int inpWidth, int inpHeight, std::string inpText, Colour inpColour, Colour inpTextColour,FontIndex inpFont, double inpFontScale)
{
    m_position = inpPosition;
    m_width = inpWidth;
    m_height = inpHeight;
    m_text = inpText;
    m_colour = inpColour;
    m_textColour = inpTextColour;
    m_font = inpFont;
    m_fontScale = inpFontScale;

    m_buttonRect.PlaceAt(
        Vector2D(m_position.XValue - m_width, m_position.YValue - m_height),
        Vector2D(m_position.XValue + m_width, m_position.YValue + m_height)
    );
}

void Button::Update()
{
    m_thickness = IsMouseOver() ? 2 : 1;
}

void Button::Render()
{
    HtGraphics::instance.DrawRect(m_buttonRect, m_colour, m_thickness);
    HtGraphics::instance.WriteTextCentered(m_position, m_text, m_textColour, m_font, 0.0, m_fontScale);
}

void Button::SetPosition(Vector2D inpPosition)
{
    m_position = inpPosition;
}

void Button::SetText(std::string inpText)
{
    m_text = inpText;
}

void Button::SetColour(Colour inpColour)
{
    m_colour = inpColour;
}

void Button::SetTextColour(Colour inpTextColour)
{
    m_textColour = inpTextColour;
}

void Button::SetHoverMargin(int inpMargin)
{
    m_hoverMargin = inpMargin;
}

Vector2D Button::GetTopRight()
{
    return Vector2D(m_position.XValue + (m_width/2), m_position.YValue + (m_height/2));
}

Vector2D Button::GetBottomLeft()
{
    return Vector2D(m_position.XValue - (m_width / 2), m_position.YValue - (m_height / 2));
}

bool Button::IsMouseOver()
{
    Vector2D mousePos = HtMouse::instance.GetPointerGamePosition();
    Vector2D bottomLeft = m_buttonRect.GetBottomLeft();
    Vector2D topRight = m_buttonRect.GetTopRight();
    return (mousePos.XValue <= topRight.XValue + m_hoverMargin && mousePos.XValue >= bottomLeft.XValue - m_hoverMargin) && (mousePos.YValue <= topRight.YValue + m_hoverMargin && mousePos.YValue >= bottomLeft.YValue - m_hoverMargin);
}

bool Button::IsClicked()
{
    return IsMouseOver() && HtMouse::instance.IsNewMouseDown(HtMouseButton::LEFT);
}

int Button::GetThickness()
{
    return m_thickness;
}
