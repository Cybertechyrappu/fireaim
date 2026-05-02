#include "renderer.h"
#include "constants.h"
#include <cmath>
#include <vector>
#include <string>

Renderer::Renderer() {}

Renderer::~Renderer() {
    cleanup();
}

bool Renderer::init(int width, int height) {
    screenWidth = width;
    screenHeight = height;

    SDL_SetHint(SDL_HINT_OPENGL_DEBUG_PLUGIN, "1");
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);

    window = SDL_CreateWindow(
        "Fireaim - 3D Shooter",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        width, height,
        SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN
    );

    if (!window) return false;

    glContext = SDL_GL_CreateContext(window);
    if (!glContext) return false;

    SDL_GL_SetSwapInterval(1);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    return true;
}

void Renderer::beginFrame() {
    glClearColor(0.04f, 0.04f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(75.0, (double)screenWidth / screenHeight, 0.1, 1000.0);
}

void Renderer::endFrame() {
    SDL_GL_SwapWindow(window);
}

void Renderer::renderScene(const Player& player, const std::vector<Target>& targets, 
                           const std::vector<Projectile>& projectiles, int score, int wave, int combo) {
    setupCamera(player);
    renderGround();
    renderPillars();

    glMatrixMode(GL_MODELVIEW);
    for (const auto& target : targets) {
        renderTarget(target);
    }

    for (const auto& proj : projectiles) {
        renderProjectile(proj);
    }
}

void Renderer::setupCamera(const Player& player) {
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    double lookX, lookY, lookZ;
    player.getLookDirection(lookX, lookY, lookZ);

    gluLookAt(
        player.x, player.y, player.z,
        player.x + lookX, player.y + lookY + 1.7, player.z + lookZ,
        0.0, 1.0, 0.0
    );
}

void Renderer::renderGround() {
    glColor3f(0.1f, 0.1f, 0.18f);
    glBegin(GL_QUADS);
    glVertex3f(-50.0f, -0.25f, -50.0f);
    glVertex3f(50.0f, -0.25f, -50.0f);
    glVertex3f(50.0f, -0.25f, 50.0f);
    glVertex3f(-50.0f, -0.25f, 50.0f);
    glEnd();
}

void Renderer::renderPillars() {
    for (int i = 0; i < 50; ++i) {
        float x = ((i * 7 % 40) - 20.0f) + (i % 3) * 5.0f;
        float z = ((i * 13 % 40) - 20.0f) + (i / 3 % 4) * 5.0f;
        float height = 2.0f + (i % 8) * 1.5f;

        float shade = 0.08f + (i % 10) * 0.02f;
        glColor3f(shade, shade * 1.2f, shade * 1.5f);

        glBegin(GL_QUADS);
        glVertex3f(x - 0.5f, 0.0f, z - 0.5f);
        glVertex3f(x + 0.5f, 0.0f, z - 0.5f);
        glVertex3f(x + 0.5f, height, z - 0.5f);
        glVertex3f(x - 0.5f, height, z - 0.5f);

        glVertex3f(x + 0.5f, 0.0f, z - 0.5f);
        glVertex3f(x + 0.5f, 0.0f, z + 0.5f);
        glVertex3f(x + 0.5f, height, z + 0.5f);
        glVertex3f(x + 0.5f, height, z - 0.5f);

        glVertex3f(x - 0.5f, 0.0f, z + 0.5f);
        glVertex3f(x - 0.5f, 0.0f, z - 0.5f);
        glVertex3f(x - 0.5f, height, z - 0.5f);
        glVertex3f(x - 0.5f, height, z + 0.5f);

        glVertex3f(x + 0.5f, 0.0f, z + 0.5f);
        glVertex3f(x - 0.5f, 0.0f, z + 0.5f);
        glVertex3f(x - 0.5f, height, z + 0.5f);
        glVertex3f(x + 0.5f, height, z + 0.5f);
        glEnd();
    }
}

void Renderer::renderTarget(const Target& target) {
    static const GLfloat colors[][3] = {
        {1.0f, 0.0f, 0.5f},
        {0.0f, 1.0f, 1.0f},
        {1.0f, 0.67f, 0.0f},
        {0.0f, 1.0f, 0.0f},
        {1.0f, 0.0f, 1.0f}
    };

    int colorIndex = std::hash<std::string>{}(target.id) % 5;
    glColor3fv(colors[colorIndex]);

    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(target.x, target.y, target.z);
    for (int i = 0; i <= 20; ++i) {
        float angle = i * 2.0f * M_PI / 20.0f;
        glVertex3f(target.x + 0.5f * std::cos(angle),
                   target.y,
                   target.z + 0.5f * std::sin(angle));
    }
    glEnd();

    glColor4f(colors[colorIndex][0], colors[colorIndex][1], colors[colorIndex][2], 0.3f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(target.x, target.y, target.z);
    for (int i = 0; i <= 20; ++i) {
        float angle = i * 2.0f * M_PI / 20.0f;
        glVertex3f(target.x + 0.6f * std::cos(angle),
                   target.y,
                   target.z + 0.6f * std::sin(angle));
    }
    glEnd();
}

void Renderer::renderProjectile(const Projectile& proj) {
    glColor3f(0.0f, 1.0f, 0.53f);
    glBegin(GL_POINTS);
    glVertex3f(proj.x, proj.y, proj.z);
    glEnd();

    glColor4f(0.0f, 1.0f, 0.53f, 0.5f);
    glBegin(GL_LINES);
    glVertex3f(proj.x, proj.y, proj.z);
    glVertex3f(proj.x - proj.dirX * 0.3, proj.y - proj.dirY * 0.3, proj.z - proj.dirZ * 0.3);
    glEnd();
}

void Renderer::renderCrosshair() {
    glDisable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, screenWidth, screenHeight, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glColor3f(0.0f, 1.0f, 0.53f);
    float cx = screenWidth / 2.0f;
    float cy = screenHeight / 2.0f;

    glBegin(GL_LINE_LOOP);
    glVertex2f(cx - 15, cy);
    glVertex2f(cx - 8, cy);
    glVertex2f(cx, cy - 15);
    glVertex2f(cx, cy - 8);
    glVertex2f(cx + 8, cy);
    glVertex2f(cx + 15, cy);
    glVertex2f(cx, cy + 8);
    glVertex2f(cx, cy + 15);
    glEnd();

    glPointSize(6);
    glBegin(GL_POINTS);
    glVertex2f(cx, cy);
    glEnd();

    glEnable(GL_DEPTH_TEST);
}

void Renderer::renderHUD(int score, int wave, int combo) {
    glDisable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, screenWidth, screenHeight, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glColor3f(0.0f, 1.0f, 0.53f);
    void* font = GLUT_BITMAP_HELVETICA_18;

    glEnable(GL_TEXTURE_2D);

    glColor4f(0.0f, 0.0f, 0.0f, 0.5f);
    glBegin(GL_QUADS);
    glVertex2f(10, 10);
    glVertex2f(180, 10);
    glVertex2f(180, 90);
    glVertex2f(10, 90);
    glEnd();

    glColor3f(0.0f, 1.0f, 0.53f);

    if (combo > 1) {
        glColor4f(1.0f, 0.0f, 0.5f, 0.8f);
        glBegin(GL_QUADS);
        glVertex2f(10, 100);
        glVertex2f(120, 100);
        glVertex2f(120, 130);
        glVertex2f(10, 130);
        glEnd();
    }

    glEnable(GL_DEPTH_TEST);
}

void Renderer::cleanup() {
    if (glContext) SDL_GL_DeleteContext(glContext);
    if (window) SDL_DestroyWindow(window);
}