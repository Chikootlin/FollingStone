#include "render.h"
#include <cstdio>
#include <cstring>

static const SDL_Color C_BG     = {18,  18,  18,  255};
static const SDL_Color C_WHITE  = {255, 255, 255, 255};
static const SDL_Color C_GHOST  = {60,  60,  60,  255};
static const SDL_Color C_BLOCK  = {220, 220, 220, 255};
static const SDL_Color C_BORDER = {255, 255, 255, 255};
static const SDL_Color C_DIM    = {100, 100, 100, 255};

static const int WIN_W = 900;
static const int WIN_H = 680;
static const int CELL  = 28;
static const int PAD   = 18;

static const int LP_W = (WIN_W - BOARD_W * CELL) / 2 - PAD * 2;
static const int LP_X = PAD;

static const int BRD_H = WIN_H - PAD * 2;
static const int BRD_W = BOARD_W * CELL;
static const int BRD_X = (WIN_W - BRD_W) / 2;
static const int BRD_Y = PAD;
static const int CELL_H = BRD_H / BOARD_H;

static const int RP_X = BRD_X + BRD_W + PAD * 2;
static const int RP_W = WIN_W - RP_X - PAD;

static TTF_Font* loadFont(const char* path, int size) {
    return TTF_OpenFont(path, size);
}

static void setColor(SDL_Renderer* r, SDL_Color c) {
    SDL_SetRenderDrawColor(r, c.r, c.g, c.b, c.a);
}

static void drawRect(SDL_Renderer* r, int x, int y, int w, int h, SDL_Color c) {
    setColor(r, c);
    SDL_Rect rect = {x, y, w, h};
    SDL_RenderFillRect(r, &rect);
}

static void drawOutlineRect(SDL_Renderer* r, int x, int y, int w, int h, SDL_Color c, int thickness = 2) {
    setColor(r, c);
    for (int i = 0; i < thickness; i++) {
        SDL_Rect rect = {x+i, y+i, w-i*2, h-i*2};
        SDL_RenderDrawRect(r, &rect);
    }
}

static void renderText(SDL_Renderer* r, TTF_Font* font, const char* text, int x, int y, SDL_Color c, bool centerX = false, int boxW = 0) {
    if (!font || !text || text[0] == '\0') return;
    SDL_Surface* surf = TTF_RenderText_Blended(font, text, c);
    if (!surf) return;
    SDL_Texture* tex = SDL_CreateTextureFromSurface(r, surf);
    int tw = surf->w, th = surf->h;
    SDL_FreeSurface(surf);
    if (!tex) return;

    if (centerX && boxW > 0)
        x = x + (boxW - tw) / 2;
    SDL_Rect dst = {x, y, tw, th};
    SDL_RenderCopy(r, tex, nullptr, &dst);
    SDL_DestroyTexture(tex);
}

static void drawBoard(SDL_Renderer* r, const Game& g) {
    drawRect(r, BRD_X, BRD_Y, BRD_W, BRD_H, {0, 0, 0, 255});
    drawOutlineRect(r, BRD_X - 2, BRD_Y - 2, BRD_W + 4, BRD_H + 4, C_BORDER, 2);
    for (int row = 0; row < BOARD_H; row++)
        for (int col = 0; col < BOARD_W; col++)
            if (g.board[row][col]) {
                int px = BRD_X + col * CELL;
                int py = BRD_Y + row * CELL_H;
                drawRect(r, px+1, py+1, CELL-2, CELL_H-2, C_BLOCK);
                drawRect(r, px+1, py+1, CELL-2, 3, {255,255,255,60});
            }
    int gy = g.ghostY();
    const int (*gs)[4] = g.cur.shape();
    for (int row = 0; row < 4; row++)
        for (int col = 0; col < 4; col++)
            if (gs[row][col]) {
                int by = gy + row, bx = g.cur.x + col;
                if (by >= 0 && by < BOARD_H && bx >= 0 && bx < BOARD_W && !g.board[by][bx]) {
                    int px = BRD_X + bx * CELL;
                    int py = BRD_Y + by * CELL_H;
                    drawOutlineRect(r, px+1, py+1, CELL-2, CELL_H-2, C_GHOST, 1);
                }
            }
    const int (*cs)[4] = g.cur.shape();
    for (int row = 0; row < 4; row++)
        for (int col = 0; col < 4; col++)
            if (cs[row][col]) {
                int by = g.cur.y + row, bx = g.cur.x + col;
                if (by >= 0 && by < BOARD_H && bx >= 0 && bx < BOARD_W) {
                    int px = BRD_X + bx * CELL;
                    int py = BRD_Y + by * CELL_H;
                    drawRect(r, px+1, py+1, CELL-2, CELL_H-2, C_WHITE);
                    drawRect(r, px+1, py+1, CELL-2, 3, {255,255,255,80});
                }
            }
    setColor(r, {35, 35, 35, 255});
    for (int col = 1; col < BOARD_W; col++) {
        int x = BRD_X + col * CELL;
        SDL_RenderDrawLine(r, x, BRD_Y, x, BRD_Y + BRD_H);
    }
    for (int row = 1; row < BOARD_H; row++) {
        int y = BRD_Y + row * CELL_H;
        SDL_RenderDrawLine(r, BRD_X, y, BRD_X + BRD_W, y);
    }
}

static void drawLeftPanel(SDL_Renderer* r, TTF_Font* fontLg, TTF_Font* fontMd, const Game& g) {
    int x = LP_X;
    int gradeBoxH = 110;
    drawRect(r, x, BRD_Y, LP_W, gradeBoxH, {0,0,0,255});
    drawOutlineRect(r, x, BRD_Y, LP_W, gradeBoxH, C_BORDER, 2);
    renderText(r, fontMd, "GRADE:", x + 10, BRD_Y + 10, C_WHITE);

    const char* grade = getGrade(g.score, g.lines);
    renderText(r, fontLg, grade, x, BRD_Y + 50, C_WHITE, true, LP_W);

    int rankY = BRD_Y + gradeBoxH + PAD;
    int rankH = (WIN_H - PAD) - rankY;
    drawRect(r, x, rankY, LP_W, rankH, {0,0,0,255});
    drawOutlineRect(r, x, rankY, LP_W, rankH, C_BORDER, 2);
    renderText(r, fontMd, "RANKING:", x + 10, rankY + 10, C_WHITE);

    int entryY = rankY + 44;
    int lineH  = (rankH - 54) / MAX_RANK;
    for (int i = 0; i < MAX_RANK; i++) {
        char buf[32];
        snprintf(buf, sizeof(buf), "%2d. %04d PT", i+1, g.ranking[i] > 0 ? g.ranking[i] : 0);
        renderText(r, fontMd, buf, x + 14, entryY + i * lineH, C_WHITE);
    }
}

static void drawRightPanel(SDL_Renderer* r, TTF_Font* fontLg, TTF_Font* fontMd, const Game& g) {
    int x = RP_X;
    int gpH = 110;
    drawRect(r, x, BRD_Y, RP_W, gpH, {0,0,0,255});
    drawOutlineRect(r, x, BRD_Y, RP_W, gpH, C_BORDER, 2);

    renderText(r, fontMd, "GAME POINT:", x + 10, BRD_Y + 10, C_WHITE);

    char ptbuf[32];
    snprintf(ptbuf, sizeof(ptbuf), "%04dpt", g.score);
    renderText(r, fontLg, ptbuf, x, BRD_Y + 58, C_WHITE, true, RP_W);
}

static void drawGameOverOverlay(SDL_Renderer* r, TTF_Font* fontLg, TTF_Font* fontMd, TTF_Font* fontSm, const Game& g) {
    SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(r, 0, 0, 0, 180);
    SDL_Rect overlay = {BRD_X, BRD_Y, BRD_W, BRD_H};
    SDL_RenderFillRect(r, &overlay);
    SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_NONE);

    int bw = 260, bh = 130;
    int bx = BRD_X + (BRD_W - bw) / 2;
    int by = BRD_Y + (BRD_H - bh) / 2;

    drawRect(r, bx, by, bw, bh, {0,0,0,255});
    drawOutlineRect(r, bx, by, bw, bh, C_BORDER, 2);
    renderText(r, fontLg, "GAME OVER", bx, by + 12, C_WHITE, true, bw);

    char buf[32];
    snprintf(buf, sizeof(buf), "%04dpt", g.score);
    renderText(r, fontMd, buf, bx, by + 64, C_WHITE, true, bw);
    renderText(r, fontSm, "R=retry   Q=quit", bx, by + 96, C_DIM, true, bw);
}

bool Renderer::init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) return false;
    if (TTF_Init() < 0) return false;
    window = SDL_CreateWindow(
        "FALLING STONE",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WIN_W, WIN_H,
        SDL_WINDOW_SHOWN);
    if (!window) return false;

    renderer = SDL_CreateRenderer(window, -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) return false;

    fontLg = loadFont("assets/PixelifySans-Bold.ttf", 42);
    fontMd = loadFont("assets/PixelifySans-Regular.ttf", 20);
    fontSm = loadFont("assets/PixelifySans-Regular.ttf", 15);

    if (!fontLg || !fontMd || !fontSm) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Font Error",
            "Could not load font assets.", window);
        return false;
    }

    return true;
}

void Renderer::destroy() {
    if (fontLg)   TTF_CloseFont(fontLg);
    if (fontMd)   TTF_CloseFont(fontMd);
    if (fontSm)   TTF_CloseFont(fontSm);
    if (renderer) SDL_DestroyRenderer(renderer);
    if (window)   SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
}

void Renderer::draw(const Game& g) {
    setColor(renderer, C_BG);
    SDL_RenderClear(renderer);

    drawBoard(renderer, g);
    drawLeftPanel(renderer, fontLg, fontMd, g);
    drawRightPanel(renderer, fontLg, fontMd, g);
    if (g.over) {
        drawGameOverOverlay(renderer, fontLg, fontMd, fontSm, g);
    }
    SDL_RenderPresent(renderer);
}

void Renderer::drawGameOver(const Game& g) {
    draw(g);
}