#include <stdint.h>
#include "piece.h"
#include "board.h"
#include "game.h"

bool noEmptySpacesInRange(const char *grid, int start, int end) {
  for (uint8_t j = start; j < end; ++j) {
    if (grid[j] == ' ') {
      return false;
    }
  }
  return true;
}

void getFullRowsRange(const char *board, int &start, int &end) {
  int _start = -1;
  int _end = -1;
  for (uint8_t i = 0; i < H_BOARD; ++i) {
    if (noEmptySpacesInRange(board, i * W_BOARD, i * W_BOARD + W_BOARD)) {
      if (_start == -1) _start = i * W_BOARD;
      _end = i * W_BOARD + W_BOARD;
    }
  }
  start = _start;
  end = _end;
}

bool inBounds(Shape board, Shape shape, point_t dst) {
  if (dst.row - shape.getBBox().uLeft.row < 0) return false;
  if (dst.row > board.getRows() - shape.getBBox().lRight.row) return false;
  if (dst.col - shape.getBBox().uLeft.col < 0) return false;
  if (dst.col > board.getCols() - shape.getBBox().lRight.col) return false;

  return true;
}

bool collide(Shape board, Shape shape, point_t dst) {
  if (dst.row >= board.getRows()) return false;
  if (dst.col >= board.getCols()) return false;

  // If it's above the board's bounding box, don't do any hard work.
  if (dst.row + shape.getRows() < board.getBBox().uLeft.row) return false;

  for (int i = dst.row; i < (int)board.getRows() && i < (int)shape.getRows() + dst.row; ++i) {
    // We don't have off-screen collisions.
    if (i < 0) continue;

    for (int j = dst.col; j < (int)board.getCols() && j < (int)shape.getCols() + dst.col; ++j) {
      // We don't have off-screen collisions.
      if (j < 0) continue;

      // i and j are in grid space.
      // Offset to get the local coordinate in the shape.
      if (shape.getGrid()[(i - dst.row) * shape.getCols() + (j - dst.col)] != ' '
          && board.getGrid()[i * board.getCols() + j] != ' ') {
        return true;
      }
    }
  }
  return false;
}

void stick(Shape board, Shape shape, point_t dst) {
  for (int i = dst.row; i < (int)board.getRows() && i < (int)shape.getRows() + dst.row; ++i) {
    // We don't have off-screen geometry.
    if (i < 0) continue;

    for (int j = dst.col; j < (int)board.getCols() && j < (int)shape.getCols() + dst.col; ++j) {
      // We don't have off-screen geometry.
      if (j < 0) continue;

      // i and j are in grid space.
      // Offset to get the local coordinate in the shape.
      char cell = shape.getGrid()[(i - dst.row) * shape.getCols() + (j - dst.col)];
      if (cell != ' ') {
        board.getGrid()[i * board.getCols() + j] = cell;
      }
    }
  }

  board.updateBoundingBox();
}

