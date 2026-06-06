#ifndef RENDER_H
#define RENDER_H
#include "game.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

struct Renderer {
    SDL_Window*   window   = nullptr;
    SDL_Renderer* renderer = nullptr;
    TTF_Font*     fontLg   = nullptr;
    TTF_Font*     fontMd   = nullptr;
    TTF_Font*     fontSm   = nullptr;

    bool init();
    void destroy();
    void draw(const Game& g);
    void drawGameOver(const Game& g);
};

#endif