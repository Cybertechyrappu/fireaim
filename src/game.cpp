#include "game.h"
#include "constants.h"
#include <cmath>
#include <algorithm>

Game::Game() : rng(std::random_device{}()) {}

Game::~Game() {}

bool Game::init() {
    return renderer.init(GameConstants::SCREEN_WIDTH, GameConstants::SCREEN_HEIGHT);
}

void Game::run() {
    bool running = true;
    Uint32 lastTime = SDL_GetTicks();

    while (running) {
        Uint32 currentTime = SDL_GetTicks();
        double deltaTime = (currentTime - lastTime) / 1000.0;
        lastTime = currentTime;

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
            else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    if (state == GameState::PLAYING) state = GameState::PAUSED;
                    else if (state == GameState::PAUSED) state = GameState::PLAYING;
                    else if (state == GameState::GAMEOVER || state == GameState::MENU) running = false;
                }
                else if (event.key.keysym.sym == SDLK_SPACE) {
                    if (state == GameState::MENU || state == GameState::GAMEOVER) {
                        resetGame();
                    }
                }
            }
            else if (event.type == SDL_MOUSEBUTTONDOWN && state == GameState::PLAYING) {
                shoot();
            }
            else if (event.type == SDL_MOUSEMOTION && state == GameState::PLAYING) {
                double dx = event.motion.xrel * 0.005;
                double dy = event.motion.yrel * 0.005;
                player.updateLook(dx, dy);
            }
        }

        if (state == GameState::PLAYING) {
            update(deltaTime);
        }

        renderer.beginFrame();
        if (state == GameState::PLAYING || state == GameState::PAUSED) {
            renderer.renderScene(player, targets, projectiles, score, wave, combo);
            renderer.renderCrosshair();
            renderer.renderHUD(score, wave, combo);
        }
        renderer.endFrame();
    }
}

void Game::update(double deltaTime) {
    double currentTime = SDL_GetTicks() / 1000.0;

    for (auto& target : targets) {
        target.update(deltaTime, currentTime);
    }

    for (auto& proj : projectiles) {
        proj.update(deltaTime);
    }

    projectiles.erase(
        std::remove_if(projectiles.begin(), projectiles.end(), 
            [currentTime](const Projectile& p) { return p.isExpired(currentTime); }),
        projectiles.end()
    );

    checkCollisions();

    if (targets.empty() && state == GameState::PLAYING) {
        nextWave();
    }
}

void Game::shoot() {
    double currentTime = SDL_GetTicks() / 1000.0;
    if (!player.canShoot(currentTime)) return;

    player.lastFireTime = currentTime;

    double dx, dy, dz;
    player.getLookDirection(dx, dy, dz);

    Projectile proj(
        "proj_" + std::to_string(projectileIdCounter++),
        player.x, player.y, player.z,
        dx, dy, dz, currentTime
    );
    projectiles.push_back(proj);
}

void Game::spawnWave() {
    int count = GameConstants::TARGETS_PER_WAVE + (wave - 1) * 2;
    for (int i = 0; i < count; ++i) {
        spawnTarget();
    }
}

void Game::spawnTarget() {
    std::uniform_real_distribution<double> angleDist(0.0, 2.0 * M_PI);
    std::uniform_real_distribution<double> distDist(GameConstants::TARGET_MIN_DISTANCE, GameConstants::TARGET_MAX_DISTANCE);
    std::uniform_real_distribution<double> heightDist(1.0, 4.0);

    double angle = angleDist(rng);
    double distance = distDist(rng);
    double x = std::cos(angle) * distance;
    double z = std::sin(angle) * distance;
    double y = heightDist(rng);

    double speed = GameConstants::TARGET_SPEED + (wave - 1) * GameConstants::TARGET_SPEED_INCREMENT;

    Target target("target_" + std::to_string(targetIdCounter++), x, y, z, speed);
    targets.push_back(target);
}

void Game::checkCollisions() {
    double currentTime = SDL_GetTicks() / 1000.0;

    for (auto& proj : projectiles) {
        for (auto& target : targets) {
            if (!target.active) continue;

            double dx = proj.x - target.x;
            double dy = proj.y - target.y;
            double dz = proj.z - target.z;
            double dist = std::sqrt(dx*dx + dy*dy + dz*dz);

            if (dist < 0.5) {
                target.active = false;

                int comboMultiplier = 1;
                if (currentTime - lastHitTime < GameConstants::COMBO_TIME_WINDOW) {
                    comboMultiplier = std::min(combo + 1, GameConstants::MAX_COMBO);
                }

                score += target.points * comboMultiplier;
                combo = comboMultiplier;
                lastHitTime = currentTime;
                target.active = false;

                projectiles.erase(std::remove(projectiles.begin(), projectiles.end(), proj), projectiles.end());
                break;
            }
        }
    }

    targets.erase(std::remove_if(targets.begin(), targets.end(), 
        [](const Target& t) { return !t.active; }), targets.end());
}

void Game::nextWave() {
    if (wave >= GameConstants::TOTAL_WAVES) {
        endGame();
    } else {
        wave++;
        spawnWave();
    }
}

void Game::endGame() {
    state = GameState::GAMEOVER;
    if (score > highScore) {
        highScore = score;
    }
}

void Game::resetGame() {
    state = GameState::PLAYING;
    score = 0;
    wave = 1;
    combo = 0;
    targets.clear();
    projectiles.clear();
    player = Player();
    spawnWave();
}

void Game::shutdown() {
    renderer.cleanup();
}