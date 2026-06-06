#include "game.h"
#include <cstdlib>
#include <ctime>
#include <algorithm>

const int PIECES[7][4][4][4] = {
    {
        //I shape
        {{0, 0, 0, 0}, {1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}},
        {{0, 0, 1, 0}, {0, 0, 1, 0}, {0, 0, 1, 0}, {0, 0, 1, 0}},
        {{0, 0, 0, 0}, {0, 0, 0, 0}, {1, 1, 1, 1}, {0, 0, 0, 0}},
        {{0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}},
    },
    {
        //O shape
        {{0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
        {{0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
        {{0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
        {{0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
    },
    {
        //T shape
        {{0, 1, 0, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
        {{0, 1, 0, 0}, {0, 1, 1, 0}, {0, 1, 0, 0}, {0, 0, 0, 0}},
        {{0, 0, 0, 0}, {1, 1, 1, 0}, {0, 1, 0, 0}, {0, 0, 0, 0}},
        {{0, 1, 0, 0}, {1, 1, 0, 0}, {0, 1, 0, 0}, {0, 0, 0, 0}},
    },
    {
        //S shape
        {{0, 1, 1, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
        {{0, 1, 0, 0}, {0, 1, 1, 0}, {0, 0, 1, 0}, {0, 0, 0, 0}},
        {{0, 0, 0, 0}, {0, 1, 1, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}},
        {{1, 0, 0, 0}, {1, 1, 0, 0}, {0, 1, 0, 0}, {0, 0, 0, 0}},
    },
    {
        //Z shape
        {{1, 1, 0, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
        {{0, 0, 1, 0}, {0, 1, 1, 0}, {0, 1, 0, 0}, {0, 0, 0, 0}},
        {{0, 0, 0, 0}, {1, 1, 0, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}},
        {{0, 1, 0, 0}, {1, 1, 0, 0}, {1, 0, 0, 0}, {0, 0, 0, 0}},
    },
    {
        //J shape
        {{1, 0, 0, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
        {{0, 1, 1, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}, {0, 0, 0, 0}},
        {{0, 0, 0, 0}, {1, 1, 1, 0}, {0, 0, 1, 0}, {0, 0, 0, 0}},
        {{0, 1, 0, 0}, {0, 1, 0, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}},
    },
    {
        //L shape
        {{0, 0, 1, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
        {{0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}},
        {{0, 0, 0, 0}, {1, 1, 1, 0}, {1, 0, 0, 0}, {0, 0, 0, 0}},
        {{1, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}, {0, 0, 0, 0}},
    }
};

const char* getGrade(int score, int lines) {
    if (lines == 0) return "E";
    int efficiency = score / lines;

    if (score >= 20000 && efficiency > 200) return "S+";
    if (score >= 15000 && efficiency > 180) return "S";
    if (score >= 10000 && efficiency > 150) return "A";
    if (score >= 6000  && efficiency > 120) return "B";
    if (score >= 3000  && efficiency > 100) return "C";
    if (score >= 1000) return "D";
    return "E";
}

void Piece::reset(int t){
    type = t;
    rot = 0;
    x = BOARD_W/2-2;
    y=0;
}

const int (*Piece::shape() const)[4]{
    return PIECES[type][rot];
}

void Game::init(){
    memset(board, 0, sizeof(board));
    score = lines = level = 0;
    over = false;
    speed = 800;
    srand((unsigned)time(nullptr));
    nxt.reset(rand() % 7);
    spawnPiece();
}

void Game::spawnPiece(){
    cur = nxt;
    nxt.reset(rand() % 7);
    if (!canPlace(cur, cur.x, cur.y)){
        over = true;
    }
}

bool Game::canPlace(const Piece& p, int nx, int ny) const{
    const int (*s)[4] = p.shape();
    for (int r=0; r < 4; r++){
        for (int c=0; c < 4; c++){
            if (s[r][c]){
                int bx = nx + c;
                int by = ny + r;
                if (bx < 0 || bx >= BOARD_W || by < 0 || by >= BOARD_H){
                    return false;
                }
                if (board[by][bx]){
                    return false;
                }
            }
        }
    }
    return true;
}

void Game::moveLeft(){
    if (canPlace(cur, cur.x-1, cur.y)){
        cur.x--;
    }
}

void Game::moveRight(){
    if (canPlace(cur, cur.x+1, cur.y)){
        cur.x++;
    }
}

void Game::rotate(){
    Piece tmp = cur;
    tmp.rot = (tmp.rot + 1) % 4;
    int kicks[] = {0, -1, 1, -2, 2};
    for (int k: kicks){
        if (canPlace(tmp, tmp.x + k, tmp.y)){
            cur = tmp;
            cur.x += k;
            return;
        }
    }
}

bool Game::moveDown(){
    if (canPlace(cur, cur.x, cur.y + 1)){
        cur.y++;
        return true;
    }
    lockPiece();
    return false;
}

void Game::hardDrop(){
    cur.y = ghostY();
    lockPiece();
}

int Game::ghostY() const{
    int gy = cur.y;
    while (canPlace(cur, cur.x, gy + 1)){
        gy++;
    }
    return gy;
}

void Game::lockPiece(){
    const int (*s)[4] = cur.shape();
    for (int r=0; r < 4; r++){
        for (int c=0; c < 4; c++){
            if (s[r][c]){
                board[cur.y + r][cur.x + c] = cur.type + 1;
            }
        }
    }
    clearLines();
    spawnPiece();
}

void Game::clearLines(){
    int cleared = 0;
    for (int r=BOARD_H - 1; r >= 0; ){
        bool full = true;
        for (int c=0; c < BOARD_W; c++){
            if (!board[r][c]){
                full = false;
                break;
            }
        }
        if (full){
            for (int rr=r; rr > 0; rr--){
                memcpy(board[rr], board[rr-1], sizeof(board[0]));
            }
            memset(board[0], 0, sizeof(board[0]));
            cleared++;
        }else{
            r--;
        }
    }
    if (cleared){
        static const int pts[] = {0, 100, 300, 500, 800};
        score += pts[std::min(cleared, 4)] * (level + 1);
        lines += cleared;
        level = lines / 10;
        speed = std::max(100, 800 - level * 70);
    }
}

void Game::submitScore(){
    for (int i=0; i < MAX_RANK; i++){
        if (score > ranking[i]){
            for (int j = MAX_RANK - 1; j > i; j--){
                ranking[j] = ranking[j-1];
            }
            ranking[i] = score;
            break;
        }
    }
}