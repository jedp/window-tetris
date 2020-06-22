#pragma once

#include <stdint.h>
#include <shape.h>

const char EMPTY_SPACE = ' ';
const char DEAD_CELL = 'X';

typedef enum {
  UP,
  RIGHT,
  DOWN,
  LEFT,
  NUM_ORIENTATIONS,
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

class Piece {
  public:
    Piece(
        Shape up,
        Shape right,
        Shape down,
        Shape left);
    Shape getCurrentShape();
    void setCoordinates(point_t coordinates);
    void setOrientation(orientation_t o) { orientation = o; };
    point_t getCoordinates();
    Shape shapeFacing(orientation_t orientation);
    Shape shapeForClockwiseRotation();
    Shape shapeForAntiClockwiseRotation();
    void rotateClockwise();
    void rotateAntiClockwise();

  private:
    point_t coordinates;
    orientation_t orientation;
    Shape shapes[4];
};


orientation_t nextClockwiseOrientation(orientation_t from);

orientation_t nextAntiClockwiseOrientation(orientation_t from);

