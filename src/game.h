#ifndef SRC_GAME_H_
#define SRC_GAME_H_

#include <board.h>
#include <pieces.h>
#include <sequence.h>

typedef enum {
  PLAYING,
  SHOWING_FULL_ROWS,
  DELETING_FULL_ROWS,
  GAME_OVER,
} state_t;

typedef struct {
  board_t canvas;
  board_t board;
  shape_t piece;
  state_t state;
  bool showPiece;
  sequence_t *sequence;
  uint32_t score;
} game_t;

void reset(game_t *game, sequence_t *sequence);

void produceNextPiece(game_t *game);

void render(game_t *game);

bool movePieceLeft(game_t *game);

bool movePieceRight(game_t *game);

bool movePiece(game_t *game, orientation_t direction);

bool rotatePiece(game_t *game);

void dropPiece(game_t *game);

void stickPiece(game_t *game);

void tick(game_t *game);

#endif  // SRC_GAME_H_

