#pragma once

#include <stdint.h>

/**
 * A point designted by a row and a column.
 */
typedef struct point {
  int row;
  int col;
} point_t;

/**
 * Container for bounding box.
 */
typedef struct bbox {
  point_t uLeft;
  point_t lRight;
} bbox_t;

/**
 * Data class for a shape.
 */
typedef struct shape_t {
  uint8_t rows;
  uint8_t cols;
  bbox_t bbox;
  char *grid; // Mutable
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
void boundingBox(const char *grid, int rows, int cols, bbox_t &bbox);

/**
 * Convert the given char grid of rows by cols to a shape_t.
 */
void shapeFromChars(const char *grid, int rows, int cols, shape_t &shape);

/**
 * Return true if the shapes are the same. False otherwise.
 */
bool shapesEqual(const shape_t shape1, const shape_t shape2);

