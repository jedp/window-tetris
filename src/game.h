#pragma once

#include <piece.h>
#include <sequence.h>

const int H_BOARD = 20;
const int W_BOARD = 10;

const int W_PIECE = 4;
const int H_PIECE = 4;

const int START_ROW = 2;
const int START_COL = 4;

const point_t START_COORDINATES = (struct point) { 0, 3 };
const point_t OFFSCREEN_COORDINATES = (struct point) { -4, 3 };

const bbox_t BOARD_AREA = (struct bbox) {
  (struct point) { 0, 0 },
  (struct point) { H_BOARD - 1, W_BOARD - 1}
};

typedef struct stats {
  int score;
} stats_t;

typedef enum state {
  PLAYING,
  SHOWING_FULL_ROWS,
  DELETING_FULL_ROWS,
} state_t;

class Game {

  public:
    Game(const Shape &canvas,
         const Sequence &sequence,
         void (*gameOverCallback)(void));
    void setGrid(const char *grid);
    bool moveLeft();
    bool moveRight();
    bool rotateClockwise();
    void drop();
    void tick();
    void reset();
    stats_t getStats() const { return stats; };

  private:
    Shape canvas;
    Sequence sequence;
    void (*gameOverCallback)(void);
    stats_t stats;
    state_t state;
    piece_name_t currentPieceName;
    orientation_t currentOrientation;
    Shape board;
    Piece pieces[NUM_PIECES];
    bool move(orientation_t dir);
    void stickCurrentPiece();
    void hideCurrentPiece();
    void produceNextPiece();
    void scoreRemovedRows(int rows);
    void scoreDroppedRows(int rows);
    void render();
    void gameOver();
};

