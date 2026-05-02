#ifndef RENDERER_H
#define RENDERER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include "player.h"
#include "target.h"
#include "projectile.h"
#include <vector>

class Renderer {
public:
    Renderer();
    ~Renderer();

    bool init(int width, int height);
    void beginFrame();
    void endFrame();
    void renderScene(const Player& player, const std::vector<Target>& targets, 
                     const std::vector<Projectile>& projectiles, int score, int wave, int combo);
    void renderCrosshair();
    void renderHUD(int score, int wave, int combo);
    void cleanup();

private:
    SDL_Window* window = nullptr;
    SDL_GLContext glContext = nullptr;
    int screenWidth = 0;
    int screenHeight = 0;

    void renderGround();
    void renderPillars();
    void renderTarget(const Target& target);
    void renderProjectile(const Projectile& proj);
    void setupCamera(const Player& player);
};

#endif // RENDERER_H