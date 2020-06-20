#include <stdbool.h>
#include <stdint.h>
#include "geom.h"
#include "piece.h"

orientation_t nextClockwiseOrientation(orientation_t from) {
  return orientation_t((from + 1) % 4);
}

orientation_t nextAntiClockwiseOrientation(orientation_t from) {
  return orientation_t((from + 3) % 4);
}

Piece::Piece()
  : coordinates((struct point) { 0, 0 })
{
}

void Piece::generateFromShapes(const char **shapes, int rows, int cols) {
  for (uint8_t orientation = UP; orientation <= LEFT; ++orientation) {
    Shape shape;
    shape.setFromChars(shapes[orientation], rows, cols);
    this->shapes[orientation] = shape;
  }
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

