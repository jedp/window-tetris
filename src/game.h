#pragma once

#include <piece.h>
#include <sequence.h>

const int H_BOARD = 20;
const int W_BOARD = 10;

const int W_PIECE = 4;
const int H_PIECE = 4;

const int START_ROW = 2;
const int START_COL = 4;

typedef enum {
  STILL_ALIVE,
  NOT_STILL_ALIVE,
} move_result_t;

void makeCanvas(shape_t &canvas);

class Game {

  public:
    Game(const shape_t &canvas, const Sequence &sequence);
    uint32_t getScore();
    move_result_t move(orientation_t movement);
    move_result_t rotate(orientation_t rotationDirection);
    move_result_t tick();

  private:
    piece_t pieces[NUM_PIECES];
    piece_t *currentPiece;
    shape_t board;
    shape_t canvas;
    Sequence sequence;
    uint32_t score;
    void produceNextPiece();
    void render();
    void reset();
};
