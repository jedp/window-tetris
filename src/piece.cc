#include <stdbool.h>
#include <stdint.h>
#include "piece.h"

orientation_t rotateClockwise(orientation_t from) {
  return orientation_t((from + 1) % 4);
}

orientation_t rotateAntiClockwise(orientation_t from) {
  return orientation_t((from + 3) % 4);
}

void generateFromShapes(const char **shapes, int rows, int cols, piece_t &piece) {
  piece.rows = rows;
  piece.cols = cols;

  for (uint8_t orientation = UP; orientation <= LEFT; ++orientation) {
    shape_t shape;
    coords_t bbox;
    shape.rows = rows;
    shape.cols = cols;
    shape.grid = shapes[orientation];

    boundingBox(shape.grid, rows, cols, bbox);
    shape.bbox = bbox;

    piece.shapes[orientation] = shape;
  }
}

