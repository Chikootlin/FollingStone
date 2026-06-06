#include "game.h"
#include "render.h"
#include <SDL2/SDL.h>
#include <cstring>

int main(int argc, char* argv[]) {
    Renderer rnd;
    if (!rnd.init()) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Init Error", SDL_GetError(), nullptr);
        return 1;
    }

    Game game;
    memset(game.ranking, 0, sizeof(game.ranking));
    game.init();

    bool running = true;
    bool scoreSubmitted = false;
    Uint32 lastTick = SDL_GetTicks();

    while (running) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                running = false;
            }
            else if (e.type == SDL_KEYDOWN) {
                if (!game.over) {
                    switch (e.key.keysym.sym) {
                        case SDLK_LEFT:  game.moveLeft();  break;
                        case SDLK_RIGHT: game.moveRight(); break;
                        case SDLK_UP:    game.rotate();    break;
                        case SDLK_DOWN:  game.moveDown();  break;
                        case SDLK_SPACE: game.hardDrop();  break;
                        case SDLK_q:     running = false;  break;
                    }
                } else {
                    if (!scoreSubmitted) {
                        game.submitScore();
                        scoreSubmitted = true;
                    }

                    switch (e.key.keysym.sym) {
                        case SDLK_r:
                            game.init();
                            scoreSubmitted = false;
                            break;
                        case SDLK_q:
                            running = false;
                            break;
                    }
                }
            }
        }

        Uint32 now = SDL_GetTicks();
        if (!game.over && (now - lastTick) >= (Uint32)game.speed) {
            game.moveDown();
            lastTick = now;
        }
        if (game.over && !scoreSubmitted) {
            game.submitScore();
            scoreSubmitted = true;
        }
        rnd.draw(game);
    }
    rnd.destroy();
    return 0;
}