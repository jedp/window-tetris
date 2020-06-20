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

bool inBounds(shape_t board, shape_t shape, point_t dst) {
  if (dst.row - shape.bbox.uLeft.row < 0) return false;
  if (dst.row > board.rows - shape.bbox.lRight.row) return false;
  if (dst.col - shape.bbox.uLeft.col < 0) return false;
  if (dst.col > board.cols - shape.bbox.lRight.col) return false;

  return true;
}

bool collide(shape_t board, shape_t shape, point_t dst) {
  if (dst.row >= board.rows) return false;
  if (dst.col >= board.cols) return false;

  // If it's above the board's bounding box, don't do any hard work.
  if (dst.row + shape.rows < board.bbox.uLeft.row) return false;

  for (int i = dst.row; i < (int)board.rows && i < (int)shape.rows + dst.row; ++i) {
    // We don't have off-screen collisions.
    if (i < 0) continue;

    for (int j = dst.col; j < (int)board.cols && j < (int)shape.cols + dst.col; ++j) {
      // We don't have off-screen collisions.
      if (j < 0) continue;

      // i and j are in grid space.
      // Offset to get the local coordinate in the shape.
      if (shape.grid[(i - dst.row) * shape.cols + (j - dst.col)] != ' '
          && board.grid[i * board.cols + j] != ' ') {
        return true;
      }
    }
  }
  return false;
}

void stick(shape_t board, shape_t shape, point_t dst) {
  for (int i = dst.row; i < (int)board.rows && i < (int)shape.rows + dst.row; ++i) {
    // We don't have off-screen geometry.
    if (i < 0) continue;

    for (int j = dst.col; j < (int)board.cols && j < (int)shape.cols + dst.col; ++j) {
      // We don't have off-screen geometry.
      if (j < 0) continue;

      // i and j are in grid space.
      // Offset to get the local coordinate in the shape.
      char cell = shape.grid[(i - dst.row) * shape.cols + (j - dst.col)];
      if (cell != ' ') {
        board.grid[i * board.cols + j] = cell;
      }
    }
  }

  updateBoundingBox(board);
}

