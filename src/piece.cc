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
    int rows,
    int cols,
    Shape up,
    Shape right,
    Shape down,
    Shape left)
  : rows(rows),
    cols(cols),
    shapes { up, right, down, left },
    orientation(UP),
    coordinates((struct point) { 0, 0 })
{
}

Shape Piece::getCurrentShape() {
  return shapes[orientation];
}

void Piece::setCoordinates(point_t coordinates) {
  this->coordinates = coordinates;
}

point_t Piece::getCoordinates() {
  return coordinates;
}

Shape Piece::shapeFacing(orientation_t orientation) {
  return shapes[orientation];
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

