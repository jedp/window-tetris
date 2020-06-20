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

void makeCanvas(Shape &canvas);

class Game {

  public:
    Game(const Shape &canvas, const Sequence &sequence, void (*gameOverCallback)());
    move_result_t move(orientation_t movement);
    move_result_t rotate(orientation_t rotationDirection);
    move_result_t tick();

  private:
    uint32_t getScore() const { return score; }
    void (*gameOverCallback)();
    Piece pieces[NUM_PIECES];
    Piece *currentPiece;
    Shape board;
    Shape canvas;
    Sequence sequence;
    uint32_t score;
    void produceNextPiece();
    void render();
    void reset();
    void gameOver();
};
