#include <stdint.h>
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

bool validPlacement(const char *board, const char *piece, int row, int col) {
  return false;
}

