#ifndef SRC_BOARD_H_
#define SRC_BOARD_H_

#include <pieces.h>

static const uint16_t BOARD_ROWS = 20;
static const uint16_t BOARD_COLS = 10;
static const uint16_t BOARD_CELLS = 200;

static const point_t START_POINT = { 0, 3 };

typedef struct {
  char grid[BOARD_CELLS];
} board_t;

void initBoard(board_t *board);

void fillWith(board_t *board, char c);

void setGrid(board_t *board, const char *grid);

void fillRowWith(board_t *board, uint8_t row, char c);

bool inside(const board_t *board,
            const shape_name_t shapeName,
            const orientation_t orientation,
            const point_t *dst);

bool collides(const board_t *board,
              const shape_name_t shapeName,
              const orientation_t orientation,
              const point_t *dst);

void stick(board_t *board, const shape_t *shape, const point_t *dst);

bool noEmptySpacesInRow(const board_t *board, uint8_t row);

bool markDeadRows(board_t *board);

uint8_t removeDeadRows(board_t *board);

#endif  // SRC_BOARD_H_

