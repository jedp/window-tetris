#ifndef SRC_PIECES_H_
#define SRC_PIECES_H_

#include <stdbool.h>
#include <stdint.h>

bool rowEmpty(const char *grid, int rows, int cols, int row);

bool colEmpty(const char *grid, int rows, int cols, int col);

void boundingBox(const char *grid, int rows, int cols);

// Reference for pieces: https://tetris.fandom.com/wiki/Orientation
static const char *shapeGrids =
  "    IIII        "
  "  I   I   I   I "
  "        IIII    "
  " I   I   I   I  "

  "J   JJJ         "
  " JJ  J   J      "
  "     JJJ  J     "
  " J   J  JJ      "

  "  L LLL         "
  " L   L   LL     "
  "    LLL L       "
  "LL   L   L      "

  "     OO  OO     "
  "     OO  OO     "
  "     OO  OO     "
  "     OO  OO     "

  " SS SS          "
  " S   SS   S     "
  "     SS SS      "
  "S   SS   S      "

  " T  TTT         "
  "T   TT  T       "
  "    TTT  T      "
  " T  TT   T      "

  "ZZ   ZZ         "
  "  Z  ZZ  Z      "
  "    ZZ   ZZ     "
  " Z  ZZ  Z       ";


static const uint8_t PIECE_EDGE = 4;
static const uint8_t PIECE_CELLS = 16;

static const char EMPTY_CELL = ' ';
static const char DEAD_CELL = 'X';

static const uint8_t START_ROW = 0;
static const uint8_t START_COL = 3;

typedef enum {
  I = 0,
  J,
  L,
  O,
  S,
  T,
  Z,
  NUM_SHAPES
} shape_name_t;

typedef enum {
  UP = 0,
  RIGHT,
  DOWN,
  LEFT,
  NUM_ORIENTATIONS
} orientation_t;

// Coordinates can be positive or negative.
typedef struct {
  int16_t row;
  int16_t col;
} point_t;

typedef struct {
  int16_t top;
  int16_t left;
  int16_t bottom;
  int16_t right;
} bbox_t;

// BBoxes for all the shapes.
static const bbox_t bboxes[28] = {
  (bbox_t) { 1, 0, 1, 3 },  // I UP
  (bbox_t) { 0, 2, 3, 2 },  // I RIGHT
  (bbox_t) { 2, 0, 2, 3 },  // I DOWN
  (bbox_t) { 0, 1, 3, 1 },  // I LEFT
  (bbox_t) { 0, 0, 1, 2 },  // J UP
  (bbox_t) { 0, 1, 2, 2 },  // Etc.
  (bbox_t) { 1, 0, 2, 2 },
  (bbox_t) { 0, 0, 2, 1 },
  (bbox_t) { 0, 0, 1, 2 },
  (bbox_t) { 0, 1, 2, 2 },
  (bbox_t) { 1, 0, 2, 2 },
  (bbox_t) { 0, 0, 2, 1 },
  (bbox_t) { 1, 1, 2, 2 },
  (bbox_t) { 1, 1, 2, 2 },
  (bbox_t) { 1, 1, 2, 2 },
  (bbox_t) { 1, 1, 2, 2 },
  (bbox_t) { 0, 0, 1, 2 },
  (bbox_t) { 0, 1, 2, 2 },
  (bbox_t) { 1, 0, 2, 2 },
  (bbox_t) { 0, 0, 2, 1 },
  (bbox_t) { 0, 0, 1, 2 },
  (bbox_t) { 0, 0, 2, 1 },
  (bbox_t) { 1, 0, 2, 2 },
  (bbox_t) { 0, 0, 2, 1 },
  (bbox_t) { 0, 0, 1, 2 },
  (bbox_t) { 0, 1, 2, 2 },
  (bbox_t) { 1, 0, 2, 2 },
  (bbox_t) { 0, 0, 2, 1 }
};

typedef struct {
  shape_name_t name;
  orientation_t orientation;
  point_t location;
  // Pointer to the current shape/orientation in the shapeGrids.
  const char *grid;
} shape_t;

orientation_t nextClockwise(orientation_t orientation);

orientation_t nextAntiClockwise(orientation_t orientation);

void initShape(shape_t *shape, shape_name_t name);

void rotateShape(shape_t *shape);

void updateGrid(shape_t *shape);

#endif  // SRC_PIECES_H_

