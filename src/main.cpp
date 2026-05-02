#include <SDL2/SDL.h>
#include <iostream>
#include "game.h"

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL initialization failed: " << SDL_GetError() << std::endl;
        return 1;
    }

    Game game;
    if (!game.init()) {
        std::cerr << "Game initialization failed" << std::endl;
        SDL_Quit();
        return 1;
    }

    game.run();
    game.shutdown();

    SDL_Quit();
    return 0;
}