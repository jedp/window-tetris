#pragma once

#include <stdint.h>

/**
 * Container for coordinates.
 *
 * Top and left are (0, 0). Bottom and right are not less than top and left.
 */
typedef struct coords {
  uint8_t top;
  uint8_t left;
  uint8_t bottom;
  uint8_t right;
} coords_t;

typedef struct shape_t {
  uint8_t rows;
  uint8_t cols;
  coords_t bbox;
  const char *grid;
} shape_t;

/**
 * Return whether the designated row in the grid is empty.
 */
bool rowEmpty(const char *grid, int rows, int cols, int row);

/**
 * Return whether the designated col in the grid is empty.
 */
bool colEmpty(const char *grid, int rows, int cols, int col);

/**
 * Set value of coordinates according to the bounding box of the grid
 * with given number of rows and cols.
 *
 * The coordinates are inclusive, so a full 4x4 shape will have bounds of
 * (0, 0) to (3, 3).
 */
void boundingBox(const char *grid, int rows, int cols, coords_t &coords);

/**
 * Convert the given char grid of rows by cols to a shape_t.
 */
void shapeFromChars(const char *grid, int rows, int cols, shape_t &shape);
