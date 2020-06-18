#include <stdint.h>
#include "piece.h"
#include "board.h"
#include "game.h"
#include <iostream>

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

bool inBounds(shape_t board, shape_t shape, int row, int col) {
  if (row - shape.bbox.top < 0) return false;
  if (row > board.rows - shape.bbox.bottom) return false;
  if (col - shape.bbox.left < 0) return false;
  if (col > board.cols - shape.bbox.right) return false;

  return true;
}

bool collide(shape_t board, shape_t shape, int row, int col) {
  if (row >= board.rows) return false;
  if (col >= board.cols) return false;

  for (int i = row; i < (int)board.rows && i < (int)shape.rows + row; ++i) {
    // We don't have off-screen collisions.
    if (i < 0) continue;

    for (int j = col; j < (int)board.cols && j < (int)shape.cols + col; ++j) {
      // We don't have off-screen collisions.
      if (j < 0) continue;

      // i and j are in grid space.
      // Offset to get the local coordinate in the shape.
      if (shape.grid[(i - row) * shape.cols + (j - col)] != ' '
          && board.grid[i * board.cols + j] != ' ') {
        return true;
      }
    }
  }
  return false;
}

void stick(shape_t board, shape_t shape, int row, int col) {
  for (int i = row; i < (int)board.rows && i < (int)shape.rows + row; ++i) {
    // We don't have off-screen geometry.
    if (i < 0) continue;

    for (int j = col; j < (int)board.cols && j < (int)shape.cols + col; ++j) {
      // We don't have off-screen geometry.
      if (j < 0) continue;

      // i and j are in grid space.
      // Offset to get the local coordinate in the shape.
      char cell = shape.grid[(i - row) * shape.cols + (j - col)];
      if (cell != ' ') {
        board.grid[i * board.cols + j] = cell;
      }
    }
  }
}

