#pragma once

#import "piece.h"

/**
 * Return true if the range of characters from start to end (exclusive)
 * contains no empty spaces.
 */
bool noEmptySpacesInRange(const char *board, int start, int end);

/**
 * Set start and end to the beginning and end character positions in board
 * of the rows that have no empty spaces in them.
 *
 * If there are no non-empty rows, start and end are set to -1.
 *
 * This assumes that all non-empty rows are contiguous, which is guaranteed
 * with the normal set of seven Tetris pieces.
 */
void getFullRowsRange(const char *board, int &start, int &end);

/**
 * Returns true if the given piece can be placed on the board at the given
 * row and col.
 *
 * Placement is invalid if any non-empty parts of the piece are off the board,
 * or overlap with a non-empty space on the board.
 *
 * Row and col may be negative.
 */
bool validPlacement(shape_t board, shape_t shape, int row, int col);

/**
 * Return true if any of the tiles in shape collide with tiles on the board
 * when shape is move to row, col.
 */
bool collide(shape_t board, shape_t shape, int row, int col);

/**
 * Stick the shape to the board at the given row and column.
 */
void stick(shape_t board, shape_t shape, int row, int col);

