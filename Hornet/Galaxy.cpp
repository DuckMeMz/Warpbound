#include "Galaxy.h"
#include <format>

//Objects
#include "Planet.h"
#include "Spaceship.h"
#include "Spacestation.h"
#include "HUD.h"
#include "Shop.h"
#include "Cursor.h"
#include "Enemy.h"

//Core
#include "ObjectManager.h"
#include "HUDAnchors.h"
#include "HUDFont.h"
#include "HtCamera.h"
#include "HtKeyboard.h"

const Vector2D Galaxy::DIFFICULTY_OFFSET = Vector2D(125,125);
const Vector2D Galaxy::WAVE_TIMER_OFFSET = Vector2D(125, 25);
const Vector2D Galaxy::SKIP_WAVE_TIMER_OFFSET = Vector2D(140, -50);
const Vector2D Galaxy::CREDITS_OFFSET = Vector2D(0, 125);

Galaxy::~Galaxy()
{
    m_player = nullptr;
    m_spaceStation = nullptr;
    m_shop = nullptr;
}

void Galaxy::Initialise()
{
    Cursor::instance.Initalise("assets/shop/crosshair.png");
    LoadLevel();
    HUD::instance.Initialise(m_player);
    HUDFont::Initialise();
    HUDAnchors::Initialise();
}

void Galaxy::Update(double frametime)
{
    HandleCheatKeys();
    //Muting Background Music
    if (HtKeyboard::instance.NewKeyPressed(SDL_SCANCODE_M))
    {
        if (m_musicPlaying)
        {
            HtAudio::instance.PauseMusic();
            m_musicPlaying = false;
        }
        else
        {
            HtAudio::instance.ResumeMusic();
            m_musicPlaying = true;
        }
    }

    HUD::instance.Update(frametime);

    //Checks for any enemies remaining
    if (!ObjectManager::instance.FindClosestObject(Vector2D(0, 0), ObjectType::ENEMY))
    {
        if (!m_waveCooldownActive)
        {
            m_waveCooldownActive = true;
            m_waveCooldownTimer = 30.0;
            m_spaceStation->ToggleDocking(true);
            ChangeBackgroundMusic(BackgroundMusic::CALM);
        }        
    }

    if (m_waveCooldownActive)
    {
        m_waveCooldownTimer -= frametime;

        if (m_waveCooldownTimer <= 0.0 && m_player->GetCurrentHealth() > 0 && !m_completeFlag)
        {
            if (!m_shop) return;
            m_shop->ForceLeave();
            m_waveCooldownActive = false;
            m_spaceStation->ToggleDocking(false);
            if(!m_endingLoaded) SpawnWave();
            ChangeBackgroundMusic(BackgroundMusic::COMBAT);
        }
    }

    if (m_completeFlag)
    {
        if (!m_endingLoaded)
        {
            HtCamera::instance.PlaceAt(Vector2D(0, 0));
            HtAudio::instance.StopAllChannels();
            ObjectManager::instance.DeactivateScene(10);
            ObjectManager::instance.SetCurrentScene(1);
            m_waveCooldownActive = false;
            m_displayGalaxyStats = false;
            DespawnPlanets();
            Planet* wormHole = new Planet();
            wormHole->Initailise("assets/planets/galaxy-1.png", Vector2D(0, 250), 0, 10, 0, 1);
            wormHole->InitailiseAnimator(64, 60, 30, true, 30, 5, 0, false, false);
            ObjectManager::instance.AddItem(wormHole);
            m_endingLoaded = true;
        }
        HtGraphics::instance.WriteTextCentered(HUDAnchors::Centre, "Congratulations! You've rediscovered the worm hole to the forgotten city!", HtGraphics::instance.CYAN, HUDFont::Font, 0, 1);
    }

    if (m_player->GetCurrentHealth() == 0)
    {
        if (!m_endingLoaded)
        {
            HtCamera::instance.PlaceAt(Vector2D(0, 0));
            HtAudio::instance.StopAllChannels();
            ObjectManager::instance.DeactivateScene(10);
            ObjectManager::instance.SetCurrentScene(2);
            m_waveCooldownActive = false;
            m_displayGalaxyStats = false;
            DespawnPlanets();
            Planet* blackHole = new Planet();
            blackHole->Initailise("assets/planets/black-hole.png", Vector2D(0, 250), 0, 10, 0, 1);
            blackHole->InitailiseAnimator(64, 60, 30, true, 30, 5, 0, true, false);
            ObjectManager::instance.AddItem(blackHole);
            m_endingLoaded = true;
        }
        HtGraphics::instance.WriteTextCentered(HUDAnchors::Centre + Vector2D(0, -250), "Failiure! You've become infected!", HtGraphics::instance.RED, HUDFont::Font, 0, 1);
    }
}

void Galaxy::Render()
{
    HtCamera::instance.UseCamera(false);
    
    if (m_displayGalaxyStats)
    {
        //Difficulty
        std::string difficultyStr = std::format("{:.2f}", GetDifficulty());
        HtGraphics::instance.WriteTextCentered(HUDAnchors::TopLeft + DIFFICULTY_OFFSET + Vector2D(-120, 0), "Difficulty: ", HtGraphics::instance.WHITE, HUDFont::Font, 0, 1);
        HtGraphics::instance.WriteTextCentered(HUDAnchors::TopLeft + DIFFICULTY_OFFSET + Vector2D(120, 0), difficultyStr, HtGraphics::instance.WHITE, HUDFont::MonoFont, 0, 1);

        //Wave Timer
        std::string waveTimerStr = std::format("{:.2f}", GetWaveTimer());
        if (waveTimerStr == "-0.00" || waveTimerStr == "-0.01") waveTimerStr = "0.00";
        HtGraphics::instance.WriteTextCentered(HUDAnchors::TopLeft + WAVE_TIMER_OFFSET + Vector2D(-120, 0), "Next Wave: ", HtGraphics::instance.WHITE, HUDFont::Font, 0, 1);
        HtGraphics::instance.WriteTextCentered(HUDAnchors::TopLeft + WAVE_TIMER_OFFSET + Vector2D(120, 0), waveTimerStr, HtGraphics::instance.WHITE, HUDFont::MonoFont, 0, 1);

        //Skip Wave Timer
        if (m_waveCooldownActive)
        {
            HtGraphics::instance.WriteTextCentered(HUDAnchors::TopLeft + SKIP_WAVE_TIMER_OFFSET, "Press U to skip wave timer", HtGraphics::instance.RED, HUDFont::Font, 0, .75);
        }

        //Credits
        HtGraphics::instance.WriteTextCentered(HUDAnchors::TopCentre + CREDITS_OFFSET + Vector2D(-120, 0), "Credits: ", HtGraphics::instance.WHITE, HUDFont::Font, 0, 1);
        HtGraphics::instance.WriteTextCentered(HUDAnchors::TopCentre + CREDITS_OFFSET + Vector2D(120, 0), std::to_string(m_player->GetMoney()), HtGraphics::instance.WHITE, HUDFont::Font, 0, 1);
    }
    

    HtCamera::instance.UseCamera(true);

    HUD::instance.Render();
    Cursor::instance.Render();
}

void Galaxy::HandleCheatKeys()
{
    if (HtKeyboard::instance.NewKeyPressed(SDL_SCANCODE_LEFTBRACKET))
    {
        //Player can't be dead. Stops overlapping endings
        if (m_player->GetCurrentHealth() > 0) m_completeFlag = true;
    }
    if (HtKeyboard::instance.NewKeyPressed(SDL_SCANCODE_RIGHTBRACKET))
    {
        //Can't have completed the game. Stops overlapping endings.
        if (!m_completeFlag) m_player->SetMaxHealth(0);
    }
    if (HtKeyboard::instance.NewKeyPressed(SDL_SCANCODE_U))
    {
        //Can't be having one of the endings
        if (m_player->GetCurrentHealth() > 0 && !m_completeFlag)
        {
            SkipWaveCountdown();
            SkipWaveCountdown();
            ChangeBackgroundMusic(BackgroundMusic::COMBAT);
        }
    }
    if (HtKeyboard::instance.NewKeyPressed(SDL_SCANCODE_SEMICOLON))
    {
        std::vector<GameObject*> enemies = ObjectManager::instance.GetAllObjectsOfType(ObjectType::ENEMY);
        for (GameObject* enemyGameObject : enemies)
        {
            Enemy* enemy = dynamic_cast<Enemy*>(enemyGameObject);
            if (!enemy) continue;
            enemy->Damage(999999);
        }
    }
    if (HtKeyboard::instance.NewKeyPressed(SDL_SCANCODE_APOSTROPHE))
    {
        m_player->AddMoney(1000);
    }
}

void Galaxy::ChangeBackgroundMusic(BackgroundMusic selectedMusic)
{
    if (selectedMusic == BackgroundMusic::NONE && HtAudio::instance.IsMusicPlaying()) HtAudio::instance.StopMusic();
    if (selectedMusic != m_currentBackgroundMusic)
    {
        HtAudio::instance.PauseMusic();
        HtAudio::instance.LoadMusic(m_backgroundMusicString[selectedMusic]);
        HtAudio::instance.SetMusicVolume(0.25);
        HtAudio::instance.PlayMusic();
        m_currentBackgroundMusic = selectedMusic;
        if (!m_musicPlaying) HtAudio::instance.PauseMusic();
    }
}

void Galaxy::LoadLevel()
{
    SpawnPlayer();
    SpawnPlanets();
    LoadBackground();
    SpawnSpaceStation();
    SpawnShop();
}

void Galaxy::NextGalaxy()
{
    if (m_difficulty == 1.5)
    {
        m_galaxyNum++;
        LoadBackground();
        m_completeFlag = true;
    }
    m_difficulty += 0.25;
    m_galaxyNum++;
    LoadBackground();
    m_player->SetMaxFuel(m_player->GetCurrentFuel() + 1);
}

void Galaxy::SpawnWave()
{
    Rectangle2D cameraArea = HtCamera::instance.GetCameraArea();

    for (int i = 0; i < CalculateEnemiesForWave(); ++i)
    {
        double xPos = cameraArea.GetBottomLeft().XValue + (std::rand() % 3600);
        double yPos = cameraArea.GetBottomLeft().YValue + (std::rand( ) % 2000);

        SpawnEnemyAt(Vector2D(xPos, yPos));
    }

    m_waveCount++;
}

void Galaxy::SkipWaveCountdown()
{
    if (m_waveCooldownActive)
    {
        m_shop->ForceLeave();
        m_waveCooldownTimer = 0.1;
    }
}

void Galaxy::SpawnEnemyAt(const Vector2D& position)
{
    Enemy* pEnemy = new Enemy(position, CalculateEnemyHealth(), m_player);
    if (pEnemy == nullptr) return;
    pEnemy->SetCollidable();
    pEnemy->SetDrawDepth(5);
    ObjectManager::instance.AddItem(pEnemy); 
};

int Galaxy::CalculateEnemiesForWave()
{
    return static_cast<int>((8 * m_difficulty + m_waveCount * 2 ));
}

int Galaxy::CalculateEnemyHealth()
{
    return static_cast<int>(ENEMY_BASE_HEALTH * GetDifficulty());
}

void Galaxy::SpawnPlayer()
{
    SpaceShip* pPlayer = new SpaceShip(this);
    if (pPlayer == nullptr) return;
    pPlayer->Initialise();
    ObjectManager::instance.AddItem(pPlayer);
    m_player = pPlayer;
}

void Galaxy::SpawnEnemies()
{

}

void Galaxy::LoadBackground()
{
    switch (m_galaxyNum)
    {
    case 1: 
        HtGraphics::instance.SetBackgroundTexture(HtGraphics::instance.LoadPicture("assets/backgrounds/galaxy1.png"));
        break;
    case 2:
        HtGraphics::instance.SetBackgroundTexture(HtGraphics::instance.LoadPicture("assets/backgrounds/galaxy2.png"));
        break;
    case 3:
        HtGraphics::instance.SetBackgroundTexture(HtGraphics::instance.LoadPicture("assets/backgrounds/galaxy3.png"));
        break;
    case 4:
        HtGraphics::instance.SetBackgroundTexture(HtGraphics::instance.LoadPicture("assets/backgrounds/galaxy4.png"));
        break;
    case 5:
        HtGraphics::instance.SetBackgroundTexture(HtGraphics::instance.LoadPicture("assets/backgrounds/galaxy5.png"));
        break;
    default:
        HtGraphics::instance.SetBackgroundTexture(HtGraphics::instance.LoadPicture("assets/backgrounds/galaxy1.png"));
        break;
    }
   
}

void Galaxy::SpawnPlanets()
{
    Planet* planet1 = new Planet();
    if (planet1 == nullptr) return;
    planet1->Initailise("assets/planets/planet-1.png", Vector2D(1000,630), 0, 3, 0, -4);
    planet1->InitailiseAnimator(64, 60, 30, true, 30, 0, 0, false, false);
    ObjectManager::instance.AddItem(planet1);

    Planet* planet2 = new Planet();
    if (planet2 == nullptr) return;
    planet2->Initailise("assets/planets/planet-2.png", Vector2D(-500, 630), 0, 6, 0, -2);
    planet2->InitailiseAnimator(64, 60, 30, true, 30, 1, 0, false, false);
    ObjectManager::instance.AddItem(planet2);

    Planet* planet3 = new Planet();
    if (planet3 == nullptr) return;
    planet3->Initailise("assets/planets/planet-3.png", Vector2D(-800, -600), 0, 3, 0, -5);
    planet3->InitailiseAnimator(64, 60, 30, true, 30, 2, 0, false, false);
    ObjectManager::instance.AddItem(planet3);

    Planet* planet4 = new Planet();
    if (planet4 == nullptr) return;
    planet4->Initailise("assets/planets/planet-4.png", Vector2D(-1000, 880), 0, 2, 0, -5);
    planet4->InitailiseAnimator(64, 60, 30, true, 30, 3, 0, false, false);
    ObjectManager::instance.AddItem(planet4);

    Planet* star1 = new Planet();
    if (star1 == nullptr) return;
    star1->Initailise("assets/planets/star-1.png", Vector2D(-300, 500), 0, 6, 0, -5);
    star1->InitailiseAnimator(64, 60, 30, true, 30, 4, 0, true, false);
    ObjectManager::instance.AddItem(star1);
}

void Galaxy::DespawnPlanets()
{
    ObjectManager::instance.DeactivateType(ObjectType::PLANET);
}

void Galaxy::SpawnShop()
{
    if (!m_player)
    {
        ErrorLogger::Write("Galaxy::SpawnShop | m_player nullptr");
        return;
    }
    ObjectManager::instance.DeactivateType(ObjectType::SHOP);
    Shop* pShop = new Shop();
    if (pShop == nullptr) return;
    pShop->Initialise(m_player, this);
    ObjectManager::instance.AddItem(pShop);
    pShop->SetSceneNumber(10);
    m_shop = pShop;
}

void Galaxy::SpawnSpaceStation()
{
    if (!m_player)
    {
        ErrorLogger::Write("Galaxy::SpawnSpaceStation | m_player nullptr");
        return;
    }
    ObjectManager::instance.DeactivateType(ObjectType::SPACESTATION);
    SpaceStation* pSpaceStation = new SpaceStation(Vector2D(0, 0), m_player, this);
    if (pSpaceStation == nullptr) return;
    pSpaceStation->Initialise(1);
    ObjectManager::instance.AddItem(pSpaceStation);
    m_spaceStation = pSpaceStation;
}



