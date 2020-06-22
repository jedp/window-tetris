#include <stdbool.h>
#include <stdint.h>
#include <shape.h>
#include <piece.h>

orientation_t nextClockwiseOrientation(orientation_t from) {
  return orientation_t((from + 1) % NUM_ORIENTATIONS);
}

orientation_t nextAntiClockwiseOrientation(orientation_t from) {
  return orientation_t((from + 3) % NUM_ORIENTATIONS);
}

Piece::Piece(
    Shape up,
    Shape right,
    Shape down,
    Shape left)
  : coordinates((struct point) { 0, 0 }),
    orientation(UP),
    shapes { up, right, down, left }
{
}

Shape Piece::getCurrentShape() {
  return shapes[orientation];
}

void Piece::setCoordinates(point_t dst) {
  coordinates = dst;
}

point_t Piece::getCoordinates() {
  return coordinates;
}

Shape Piece::shapeFacing(orientation_t o) {
  return shapes[o];
}

Shape Piece::shapeForClockwiseRotation() {
  return shapes[nextClockwiseOrientation(orientation)];
}

Shape Piece::shapeForAntiClockwiseRotation() {
  return shapes[nextAntiClockwiseOrientation(orientation)];
}

void Piece::rotateClockwise() {
  orientation = nextClockwiseOrientation(orientation);
}

void Piece::rotateAntiClockwise() {
  orientation = nextAntiClockwiseOrientation(orientation);
}

