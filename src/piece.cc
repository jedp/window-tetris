#include <stdbool.h>
#include <stdint.h>
#include "geom.h"
#include "piece.h"

orientation_t rotateClockwise(orientation_t from) {
  return orientation_t((from + 1) % 4);
}

orientation_t rotateAntiClockwise(orientation_t from) {
  return orientation_t((from + 3) % 4);
}

void generateFromShapes(const char **shapes, int rows, int cols, piece_t &piece) {
  for (uint8_t orientation = UP; orientation <= LEFT; ++orientation) {
    shape_t shape;
    shapeFromChars(shapes[orientation], rows, cols, shape);
    piece.shapes[orientation] = shape;
  }
}

