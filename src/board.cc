#include <stdint.h>
#include "piece.h"
#include "board.h"
#include "game.h"

bool noEmptySpacesInRange(const char *grid, int start, int end) {
  for (int j = start; j < end; ++j) {
    if (grid[j] == ' ') {
      return false;
    }
  }
  return true;
}

void getFullRowsRange(const char *board, int &start, int &end) {
  int _start = -1;
  int _end = -1;
  for (int i = 0; i < H_BOARD; ++i) {
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

