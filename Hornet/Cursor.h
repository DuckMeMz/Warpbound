#pragma once
#include "HtGraphics.h"
#include "HtMouse.h"
class Cursor
{
private:
    PictureIndex m_cursorImage;
    bool m_active = true;
public:

    static Cursor instance;

    void Initalise(std::string imagePath)
    {
        m_cursorImage = HtGraphics::instance.LoadPicture(imagePath);
    }

    void Render() { 
        if (m_active)
        {
            HtGraphics::instance.DrawAt(HtMouse::instance.GetPointerGamePosition(), m_cursorImage, 5);
        }
    }

    void ToggleVisiblity(bool value) { m_active = value; }
    bool GetVisibility() {return m_active; }
};

