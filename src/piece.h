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
typedef enum PieceName {
  I,
  J,
  L,
  O,
  S,
  T,
  Z,
  NUM_PIECES,
} piece_name_t;

typedef struct piece_t {
  point_t coordinates;
  orientation_t currentShape;
  shape_t shapes[4];
} piece_t;

orientation_t rotateClockwise(orientation_t from);

orientation_t rotateAntiClockwise(orientation_t from);

void generateFromShapes(const char **shapes, int rows, int cols, piece_t &piece);

