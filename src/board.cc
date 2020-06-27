#include <board.h>

void initBoard(board_t *board) {
  fillWith(board, EMPTY_CELL);
}

void fillWith(board_t *board, char c) {
  for (uint16_t i = 0; i < BOARD_CELLS; ++i) {
    board->grid[i] = c;
  }
}

void setGrid(board_t *board, const char *grid) {
  for (uint16_t i = 0; i < BOARD_CELLS; ++i) {
    board->grid[i] = grid[i];
  }
}

void fillRowWith(board_t *board, uint8_t row, char c) {
  for (uint16_t i = row * BOARD_COLS; i < row * BOARD_COLS + BOARD_COLS; ++i) {
    board->grid[i] = c;
  }
}

bool inside(const board_t *board,
            const shape_name_t shapeName,
            const orientation_t orientation,
            const point_t *dst) {
  const bbox_t bbox = bboxes[shapeName * NUM_ORIENTATIONS + orientation];
  if (bbox.top + dst->row < 0) return false;
  if (bbox.left + dst->col < 0) return false;
  if (bbox.bottom + dst->row >= BOARD_ROWS) return false;
  if (bbox.right + dst->col >= BOARD_COLS) return false;
  return true;
}

bool collides(const board_t *board,
              const shape_name_t shapeName,
              const orientation_t orientation,
              const point_t *dst) {
  // Get the offset in shapeGrids of the shape with this name and orientation.
  uint16_t shapeIndex;
  uint16_t shapeOffset = shapeName * PIECE_CELLS * NUM_ORIENTATIONS;
  uint16_t orientationOffset = orientation * PIECE_CELLS;
  const char *shapeGrid = shapeGrids + shapeOffset + orientationOffset;

  for (int16_t row = dst->row;
       row < BOARD_ROWS && row < dst->row + PIECE_EDGE; ++row) {
    // boardCoordinates can be negative.
    if (row < 0) continue;

    for (int16_t col = dst->col;
         col < BOARD_COLS && col < dst->col + PIECE_EDGE; ++col) {
      if (col < 0) continue;

        shapeIndex = (row - dst->row) * PIECE_EDGE + (col - dst->col);
        if (board->grid[row * BOARD_COLS + col] != EMPTY_CELL
            && shapeGrid[shapeIndex] != EMPTY_CELL) {
          return true;
        }
    }
  }
  return false;
}

void stick(board_t *board, const shape_t *shape, const point_t *dst) {
  // boardCoordinates can be negative.
  char cell;
  for (int16_t row = dst->row;
       row < BOARD_ROWS && row < dst->row + 4; ++row) {
    if (row < 0) continue;

    for (int16_t col = dst->col;
         col < BOARD_COLS && col < dst->col + 4; ++col) {
      if (col < 0) continue;

      cell = shape->grid[(row - dst->row) * 4 + (col - dst->col)];
      if (cell != EMPTY_CELL) {
        board->grid[row * BOARD_COLS + col] = cell;
      }
    }
  }
}

bool noEmptySpacesInRow(const board_t *board, uint8_t row) {
  for (uint16_t i = BOARD_COLS * row;
       i < BOARD_COLS * row +  BOARD_COLS; ++i) {
    if (board->grid[i] == EMPTY_CELL) {
      return false;
    }
  }
  return true;
}

bool markDeadRows(board_t *board) {
  bool found = false;
  for (int row = 0; row < BOARD_ROWS; ++row) {
    if (noEmptySpacesInRow(board, row)) {
      found = true;
      fillRowWith(board, row, DEAD_CELL);
    }
  }
  return found;
}

uint8_t removeDeadRows(board_t *board) {
  uint8_t removed = 0;
  // If the first character in the row is dead, the whole row is dead.
  for (uint16_t row = 0; row < BOARD_ROWS; ++row) {
    if (board->grid[row * BOARD_COLS] == DEAD_CELL) {
      ++removed;

      // Take all rows above and move them down one.
      for (uint16_t i = row * BOARD_COLS + BOARD_COLS - 1;
           i >= BOARD_COLS; --i) {
        board->grid[i] = board->grid[i - BOARD_COLS];
      }

      // Fill in a blank first row.
      for (uint16_t i = 0; i < BOARD_COLS; ++i) {
        board->grid[i] = EMPTY_CELL;
      }
    }
  }
  return removed;
}

