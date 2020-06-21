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

typedef enum {
  STILL_ALIVE,
  NOT_STILL_ALIVE,
} move_result_t;

class Game {

  public:
    Game(const Shape &canvas, const Sequence &sequence);
    bool moveLeft();
    bool moveRight();
    bool rotateClockwise();
    void tick();

  private:
    uint32_t getScore() const { return score; }
    piece_name_t currentPieceName;
    orientation_t currentOrientation;
    Sequence sequence;
    Shape canvas;
    Shape board;
    Piece pieces[NUM_PIECES];
    uint32_t score;
    bool move(orientation_t dir);
    void produceNextPiece();
    void render();
    void reset();
    void gameOver();
};

