#include <stdint.h>
#include "assert.h"
#include "geom.h"
#include "board.h"

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

void boundingBox(const char *grid, int rows, int cols, coords_t &coords) {
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

  coords.top = _top;
  coords.left = _left;
  coords.bottom = _bottom;
  coords.right = _right;
}

void shapeFromChars(const char *grid, int rows, int cols, shape_t &shape) {
  coords_t bbox;
  shape.rows = rows;
  shape.cols = cols;
  shape.grid = new char[rows * cols]();

  for (uint8_t i = 0; i < rows * cols; ++i) {
    shape.grid[i] = grid[i];
  }

  boundingBox(shape.grid, rows, cols, bbox);
  shape.bbox = bbox;
}

bool shapesEqual(const shape_t shape1, const shape_t shape2) {
  if (shape1.rows != shape2.rows) return false;
  if (shape1.cols != shape2.cols) return false;

  for (uint8_t i = 0; i < shape1.rows; ++i) {
   for (uint8_t j = 0; j < shape1.cols; ++j) {
      if (shape1.grid[i * shape1.cols + j] != shape2.grid[i * shape1.cols + j]) {
        return false;
      }
    }
  }
  return true;
}

