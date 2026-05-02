#ifndef GAME_H
#define GAME_H

#include "player.h"
#include "target.h"
#include "projectile.h"
#include "renderer.h"
#include <vector>
#include <string>
#include <cmath>
#include <random>

enum class GameState { MENU, PLAYING, PAUSED, GAMEOVER };

class Game {
public:
    Game();
    ~Game();

    bool init();
    void run();
    void shutdown();

private:
    Renderer renderer;
    Player player;
    std::vector<Target> targets;
    std::vector<Projectile> projectiles;
    GameState state = GameState::MENU;
    int score = 0;
    int highScore = 0;
    int wave = 1;
    int combo = 0;
    double lastHitTime = 0.0;
    
    std::mt19937 rng;
    int targetIdCounter = 0;
    int projectileIdCounter = 0;

    void handleInput();
    void update(double deltaTime);
    void spawnWave();
    void spawnTarget();
    void shoot();
    void checkCollisions();
    void nextWave();
    void endGame();
    void resetGame();
};

#endif // GAME_H