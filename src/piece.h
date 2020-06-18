#pragma once

#include <stdint.h>
#include <geom.h>

typedef enum {
  UP,
  RIGHT,
  DOWN,
  LEFT,
} orientation_t;

/**
 * The names of the seven tiles.
 */
typedef enum {
  I,
  J,
  L,
  O,
  S,
  T,
  Z,
} piece_name_t;

typedef struct piece_t {
  uint8_t rows;
  uint8_t cols;
  shape_t shapes[4];
} piece_t;

orientation_t rotateClockwise(orientation_t from);

orientation_t rotateAntiClockwise(orientation_t from);

void generateFromShapes(const char **shapes, int rows, int cols, piece_t &piece);

