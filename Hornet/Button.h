#pragma once
#include "HtGraphics.h"

class Button
{
public:
    Button();
    ~Button();

    void Initialise(Vector2D inpPosition, int inpWidth, int inpHeight, std::string inpText, Colour inpColour, Colour inpTextColour, FontIndex inpFont = 0, double inpFontScale = 1);
    void Update();
    void Render();
    void SetPosition(Vector2D inpPosition);
    void SetText(std::string inpText);
    void SetColour(Colour inpColour);
    void SetTextColour(Colour inpTextColour);
    void SetHoverMargin(int inpMargin);

    Vector2D GetTopRight();
    Vector2D GetBottomLeft();

    bool IsMouseOver();
    bool IsClicked();
    int GetThickness();
private:
    Vector2D m_position;
    int m_width;
    int m_height;
    double m_fontScale;
    std::string m_text;
    Colour m_colour;
    Colour m_textColour;
    FontIndex m_font;

    bool m_mouseOver = false;
    bool m_clicked = false;

    int m_hoverMargin = 20;

    Rectangle2D m_buttonRect;
    int m_thickness;
};

