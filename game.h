#ifndef GAME_H
#define GAME_H
#include <cstring>

const int BOARD_W=10;
const int BOARD_H=20;
const int MAX_RANK=10;

extern const int PIECES[7][4][4][4];
const char* getGrade(int score, int lines);

struct Piece
{
    int type;
    int rot;
    int x, y;
    void reset(int t);
    const int (*shape() const)[4];
};

struct Game
{
    int board[BOARD_H][BOARD_W];
    Piece cur, nxt;
    int score, lines, level;
    bool over;
    int speed;
    int ranking[MAX_RANK];

    void init();
    void spawnPiece();
    bool canPlace(const Piece& p, int nx, int ny) const;
    void moveLeft();
    void moveRight();
    void rotate();
    bool moveDown();
    void hardDrop();
    int ghostY() const;
    void lockPiece();
    void clearLines();
    void submitScore();
};


#endif