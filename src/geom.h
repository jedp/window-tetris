#pragma once

#include <stdint.h>

/**
 * A point designted by a row and a column.
 */
typedef struct point {
  int row;
  int col;
} point_t;

/**
 * Container for bounding box.
 */
typedef struct bbox {
  point_t uLeft;
  point_t lRight;
} bbox_t;

/**
 * Data class for a shape.
 */
class Shape {

  public:
    uint8_t getRows() const { return rows; }
    uint8_t getCols() const { return cols; }
    char *getGrid() const { return grid; }
    bbox_t getBBox() const { return bbox; }
    void setWithChar(char c, int rows, int cols);
    void setFromChars(const char *chars, int rows, int cols);
    void setCharAt(char c, point_t coordinates);
    void fillWith(char fillChar);
    bool collides(const Shape &other, point_t dst);
    void drop(Shape &other, point_t dst);
    void updateBoundingBox();
    bool operator==(const Shape &other);

  private:
    uint8_t rows;
    uint8_t cols;
    bbox_t bbox;
    char *grid;
};

/**
 * Return whether the designated row in the grid is empty.
 */
bool rowEmpty(const char *grid, int rows, int cols, int row);

/**
 * Return whether the designated col in the grid is empty.
 */
bool colEmpty(const char *grid, int rows, int cols, int col);

/**
 * Set value of coordinates according to the bounding box of the grid
 * with given number of rows and cols.
 *
 * The coordinates are inclusive, so a full 4x4 shape will have bounds of
 * (0, 0) to (3, 3).
 *
 * A shape with no non-empty spaces will have a bounding box of 0 - 0.
 */
void boundingBox(const char *grid, int rows, int cols, bbox_t &bbox);

