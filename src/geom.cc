#include <stdint.h>
#include "geom.h"
#include "board.h"

void Shape::setWithChar(char c, int rows, int cols) {
  this->rows = rows;
  this->cols = cols;
  this->grid = new char[rows * cols]();

  for (uint8_t i = 0; i < rows * cols; ++i) {
    this->grid[i] = c;
  }

  updateBoundingBox();
}

void Shape::setFromChars(const char *chars, int rows, int cols) {
  this->rows = rows;
  this->cols = cols;
  this->grid = new char[rows * cols]();

  for (uint8_t i = 0; i < rows * cols; ++i) {
    this->grid[i] = chars[i];
  }

  updateBoundingBox();
}

void Shape::setCharAt(char c, point_t coordinates) {
  grid[coordinates.row * cols + coordinates.col] = c;

  updateBoundingBox();
}

void Shape::fillWith(char fillChar) {
  for (uint8_t i = 0; i < rows * cols; ++i) {
    grid[i] = fillChar;
  }

  updateBoundingBox();
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

  for (int i = dst.row; i < (int)rows && i < (int)other.getRows() + dst.row; ++i) {
    if (i < 0) continue;

    for (int j = dst.col; j < (int)cols && j < (int)other.getCols() + dst.col; ++j) {
      if (j < 0) continue;

      // i and j are in our coordinate space.
      // Offset to get the local coordinate in the shape.
      if (other.getGrid()[(i - dst.row) * other.getCols() + (j - dst.col)] != ' '
          && grid[i * cols + j] != ' ') {
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
void Shape::drop(const Shape &other, point_t dst) {
  for (uint8_t i = dst.row; i < (int)rows && i < (int)other.getRows() + dst.row; ++i) {
    if (i < 0) continue;

    for (uint8_t j = dst.col; j < (int)cols && j < (int)other.getCols() + dst.col; ++j) {
      if (j < 0) continue;

      char cell = other.getGrid()[(i - dst.row) * other.getCols() + (j - dst.col)];
      if (cell != ' ') {
        grid[i * cols + j] = cell;
      }
    }
  }

  updateBoundingBox();
}

bool Shape::operator==(const Shape &other) {
  if (rows != other.rows) return false;
  if (cols != other.cols) return false;

  for (uint8_t i = 0; i < rows * cols; ++i) {
    if (grid[i] != other.grid[i]) {
      return false;
    }
  }
  return true;
}

void Shape::updateBoundingBox() {
  boundingBox(grid, rows, cols, bbox);
}

bool rowEmpty(const char *grid, int rows, int cols, int row) {
  for (uint8_t col = 0; col < cols; ++col) {
    if (grid[row * cols + col] != ' ') {
      return false;
    }
  }
  return true;
}

bool colEmpty(const char *grid, int rows, int cols, int col) {
  for (uint8_t row = 0; row < rows; ++row) {
    if (grid[row * cols + col] != ' ') {
      return false;
    }
  }
  return true;
}

void boundingBox(const char *grid, int rows, int cols, bbox_t &bbox) {
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

