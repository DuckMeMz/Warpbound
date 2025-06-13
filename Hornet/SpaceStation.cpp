#include "SpaceStation.h"
#include "ErrorLogger.h"
#include "ObjectManager.h"
#include "Spaceship.h"
#include "HUDFont.h"
#include "HUDAnchors.h"
#include "HtKeyboard.h"
#include "HtCamera.h"

SpaceStation::SpaceStation(const Vector2D& position, SpaceShip* player, Galaxy* galaxy) : GameObject(ObjectType::SPACESTATION), m_position(position), pPlayer(player)
{
}

SpaceStation::~SpaceStation()
{
}

void SpaceStation::Initialise(int spaceStationNum, int inpScale)
{ 
    m_scale = inpScale;
    m_dock.PlaceAt(m_position, m_dockingRadius);
    switch (spaceStationNum)
    {
    case 1:
        m_pictureIndex = HtGraphics::instance.LoadPicture("assets/spacestations/spacestation1.png");
        break;
    case 2:
        m_pictureIndex = HtGraphics::instance.LoadPicture("assets/spacestations/spacestation2.png");
        break;
    case 3:
        m_pictureIndex = HtGraphics::instance.LoadPicture("assets/spacestations/spacestation3.png");
        break;
    case 4:
        m_pictureIndex = HtGraphics::instance.LoadPicture("assets/spacestations/spacestation4.png");
        break;
    case 5:
        m_pictureIndex = HtGraphics::instance.LoadPicture("assets/spacestations/spacestation5.png");
        break;
    default:
        ErrorLogger::Write("Spacestation: Initialise() - Tried loading an invalid space station number. Loading Spacestation 1....  | Range 1 - 5. Invalid Num: " + spaceStationNum);
        break;
    }
}

void SpaceStation::Update(double frametime)
{
    if (m_dockingAllowed && CheckDockCollision())
    {
        ShowConfirmation();
    }
    else
    {
        HideConfirmation();
    }
   
    if (m_withinRadius && HtKeyboard::instance.NewKeyPressed(SDL_SCANCODE_E) && m_dockingAllowed)
    {

        EnterSpacestation();
    }

    Render();
}

void SpaceStation::Render()
{
    if (m_dockingAllowed)
    {
        HtGraphics::instance.DrawCircle(m_dock, HtGraphics::instance.GREEN);
    }
   
    HtGraphics::instance.DrawAt(m_position, m_pictureIndex, m_scale);
}

void SpaceStation::EnterSpacestation()
{
    ObjectManager::instance.SetCurrentScene(10);
}

void SpaceStation::ShowConfirmation()
{
    HtCamera::instance.UseCamera(false);
    HtGraphics::instance.WriteTextCentered(HUDAnchors::TopCentre, "Press  E  to enter space station", HtGraphics::instance.CYAN, HUDFont::Font, 0, 1);
    HtCamera::instance.UseCamera(true);
    m_withinRadius = true;
}

void SpaceStation::HideConfirmation()
{
    m_withinRadius = false;
}

void SpaceStation::ToggleDocking(bool toggle)
{
    m_dockingAllowed = toggle;
}

bool SpaceStation::CheckDockCollision()
{
    if (pPlayer->GetCollisionShape().Intersects(m_dock))
    {
        return true;
    }
    return false; 
}