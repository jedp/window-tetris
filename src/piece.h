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

class Piece {
  public:
    Piece();
    void generateFromShapes(const char **shapes, int rows, int cols);
    Shape getCurrentShape();
    void setCoordinates(point_t coordinates);
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

