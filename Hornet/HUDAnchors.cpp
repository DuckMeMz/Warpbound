#include "HUDAnchors.h"
#include "HtCamera.h"
#include "HtGraphics.h"


Vector2D HUDAnchors::TopLeft;
Vector2D HUDAnchors::CentreLeft;
Vector2D HUDAnchors::BottomLeft;
Vector2D HUDAnchors::TopCentre;
Vector2D HUDAnchors::Centre;
Vector2D HUDAnchors::BottomCentre;
Vector2D HUDAnchors::TopRight;
Vector2D HUDAnchors::CentreRight;
Vector2D HUDAnchors::BottomRight;


void HUDAnchors::Initialise()
{
    Vector2D oldCameraPos = HtCamera::instance.GetCameraCentre();
    HtCamera::instance.PlaceAt(Vector2D(0,0));
    double zoom = HtCamera::instance.GetZoom();

    const double SCREEN_WIDTH = HtCamera::instance.GetCameraArea().GetTopRight().XValue;
    const double SCREEN_HEIGHT = HtCamera::instance.GetCameraArea().GetTopRight().YValue;

    //Distance from the edge of the screen
    const double X_MARGIN = 200;
    const double Y_MARGIN = 200;

    /*
                +1000
    -1800                       +1800
                -1000
 
    */

    TopLeft = Vector2D(
        (-SCREEN_WIDTH + X_MARGIN) * zoom,
        (SCREEN_HEIGHT - Y_MARGIN) * zoom
    );
    CentreLeft = Vector2D(
        (-SCREEN_WIDTH + X_MARGIN) * zoom,
        0
    );
    BottomLeft = Vector2D(
        (-SCREEN_WIDTH + X_MARGIN) * zoom,
        (-SCREEN_HEIGHT + Y_MARGIN) * zoom
    );
    TopCentre = Vector2D(
        0,
        (SCREEN_HEIGHT - Y_MARGIN) * zoom
    );
    Centre = Vector2D(0, 0);
    BottomCentre = Vector2D(
        0,
        (-SCREEN_HEIGHT + Y_MARGIN) * zoom
    );
    TopRight = Vector2D(
        (SCREEN_WIDTH - X_MARGIN) * zoom,
        (SCREEN_HEIGHT - Y_MARGIN) * zoom
    );
    CentreRight = Vector2D(
        (SCREEN_WIDTH - X_MARGIN) * zoom,
        0
    );
    BottomRight = Vector2D(
        (SCREEN_WIDTH - X_MARGIN) * zoom,
        (-SCREEN_HEIGHT + Y_MARGIN) * zoom
    );
    HtCamera::instance.PlaceAt(oldCameraPos);
}

void HUDAnchors::PrintAnchors()
{
    std::cout << "HUD Anchors Positions:" << std::endl;
    std::cout << "TopLeft: " << TopLeft << std::endl;
    std::cout << "CentreLeft: " << CentreLeft << std::endl;
    std::cout << "BottomLeft: " << BottomLeft << std::endl;
    std::cout << "TopCentre: " << TopCentre << std::endl;
    std::cout << "Centre: " << Centre << std::endl;
    std::cout << "BottomCentre: " << BottomCentre << std::endl;
    std::cout << "TopRight: " << TopRight << std::endl;
    std::cout << "CentreRight: " << CentreRight << std::endl;
    std::cout << "BottomRight: " << BottomRight << std::endl;
}

void HUDAnchors::DebugAnchors()
{
    Circle2D topLeftCircle;
    topLeftCircle.PlaceAt(TopLeft, 25);
    HtGraphics::instance.FillCircle(topLeftCircle, HtGraphics::instance.GREEN);

    Circle2D centreLeftCircle;
    centreLeftCircle.PlaceAt(CentreLeft, 25);
    HtGraphics::instance.FillCircle(centreLeftCircle, HtGraphics::instance.GREEN);

    Circle2D bottomLeftCircle;
    bottomLeftCircle.PlaceAt(BottomLeft, 25);
    HtGraphics::instance.FillCircle(bottomLeftCircle, HtGraphics::instance.GREEN);

    Circle2D topCentreCircle;
    topCentreCircle.PlaceAt(TopCentre, 25);
    HtGraphics::instance.FillCircle(topCentreCircle, HtGraphics::instance.GREEN);

    Circle2D centreCircle;
    centreCircle.PlaceAt(Centre, 25);
    HtGraphics::instance.FillCircle(centreCircle, HtGraphics::instance.GREEN);

    Circle2D bottomCentreCircle;
    bottomCentreCircle.PlaceAt(BottomCentre, 25);
    HtGraphics::instance.FillCircle(bottomCentreCircle, HtGraphics::instance.GREEN);

    Circle2D topRightCircle;
    topRightCircle.PlaceAt(TopRight, 25);
    HtGraphics::instance.FillCircle(topRightCircle, HtGraphics::instance.GREEN);

    Circle2D centreRightCircle;
    centreRightCircle.PlaceAt(CentreRight, 25);
    HtGraphics::instance.FillCircle(centreRightCircle, HtGraphics::instance.GREEN);

    Circle2D bottomRightCircle;
    bottomRightCircle.PlaceAt(BottomRight, 25);
    HtGraphics::instance.FillCircle(bottomRightCircle, HtGraphics::instance.GREEN);
}
