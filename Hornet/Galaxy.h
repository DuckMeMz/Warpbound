#pragma once
#include <unordered_map>
#include <string>

class SpaceShip;
class Vector2D;
class SpaceStation;
class Planet;
class Shop;

enum class BackgroundMusic {
    NONE, CALM, SHOP, COMBAT
};

class Galaxy
{
public:
    Galaxy() {}
    ~Galaxy();
    void Initialise();
    void Update(double frametime);
    void Render();

    void HandleCheatKeys();

    void ChangeBackgroundMusic(BackgroundMusic selectedMusic);

    void LoadLevel();

    void SpawnPlayer();
    void SpawnEnemies();
    void LoadBackground();
    void SpawnPlanets();
    void DespawnPlanets();
    void SpawnShop();
    void SpawnSpaceStation();
    void NextGalaxy();

    void SpawnWave();

    float GetDifficulty() { return m_difficulty; }
    double GetWaveTimer() { return m_waveCooldownTimer; }

    void SkipWaveCountdown();

    //Enemy Spawning
    void SpawnEnemyAt(const Vector2D& position);
    int CalculateEnemiesForWave();
    int CalculateEnemyHealth();
    

public:
    int m_galaxyNum = 1;
    bool m_completeFlag = false;
    bool m_endingLoaded = false;
    static const int MAX_MONEY = 750;
    static const int MIN_MONEY = 150;

    int m_waveCount = 0;
    double m_waveCooldownTimer = -1.0;
    bool m_waveCooldownActive = false;

    //Galaxy Stats
    bool m_displayGalaxyStats = true;
    static const Vector2D DIFFICULTY_OFFSET;
    static const Vector2D WAVE_TIMER_OFFSET;
    static const Vector2D SKIP_WAVE_TIMER_OFFSET;
    static const Vector2D CREDITS_OFFSET;

    static const int ENEMY_BASE_HEALTH = 100;

    //Background Music
    bool m_musicPlaying = true;
    BackgroundMusic m_currentBackgroundMusic = BackgroundMusic::NONE;
    std::unordered_map<BackgroundMusic, std::string> m_backgroundMusicString = {
        {BackgroundMusic::CALM, "assets/sfx/music/calm.wav"},
        {BackgroundMusic::SHOP, "assets/sfx/music/calm.wav"},
        {BackgroundMusic::COMBAT, "assets/sfx/music/combat.wav"}
    };
    double m_musicVolume = 1.0;
 

private:
    SpaceShip* m_player = nullptr;
    SpaceStation* m_spaceStation = nullptr;
    Shop* m_shop = nullptr;
    float m_difficulty = 0.5f;
    //1-5 Changes image of spacestation.
    int spaceStationNum = 1;
};