#include <stdint.h>
#include "geom.h"
#include "board.h"

uint8_t Shape::getRows() {
  return rows;
}

uint8_t Shape::getCols() {
  return cols;
}

bbox_t Shape::getBBox() {
  return bbox;
}

char* Shape::getGrid() {
  return grid;
}

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

