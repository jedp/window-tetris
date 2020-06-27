#include <game.h>

#include <assert.h>

#include <sequence.h>

uint16_t scoreForDroppedRows(uint8_t rows);
uint16_t scoreForRemovedRows(uint8_t rows);

void reset(game_t *game, sequence_t *sequence) {
  game->sequence = sequence;
  game->state = PLAYING;
  game->score = 0;
  game->showPiece = true;

  reshuffle(game->sequence);

  initBoard(&(game->canvas));
  initBoard(&(game->board));
  shape_name_t nextName = next(game->sequence);
  initShape(&(game->piece), nextName);
}

void produceNextPiece(game_t *game) {
  game->showPiece = true;
  shape_name_t nextName = next(game->sequence);

  initShape(&(game->piece), nextName);

  if (collides(&(game->board),
               game->piece.name,
               game->piece.orientation,
               &START_POINT)) {
    game->state = GAME_OVER;
    return;
  }

  render(game);
}

void render(game_t *game) {
  // Clear the canvas.
  initBoard(&(game->canvas));

  // Stick the board and the current piece to the canvas.
  setGrid(&(game->canvas), game->board.grid);
  if (game->showPiece) {
    stick(&(game->canvas), &(game->piece), &(game->piece.location));
  }
}


bool movePieceLeft(game_t *game) {
  return movePiece(game, LEFT);
}

bool movePieceRight(game_t *game) {
  return movePiece(game, RIGHT);
}

bool movePiece(game_t *game, orientation_t dir) {
  point_t dst;
  switch (dir) {
    case LEFT:
      dst.row = game->piece.location.row;
      dst.col = game->piece.location.col - 1;
      break;
    case RIGHT:
      dst.row = game->piece.location.row;
      dst.col = game->piece.location.col + 1;
      break;
    case DOWN:
      dst.row = game->piece.location.row + 1;
      dst.col = game->piece.location.col;
      break;
    default:
      assert(false);
  }

  // Is the piece still on the board at the proposed location?
  if (!inside(&(game->board),
              game->piece.name,
              game->piece.orientation,
              &dst)) {
    return false;
  }

  // Is the piece still free of the other shapes on the board?
  if (collides(&(game->board),
               game->piece.name,
               game->piece.orientation,
               &dst)) {
    return false;
  }

  game->piece.location = dst;

  render(game);

  return true;
}

bool rotatePiece(game_t *game) {
  if (game->state != PLAYING) return false;

  if (!inside(&(game->board),
              game->piece.name,
              nextClockwise(game->piece.orientation),
              &(game->piece.location))) {
    return false;
  }

  if (collides(&(game->board),
               game->piece.name,
               nextClockwise(game->piece.orientation),
               &(game->piece.location))) {
    return false;
  }

  game->piece.orientation = nextClockwise(game->piece.orientation);
  updateGrid(&(game->piece));

  render(game);

  return true;
}

void dropPiece(game_t *game) {
  if (game->state != PLAYING) return;

  int startRow = game->piece.location.row;

  while (movePiece(game, DOWN)) {}

  game->score += scoreForDroppedRows(game->piece.location.row - startRow);

  stickPiece(game);
}

void stickPiece(game_t *game) {
  stick(&(game->board), &(game->piece), &(game->piece.location));

  render(game);
  if (markDeadRows(&(game->board))) {
    game->state = SHOWING_FULL_ROWS;

    // Hide the current piece while we're doing fancy row-destroying animation.
    game->showPiece = false;
  } else {
    produceNextPiece(game);
  }
}

void tick(game_t *game) {
  switch (game->state) {
    case GAME_OVER:
      return;

    // When PLAYING, move the piece down one row.
    //
    // If it can't go any further, stick it where it is and see if any rows
    // are now full.
    case PLAYING:
      if (!movePiece(game, DOWN)) {
        stickPiece(game);
      }
      break;

    // When SHOWING_FULL_ROWS, remove the full rows and advance state.
    case SHOWING_FULL_ROWS:
      game->score += scoreForRemovedRows(removeDeadRows(&(game->board)));
      game->state = DELETING_FULL_ROWS;
      break;

    // When DELETING_FULL_ROWS, produce the next piece and keep playing.
    case DELETING_FULL_ROWS:
      produceNextPiece(game);
      game->state = PLAYING;
      break;

    default:
      assert(false);
  }
}

uint16_t scoreForRemovedRows(uint8_t rows) {
  // Classic scoring.
  if (rows == 1) return 30;
  if (rows == 2) return 100;
  if (rows == 3) return 300;
  return 1200;
}

uint16_t scoreForDroppedRows(uint8_t rows) {
  // Classic scoring.
  return rows;
}

