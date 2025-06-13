#include "Planet.h"
#include "ErrorLogger.h"
#include "Event.h"
#include <iostream>

Planet::Planet() : GameObject(ObjectType::PLANET)
{
}

Planet::~Planet()
{
    delete pAnimator;
    pAnimator = nullptr;
}

void Planet::Initailise(const std::string filename, Vector2D inpPos, double inpAngle, double inpScale, double inpTransparency, int inpDrawDepth)
{
    SetDrawDepth(inpDrawDepth);
    m_imageFile = filename;
    m_position = inpPos;
    m_angle = inpAngle;
    m_scale = inpScale;
    m_transparency = inpTransparency;
}

void Planet::Update(double frametime)
{
    //Animaton code
    if (m_isAnimated)
    {
        pAnimator->Animate(frametime, m_images, m_imageNumber);
    }
}

void Planet::InitailiseAnimator(int spriteRes, int frameheight, int framewidth, bool isLoop, int animationFPS, int loadOrder, int pixelMargin, bool isStar, bool frameCache)
{
    m_isAnimated = true;
    pAnimator = new Animator();
    pAnimator->Initailise(m_imageFile, spriteRes, frameheight, framewidth, animationFPS, isLoop, m_images, m_imageNumber, loadOrder, pixelMargin, isStar, frameCache);
}
