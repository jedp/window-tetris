#include <stdint.h>
#include "piece.h"
#include "board.h"
#include "game.h"

bool inBounds(Shape board, Shape shape, point_t dst) {
  if (dst.row - shape.getBBox().uLeft.row < 0) return false;
  if (dst.row > board.getRows() - shape.getBBox().lRight.row) return false;
  if (dst.col - shape.getBBox().uLeft.col < 0) return false;
  if (dst.col > board.getCols() - shape.getBBox().lRight.col) return false;

  return true;
}

