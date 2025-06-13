#pragma once

class Vector2D;

struct HUDAnchors
{
    static Vector2D TopLeft;
    static Vector2D CentreLeft;
    static Vector2D BottomLeft;
    static Vector2D TopCentre;
    static Vector2D Centre;
    static Vector2D BottomCentre;
    static Vector2D TopRight;
    static Vector2D CentreRight;
    static Vector2D BottomRight;

    static void Initialise();

    static void DebugAnchors();

    static void PrintAnchors();
};



