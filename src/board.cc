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

bool validPlacement(const char *board, int rows, int cols, shape_t shape, int row, int col) {
  // Bounds check.
  if (row - shape.bbox.top < 0) return false;
  if (row > rows - shape.bbox.bottom) return false;
  if (col - shape.bbox.left < 0) return false;
  if (col > cols - shape.bbox.right) return false;
  return true;
}

