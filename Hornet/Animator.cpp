#include "Animator.h"
#include "GameObject.h"
#include "ErrorLogger.h"
#include <iostream>


Animator::Animator()
{
}

Animator::~Animator()
{
}

void Animator::Initailise(const std::string filename, int spriteRes, int spriteSheetHeight, int spriteSheetWidth, int animationFPS, bool isLoop, std::vector<PictureIndex>& m_images, int& m_currentFrame, int loadOrder, int pixelMargin, bool isStar, bool frameCache)
{
    m_spriteSheetName = filename;
    m_spriteRes = spriteRes;
    m_spriteSheetWidth = spriteSheetWidth;
    m_spriteSheetHeight = spriteSheetHeight;
    m_animationFPS = animationFPS;
    m_animationCached = false;
    m_default_frameTimer = 1.0 / animationFPS;
    m_isLoop = isLoop;
    m_loopFinished = false;
    m_loadOrder = loadOrder;
    m_pixelMargin = pixelMargin;
    m_isStar = isStar;
    m_isFrameCache = frameCache;
    m_frameCount = ((m_spriteSheetWidth * m_spriteSheetHeight));
    m_iteration = 0;
    //Load the first frame where IMG_Load is used causing lag. Should keep the loading screen up until 1st frame is loaded.
    PictureIndex newFrame = HtGraphics::instance.LoadSpritesheetFrame(
        m_spriteSheetName,
        m_spriteRes,
        m_spriteSheetHeight,
        m_spriteSheetWidth,
        m_pixelMargin,
        m_iteration,
        m_loadOrder,
        m_isStar);

    m_images.push_back(newFrame);

    m_currentFrame = 0;
    m_iteration++;
}

void Animator::Animate(double frametime, std::vector<PictureIndex>& m_images, int& m_currentFrame)
{
    if (!m_loopFinished)
        m_frameTimer -= frametime;

    if (m_frameTimer <= 0)
    {
        m_frameTimer = m_default_frameTimer;

        PictureIndex newFrame = HtGraphics::instance.LoadSpritesheetFrame(
            m_spriteSheetName,
            m_spriteRes,
            m_spriteSheetHeight,
            m_spriteSheetWidth,
            m_pixelMargin,
            m_iteration,
            m_loadOrder,
            m_isStar);
        
        //If it is caching don't need to worry about deletion
        if (m_isFrameCache)
        {
            m_images.push_back(newFrame);
            m_currentFrame++;

            if (m_currentFrame >= m_frameCount)
            {
                //Set currentframe back to start to go back to this objects saved picture indexes
                m_currentFrame = 0;
                m_iteration = 0;
            }
        }
        else //If it isn't caching then 
        {
            //Destroy and delete the old frame
            if (!m_images.empty())
            {
                PictureIndex oldFrame = m_images.back();
                HtGraphics::instance.DestroyTexture(oldFrame);
                HtGraphics::instance.DeletePictureMapElement(oldFrame);
                m_images.pop_back();
            }
            //Load the new one before it gets rendered no frame buffer
            m_images.push_back(newFrame);
            //Keep just indexing into the first element of m_images becuase it won't change
            m_currentFrame = 0;

            //Restart the loop
            if (m_iteration >= m_frameCount - 1)
                m_iteration = 0;
        }
        m_iteration++;
    }
}



