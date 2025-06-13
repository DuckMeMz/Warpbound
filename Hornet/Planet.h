#pragma once
#include "GameObject.h"
#include "Animator.h"

class Animator;

class Planet :
    public GameObject
{ 
public:
    Planet();

    ~Planet();

    void Initailise(const std::string filename, Vector2D inpPos, double inpAngle, double inpScale,  double inpTransparency, int inpDrawDepth);

    void Update(double frametime) override;

    void InitailiseAnimator(int spriteRes, int frameheight, int framewidth, bool isLoop, int animationFPS, int loadOrder, int pixelMargin, bool isStar, bool frameCache);


      


private:

    Animator* pAnimator;

    std::string m_imageFile;

};

