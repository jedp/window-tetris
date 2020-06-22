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
    Shape(int rows, int cols, const char *chars);

    /** The number of rows in the shape. */
    int getRows() const { return rows; }

    /** The number of cols in the shape. */
    int getCols() const { return cols; }

    /**
     * The rendering of the shape as a grid of characters..
     *
     * Invisible pixels are represented as the space character. Visible
     * pixels will have the nme of a piece ('I', 'J', 'L', etc.) or 'X'
     * to signify that the block represented by the pixel is being destroyed.
     */
    char *getGrid() const { return grid; }

    /**
     * The bounding box of the visible pixes in the shape.
     *
     * This will be updated when the shape is mutated by a method that sets
     * its pixels.
     */
    bbox_t getBBox() const { return bbox; }

    /** Update a pixel in the shape. */
    void setCharAt(char c, point_t coordinates);

    /**
     * Fill the whole shape with a new grid of characters.
     *
     * Better make this the same size as the old grid!
     */
    void fillWithChars(const char *chars);

    /** Fill the whole shape with a single character. */
    void fillWith(char fillChar);

    /** Fill the designated row in the shape with a single character. */
    void fillRowWith(char fillChar, int row);

    /**
     * Find rows that are full and fill them with the 'X' characer.
     *
     * Return true if any full rows were found.
     */
    bool findAndMarkRowsForRemoval();

    /**
     * Find all rows containing 'X's and remove them.
     *
     * After removing rows, slide all pixels above the former rows down to
     * fill the newly created spaces.
     *
     * Return the number of rows removed.
     */
    int removeRowsMarkedForRemoval();

    /** Return true if there are no empty spaces in this row. */
    bool noEmptySpacesInRow(int row);

    /**
     * Return true if the present shape's bounding box is entirely contained
     * within the given bbox when the present shape is affined to point dst.
     */
    bool within(bbox_t bbox, point_t dst);

    /**
     * Return true if the non-empty pixels in this shape touch any non-empty
     * pixels in the other shape when affined to point dst.
     */
    bool collides(const Shape &other, point_t dst);

    /**
     * Amend this shape by sticking pixels from other, affined to point dst.
     */
    void stick(const Shape &other, point_t dst);

    /**
     * Re-compute the bounding box.
     *
     * The coordinates are inclusive, so a full 4x4 shape will have bounds of
     * (0, 0) to (3, 3).
     *
     * A shape with all empty spaces will have a bounding box of 0 - 0.
     */
    void updateBoundingBox();

    /** Return true if both shapes contain the same grid pixels. */
    bool operator==(const Shape &other);

 private:
    int rows;
    int cols;
    char *grid;
    bbox_t bbox;
};

/**
 * Return whether the designated row in the grid is empty.
 */
bool rowEmpty(const char *grid, int rows, int cols, int row);

/**
 * Return whether the designated col in the grid is empty.
 */
bool colEmpty(const char *grid, int rows, int cols, int col);

