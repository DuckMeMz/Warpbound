#pragma once
#include <string>
#include "HtGraphics.h" 

class Animator
{
public:

    
    Animator();

    ~Animator();

    void Initailise(const std::string filename, int spriteRes, int spriteSheetHeight, int spriteSheetWidth, int animationFPS, bool isLoop, std::vector<PictureIndex>& m_images, int& m_imageNumber, int loadOrder, int pixelMargin, bool isStar, bool frameCache);

    void Animate(double frametime, std::vector<PictureIndex>& m_images, int& m_currentFrame);

private:
        double m_default_frameTimer;
        double m_frameTimer;
        std::string m_spriteSheetName;
        int m_spriteRes;
        int m_spriteSheetWidth;
        int m_spriteSheetHeight;
        int m_frameCount;
        int m_animationFPS;
        bool m_animationCached;
        bool m_isLoop;
        bool m_loopFinished;
        int m_loadOrder;
        int m_pixelMargin;
        bool m_isStar;
        bool m_isFrameCache;
        int m_iteration;
};

