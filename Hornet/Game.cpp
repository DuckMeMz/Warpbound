#include "Game.h"
#include "ObjectManager.h"
#include "Spaceship.h"
#include "SpaceStation.h"
#include "ErrorLogger.h"
#include "Enemy.h"
#include "Galaxy.h"
#include "Shop.h"
#include "Cursor.h"
#include "HUD.h"
#include <cstdlib>
#include <ctime>




#include "HtCamera.h"

void Game::StartOfProgram()
{
    // This makes the operating system's mouse pointer invisible
    // It's usually best to use your own instead.
    HtMouse::instance.SetPointerVisiblity(false);
}

// Use this function to intialise your game objects and load any assets
void Game::StartOfGame()
{
    pGalaxy = new Galaxy();
    pGalaxy->Initialise();
}


// Function runs each frame.
// "frametime" is the time in seconds since the last call (delta time)
void Game::Update(double frametime)
{

    pGalaxy->Update(frametime);

    //-----------------------------------------------------------------------
    ObjectManager::instance.UpdateAll(frametime);
    ObjectManager::instance.ProcessCollisions();
    ObjectManager::instance.RenderAll();
    pGalaxy->Render();
    Cursor::instance.Render();
#ifdef _DEBUG
    ObjectManager::instance.CycleDebugObject();
    ObjectManager::instance.RenderDebug();
#endif
    ObjectManager::instance.DeleteInactiveItems();

    // Draws all graphics to the screen. Best not to remove this line.
    HtGraphics::instance.Present();
}

// Function runs when the user pauses the program.
// You may want to stop sound effects.
void Game::OnSuspend()
{

}


// This function runs when the user resumes the game from pause
// You can start sound effects if needed, but it is usually
// best to just let the game create new sounds if needed.
void Game::OnResume()
{

}

// You can use this to clear up any memory, if needed
void Game::EndOfGame()
{

    HUD::instance.Shutdown();
    delete pGalaxy;
    pGalaxy = nullptr;


    //This line automatically deletes all managed objects
    ObjectManager::instance.DeleteAllObjects();
    ObjectManager::instance.SetCurrentScene(0);
}

void Game::EndOfProgram()
{
}
