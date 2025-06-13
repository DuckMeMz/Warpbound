#pragma once
#include "HUDElement.h"

class SpaceShip;


class SlidingBar : public HUDElement
{
protected:
    SlidingBar(const Vector2D& position, const Vector2D& offset, const int value);

    void SetByValue(int value, int maxValue);
    void SetByPercent(int value);

public:
    virtual void Update();
    virtual void Render();

    void SetColours(const Colour& fill, const Colour& outline, const Colour& background, const Colour& text);
    void SetDimensions(const int& m_width, const int& height);
    int GetValue() { return m_value; }
    void SetValue(int value) { m_value = value; }

private:
    int m_value;
    Rectangle2D m_fill, m_outline, m_background;
    Colour m_fillColour, m_outlineColour, m_backgroundColour, m_textColour;
    int m_width, m_height;

};

class ResourceBar : public SlidingBar
{
public:
    ResourceBar(const Vector2D& position, const Vector2D offset, const int value, SpaceShip* player, const std::string& iconPath);

    void Update() override;
    void Render() override;

    virtual int GetCustomCurrent() = 0;
    virtual int GetCustomMax() = 0;

protected:
    virtual Vector2D GetCurrentOffset() = 0;
    virtual Vector2D GetSplitterOffset() = 0;
    virtual Vector2D GetMaxOffset() = 0;
    virtual Vector2D GetIconOffset() = 0;

    static SpaceShip* m_player;
    PictureIndex m_icon;
};


class HealthBar : public ResourceBar
{
public:
    using ResourceBar::ResourceBar;
    virtual int GetCustomCurrent() override;
    virtual int GetCustomMax() override;

private:
    Vector2D GetCurrentOffset();
    Vector2D GetSplitterOffset();
    Vector2D GetMaxOffset();
    Vector2D GetIconOffset();

    static const Vector2D CURRENT_OFFSET;
    static const Vector2D SPLITTER_OFFSET;
    static const Vector2D MAX_OFFSET;
    static const Vector2D ICON_OFFSET;
};

class ArmorBar : public ResourceBar
{
public:
    using ResourceBar::ResourceBar;
    int GetCustomCurrent() override;
    int GetCustomMax() override;
private:
    Vector2D GetCurrentOffset();
    Vector2D GetSplitterOffset();
    Vector2D GetMaxOffset();
    Vector2D GetIconOffset();

    static const Vector2D CURRENT_OFFSET;
    static const Vector2D SPLITTER_OFFSET;
    static const Vector2D MAX_OFFSET;
    static const Vector2D ICON_OFFSET;
};

