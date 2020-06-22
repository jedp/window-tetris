#pragma once

#include <piece.h>
#include <sequence.h>

const int H_BOARD = 20;
const int W_BOARD = 10;

const int W_PIECE = 4;
const int H_PIECE = 4;

const int START_ROW = 2;
const int START_COL = 4;

const char EMPTY_SPACE = ' ';
const point_t START_COORDINATES = (struct point) { 0, 3 };
const bbox_t BOARD_AREA = (struct bbox) {
  (struct point) { 0, 0 },
  (struct point) { H_BOARD - 1, W_BOARD - 1}
};

typedef struct stats {
  int score;
} stats_t;

class Game {

  public:
    Game(const Shape &canvas,
         const Sequence &sequence,
         void (*gameOverCallback)(void));
    bool moveLeft();
    bool moveRight();
    bool rotateClockwise();
    void drop();
    void tick();
    void reset();
    stats_t getStats() const { return stats; };

  private:
    piece_name_t currentPieceName;
    orientation_t currentOrientation;
    Sequence sequence;
    Shape canvas;
    Shape board;
    Piece pieces[NUM_PIECES];
    stats_t stats;
    bool move(orientation_t dir);
    void produceNextPiece();
    void score(int);
    void render();
    void gameOver();
    void (*gameOverCallback)(void);
};

