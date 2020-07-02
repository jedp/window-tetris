#include <pieces.h>

orientation_t nextClockwise(orientation_t from) {
  return orientation_t((from + 1) % NUM_ORIENTATIONS);
}

orientation_t nextAntiClockwise(orientation_t from) {
  return orientation_t((from + 3) % NUM_ORIENTATIONS);
}

void initShape(shape_t *shape, shape_name_t name) {
  shape->name = name;
  shape->orientation = UP;
  shape->location.row = START_ROW;
  shape->location.col = START_COL;

  updateGrid(shape);
}

void rotateShape(shape_t *shape) {
  shape->orientation = nextClockwise(shape->orientation);
  updateGrid(shape);
}

void updateGrid(shape_t *shape) {
  shape->grid = shapeGrids +
    (shape->name * PIECE_EDGE + shape->orientation) * PIECE_CELLS;
}

