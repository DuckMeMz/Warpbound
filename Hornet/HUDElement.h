#pragma once
#include "HtGraphics.h"
#include "ErrorLogger.h"

class HUDElement
{
protected:
    HUDElement(const Vector2D& position, const Vector2D& offset) : m_offset(position + offset) {}
    
public:

    virtual ~HUDElement() {};
    virtual void Update() = 0;
    virtual void Render() = 0; 

    void SetOffset(Vector2D offset) { m_offset = offset; }
    Vector2D GetOffset() { return m_offset; }

protected:
    Vector2D m_offset;
};