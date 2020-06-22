#include <shape.h>
#include <piece.h>

Shape::Shape(int rows, int cols, const char *chars)
: rows(rows),
  cols(cols),
  grid(new char[rows * cols + 1]()) {
  for (int i = 0; i < rows * cols; ++i) {
    this->grid[i] = chars[i];
  }
  // In case we ever want to print this, make it a nice string.
  this -> grid[rows * cols] = '\0';

  updateBoundingBox();
}

void Shape::fillWithChars(const char *chars) {
  for (int i = 0; i < rows * cols; ++i) {
    this->grid[i] = chars[i];
  }

  updateBoundingBox();
}

void Shape::setCharAt(char c, point_t coordinates) {
  grid[coordinates.row * cols + coordinates.col] = c;

  updateBoundingBox();
}

void Shape::fillWith(char fillChar) {
  for (int i = 0; i < rows * cols; ++i) {
    grid[i] = fillChar;
  }

  updateBoundingBox();
}

void Shape::fillRowWith(char fillChar, int row) {
  for (int i = row * cols; i < row * cols + cols; ++i) {
    grid[i] = fillChar;
  }
}

bool Shape::findAndMarkRowsForRemoval() {
  int found = false;
  for (int row = 0; row < rows; ++row) {
    if (noEmptySpacesInRow(row)) {
      fillRowWith(DEAD_CELL, row);
      found = true;
    }
  }
  return found;
}

int Shape::removeRowsMarkedForRemoval() {
  int removed = 0;
  // If the first cell in the row is an X, the row can be removed.
  for (int row = 0; row < rows; ++row) {
    if (grid[row * cols] == DEAD_CELL) {
      ++removed;
      // Take all rows 0 .. row-1 and move them down one.
      for (int i = row * cols + cols -1; i >= cols; --i) {
        grid[i] = grid[i - cols];
      }
      // Fill in a blank first row.
      for (int i = 0; i < cols; ++i) {
        grid[i] = EMPTY_SPACE;
      }
    }
  }
  return removed;
}

bool Shape::noEmptySpacesInRow(int row) {
  for (int i = cols * row; i < cols * row + cols; ++i) {
    if (grid[i] == ' ') {
      return false;
    }
  }
  return true;
}

bool Shape::within(bbox_t other, point_t dst) {
  if (bbox.uLeft.row + dst.row < other.uLeft.row) return false;
  if (bbox.uLeft.col + dst.col < other.uLeft.col) return false;
  if (bbox.lRight.col + dst.col > other.lRight.col) return false;
  if (bbox.lRight.row + dst.row > other.lRight.row) return false;
  return true;
}

/**
 * Return true if the other shape would collide with us if place at point
 * dst in our coordinate space; false otherwise.
 */
bool Shape::collides(const Shape &other, point_t dst) {
  if (dst.row >= rows) return false;
  if (dst.col >= cols) return false;

  // If it's above the our bounding box, don't do any hard work.
  if (dst.row + other.getRows() < bbox.uLeft.row) return false;

  for (int i = dst.row; i < rows && i < other.getRows() + dst.row; ++i) {
    if (i < 0) continue;

    for (int j = dst.col; j < cols && j < other.getCols() + dst.col; ++j) {
      if (j < 0) continue;

      // i and j are in our coordinate space.
      // Offset to get the local coordinate in the shape.
      if (other.getGrid()[(i - dst.row) * other.getCols() + (j - dst.col)]
          != EMPTY_SPACE
          && grid[i * cols + j] != EMPTY_SPACE) {
        return true;
      }
    }
  }
  return false;
}

/**
 * Stick the shape to the board at the given point.
 *
 * The destination point is in board space.
 */
void Shape::stick(const Shape &other, point_t dst) {
  for (int i = dst.row; i < rows && i < other.getRows() + dst.row; ++i) {
    if (i < 0) continue;

    for (int j = dst.col; j < cols && j < other.getCols() + dst.col; ++j) {
      if (j < 0) continue;

      char cell = other
        .getGrid()[(i - dst.row) * other.getCols() + (j - dst.col)];
      if (cell != EMPTY_SPACE) {
        grid[i * cols + j] = cell;
      }
    }
  }

  updateBoundingBox();
}

bool Shape::operator==(const Shape &other) {
  if (rows != other.rows) return false;
  if (cols != other.cols) return false;

  for (int i = 0; i < rows * cols; ++i) {
    if (grid[i] != other.grid[i]) {
      return false;
    }
  }
  return true;
}

void Shape::updateBoundingBox() {
  uint8_t _top = 0;
  uint8_t _left = 0;
  uint8_t _bottom = 0;
  uint8_t _right = 0;

  for (uint8_t row = 0; row < rows; ++row) {
    if (!rowEmpty(grid, rows, cols, row)) {
      _top = row;
      break;
    }
  }

  for (uint8_t row = rows; row > 0; --row) {
    if (!rowEmpty(grid, rows, cols, row - 1)) {
      _bottom = row - 1;
      break;
    }
  }

  for (uint8_t col = 0; col < cols; ++col) {
    if (!colEmpty(grid, rows, cols, col)) {
      _left = col;
      break;
    }
  }

  for (uint8_t col = cols; col > 0; --col) {
    if (!colEmpty(grid, rows, cols, col - 1)) {
      _right = col - 1;
      break;
    }
  }

  bbox.uLeft.row = _top;
  bbox.uLeft.col = _left;
  bbox.lRight.row = _bottom;
  bbox.lRight.col = _right;
}

bool rowEmpty(const char *grid, int rows, int cols, int row) {
  for (int col = 0; col < cols; ++col) {
    if (grid[row * cols + col] != ' ') {
      return false;
    }
  }
  return true;
}

bool colEmpty(const char *grid, int rows, int cols, int col) {
  for (int row = 0; row < rows; ++row) {
    if (grid[row * cols + col] != ' ') {
      return false;
    }
  }
  return true;
}

