#pragma once
#include "GameObject.h"

class SpaceShip;
class Galaxy;

class SpaceStation :
    public GameObject
{
public:

    SpaceStation(const Vector2D& position, SpaceShip* player, Galaxy* galaxy);
    ~SpaceStation();

    void Initialise(int spaceStationNum, int inpScale = 1);

    void Update(double frametime) override;
  
    void Render();

    void EnterSpacestation();

    void ShowConfirmation();
    void HideConfirmation();

    void ToggleDocking(bool toggle);

    bool CheckDockCollision();

private:

    //Entry location:
    Circle2D m_dock;
    double m_dockingRadius = 300;
    bool m_withinRadius = false;
    bool m_dockingAllowed = true;

    //Render
    PictureIndex m_pictureIndex;

    //Functionality
    SpaceShip* pPlayer;
    Galaxy* m_galaxy;

    Vector2D m_position;
};

