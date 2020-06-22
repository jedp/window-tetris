#pragma once

#include "piece.h"

/**
 * Returns true if the given piece can be placed on the board at the given
 * point.
 *
 * Placement is invalid if any non-empty parts of the piece are off the board,
 * or overlap with a non-empty space on the board.
 *
 * The destination point is in board space. It may have negative coordinates.
 */
bool inBounds(Shape board, Shape shape, point_t dst);

