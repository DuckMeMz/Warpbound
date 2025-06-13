#pragma once
#include "HUDElement.h"
#include "HUDFont.h"

template <typename T>
class Counter :
    public HUDElement
{
    static_assert(std::is_same_v<T, int> || std::is_same_v<T, float> || std::is_same_v<T, double>, "Counter types are limited to: Int, Float, Double");

public:
    Counter(const Vector2D& position, const Vector2D& offset, const T value);

    void Initialise();
    void Update() override;
    void Render() override;

    void SetValue(T value);
    T GetValue();

    Counter<T>& operator++();
    Counter<T> operator++(int);
    Counter<T>& operator--();
    Counter<T> operator--(int);

private:
    T m_value;
    FontIndex m_font;
    Colour m_colour;
    int m_scale;
};

template<typename T>
void Counter<T>::Update()
{

}
 
template<typename T>
void Counter<T>::Render()
{

}

template<typename T>
inline Counter<T>::Counter(const Vector2D& position, const Vector2D& offset, const T value) : HUDElement(position, offset), m_value(value)
{

}

template<typename T>
inline void Counter<T>::SetValue(T value)
{
    m_value = value;
}


template<typename T>
inline T Counter<T>::GetValue()
{
    return m_value;
}

template<typename T>
Counter<T>& Counter<T>::operator++()
{
    ++m_value;
    return *this;
};

template<typename T>
Counter<T> Counter<T>::operator++(int)
{
    Counter<T> temp = *this;
    operator++();
    return temp;

}

template<typename T>
Counter<T>& Counter<T>::operator--()
{
    --m_value;
    return *this;
}

template<typename T>
Counter<T> Counter<T>::operator--(int)
{
    Counter<T> temp = *this;
    operator--();
    return temp;
}

class SpaceShip; 

template<typename T>
class ResourceCounter : public HUDElement
{
    static_assert(std::is_same_v<T, int> || std::is_same_v<T, float> || std::is_same_v<T, double>, "Counter types are limited to: Int, Float, Double");

public:

    ResourceCounter(const Vector2D& position, const Vector2D& offset, T current, T max, SpaceShip* player, const std::string& iconPath) :
        HUDElement(position, offset),
        m_current(position, offset, current),
        m_max(position, offset, max), m_icon(HtGraphics::instance.LoadPicture(iconPath))
    { 
        if (player) m_player = player;
        else ErrorLogger::Write("ResourceCounter::ResourceCounter | Tried constructing with nullptr");
    }
    
    void Update() override;
    void Render() override;

    Colour GetColour() { return m_colour; }
    void SetColour(Colour colour) { m_colour = colour; }

    virtual T GetCustomCurrent() = 0;
    virtual T GetCustomMax() = 0;

    void SetCurrent(T value) { m_current.SetValue(value); }
    void SetMax(T value) { m_max.SetValue(value); }

    T GetCurrent() { return m_current.GetValue(); }
    T GetMax() { return m_max.GetValue(); }

protected:
    virtual Vector2D GetCurrentOffset() = 0;
    virtual Vector2D GetSplitterOffset() = 0;
    virtual Vector2D GetMaxOffset() = 0;
    virtual Vector2D GetIconOffset() = 0;

private:
    Counter<T> m_current;
    Counter<T> m_max;
    Colour m_colour;
    PictureIndex m_icon;
    Rectangle2D m_outline;

protected:
    static SpaceShip* m_player;
};

template<typename T>
void ResourceCounter<T>::Update()
{
    SetCurrent(GetCustomCurrent());
    SetMax(GetCustomMax());
}
 
template<typename T>
void ResourceCounter<T>::Render()
{
    //Current
    m_outline = Rectangle2D{
     Vector2D(m_offset.XValue - 135, m_offset.YValue + 30),
     Vector2D(m_offset.XValue + 135, m_offset.YValue - 30)
    };

    HtGraphics::instance.DrawRect(m_outline, HtGraphics::instance.CYAN, 1);

    HtGraphics::instance.WriteTextCentered(m_offset + GetCurrentOffset(), std::to_string(GetCustomCurrent()), m_colour, HUDFont::MonoFont, 0, 1);
    //Splitter
    HtGraphics::instance.WriteTextCentered(m_offset + GetSplitterOffset(), "| ", m_colour, HUDFont::MonoFont, 0, 1);
    //Max
    HtGraphics::instance.WriteTextCentered(m_offset + GetMaxOffset(), std::to_string(GetCustomMax()), m_colour, HUDFont::MonoFont, 0, 1);
    HtGraphics::instance.DrawAt(m_offset + GetIconOffset(), m_icon, 2);

    
    

}

class AmmoCounter : public ResourceCounter<int>
{
public:
    using ResourceCounter<int>::ResourceCounter;
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


class MissileCounter : public ResourceCounter<int>
{
public:
    using ResourceCounter<int>::ResourceCounter;
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