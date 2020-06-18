#include <board.h>
#include <piece.h>
#include <geom.h>
#include <game.h>
#include <constants.h>
#include <unity.h>

const char *board_empty_grid =
  "          "
  "          "
  "          "
  "          "
  "          "
  "          "
  "          "
  "          "
  "          "
  "          "
  "          "
  "          "
  "          "
  "          "
  "          "
  "          "
  "          "
  "          "
  "          "
  "          ";

const char *board0_grid =
  "          "
  "          "
  "          "
  "          "
  "          "
  "          "
  "          "
  "          "
  "          "
  "          "
  "          "
  "T  OOS    "
  "TTZOOSS   "
  "TZZTZZS   "
  "SZLTTZZL  "
  "SSLTOOJL  "
  "JSLLOOJLL "
  "JJJLTJJZI "
  "ILLLTTZZI "
  "IOOSTIZZI ";

const char *board1_grid =
  "          "
  "          "
  "          "
  "          "
  "          "
  "          "
  "          "
  "          "
  "          "
  "          "
  "          "
  "T  OOS    "
  "TTZOOSS   "
  "TZZTZZS   "
  "SZLTTZZL  "
  "SSLTOOJL  "
  "JSLLOOJLLI"
  "JJJLTJJZII"
  "ILLLTTZZII"
  "IOOSTIZZII";

void test_empty_rows(void) {
  int start = -2;
  int end = -2;
  getFullRowsRange(board0_grid, start, end);
  TEST_ASSERT_EQUAL(-1, start);
  TEST_ASSERT_EQUAL(-1, end);
}

void test_non_empty_rows(void) {
  int start = -2;
  int end = -2;
  getFullRowsRange(board1_grid, start, end);
  TEST_ASSERT_EQUAL(160, start);
  TEST_ASSERT_EQUAL(200, end);
}

void test_row_empty(void) {
  // Simple case: One row.
  TEST_ASSERT_TRUE(rowEmpty("    ", 1, 4, 0));
  TEST_ASSERT_FALSE(rowEmpty(" x  ", 1, 4, 0));

  // Simple case: Three rows.
  TEST_ASSERT_TRUE(rowEmpty("            ", 3, 4, 2));
  TEST_ASSERT_FALSE(rowEmpty("           x", 3, 4, 2));

  // Complex case.
  int rows = 4;
  int cols = 5;
  const char *grid = "     "
                     " XX  "
                     " XXX "
                     "     ";
  TEST_ASSERT_TRUE(rowEmpty(grid, rows, cols, 0));
  TEST_ASSERT_FALSE(rowEmpty(grid, rows, cols, 1));
  TEST_ASSERT_FALSE(rowEmpty(grid, rows, cols, 2));
  TEST_ASSERT_TRUE(rowEmpty(grid, rows, cols, 3));
}

void test_col_empty(void) {
  // Simple case: One col.
  TEST_ASSERT_TRUE(colEmpty("    ", 4, 1, 0));
  TEST_ASSERT_FALSE(colEmpty("   x", 4, 1, 0));

  // Simple case: Three cols.
  TEST_ASSERT_TRUE(colEmpty("            ", 4, 3, 2));
  TEST_ASSERT_FALSE(colEmpty("           x", 4, 3, 2));

  // Complex case.
  int rows = 4;
  int cols = 5;
  const char *grid = "     "
                     " XX  "
                     " XXX "
                     "     ";
  TEST_ASSERT_TRUE(colEmpty(grid, rows, cols, 0));
  TEST_ASSERT_FALSE(colEmpty(grid, rows, cols, 1));
  TEST_ASSERT_FALSE(colEmpty(grid, rows, cols, 2));
  TEST_ASSERT_FALSE(colEmpty(grid, rows, cols, 3));
  TEST_ASSERT_TRUE(colEmpty(grid, rows, cols, 4));
}

void test_rotate_clockwise(void) {
  TEST_ASSERT_EQUAL(RIGHT, rotateClockwise(UP));
  TEST_ASSERT_EQUAL(DOWN, rotateClockwise(RIGHT));
  TEST_ASSERT_EQUAL(LEFT, rotateClockwise(DOWN));
  TEST_ASSERT_EQUAL(UP, rotateClockwise(LEFT));
}

void test_rotate_anticlockwise(void) {
  TEST_ASSERT_EQUAL(LEFT, rotateAntiClockwise(UP));
  TEST_ASSERT_EQUAL(DOWN, rotateAntiClockwise(LEFT));
  TEST_ASSERT_EQUAL(RIGHT, rotateAntiClockwise(DOWN));
  TEST_ASSERT_EQUAL(UP, rotateAntiClockwise(RIGHT));
}

void test_bounding_box_empty_grid(void) {
  coords_t coords;
  int rows = 3;
  int cols = 4;
  const char *grid = "    "
                     "    "
                     "    ";
  boundingBox(grid, rows, cols, coords);
  // An empty thing has a zero-size bounding box.
  TEST_ASSERT_EQUAL(0, coords.top);
  TEST_ASSERT_EQUAL(0, coords.left);
  TEST_ASSERT_EQUAL(0, coords.bottom);
  TEST_ASSERT_EQUAL(0, coords.right);
}

void test_bounding_box(void) {
  coords_t coords;
  int rows = 4;
  int cols = 5;
  const char *grid = "     "
                     " xxx "
                     "  xxx"
                     "     ";
  boundingBox(grid, rows, cols, coords);
  TEST_ASSERT_EQUAL(1, coords.top);
  TEST_ASSERT_EQUAL(1, coords.left);
  TEST_ASSERT_EQUAL(2, coords.bottom);
  TEST_ASSERT_EQUAL(4, coords.right);
}

void test_generate_shapes(void) {
  const char *shapes_J[] = {
    [UP] =    "J   JJJ         ",
    [RIGHT] = " JJ  J   J      ",
    [DOWN] =  "    JJJ   J     ",
    [LEFT] =  " J   J  JJ      "
  };
  int rows = 4;
  int cols = 4;
  piece_t piece;
  generateFromShapes(shapes_J, rows, cols, piece);

  TEST_ASSERT_EQUAL(4, piece.rows);
  TEST_ASSERT_EQUAL(4, piece.cols);

  TEST_ASSERT_EQUAL(4, piece.shapes[UP].rows);
  TEST_ASSERT_EQUAL(4, piece.shapes[UP].cols);
  TEST_ASSERT_EQUAL(0, piece.shapes[UP].bbox.top);
  TEST_ASSERT_EQUAL(2, piece.shapes[UP].bbox.right);
  TEST_ASSERT_EQUAL(1, piece.shapes[UP].bbox.bottom);
  TEST_ASSERT_EQUAL(0, piece.shapes[UP].bbox.left);

  TEST_ASSERT_EQUAL(4, piece.shapes[RIGHT].rows);
  TEST_ASSERT_EQUAL(4, piece.shapes[RIGHT].cols);
  TEST_ASSERT_EQUAL(0, piece.shapes[RIGHT].bbox.top);
  TEST_ASSERT_EQUAL(2, piece.shapes[RIGHT].bbox.right);
  TEST_ASSERT_EQUAL(2, piece.shapes[RIGHT].bbox.bottom);
  TEST_ASSERT_EQUAL(1, piece.shapes[RIGHT].bbox.left);

  TEST_ASSERT_EQUAL(4, piece.shapes[DOWN].rows);
  TEST_ASSERT_EQUAL(4, piece.shapes[DOWN].cols);
  TEST_ASSERT_EQUAL(1, piece.shapes[DOWN].bbox.top);
  TEST_ASSERT_EQUAL(2, piece.shapes[DOWN].bbox.right);
  TEST_ASSERT_EQUAL(2, piece.shapes[DOWN].bbox.bottom);
  TEST_ASSERT_EQUAL(0, piece.shapes[DOWN].bbox.left);

  TEST_ASSERT_EQUAL(4, piece.shapes[LEFT].rows);
  TEST_ASSERT_EQUAL(4, piece.shapes[LEFT].cols);
  TEST_ASSERT_EQUAL(0, piece.shapes[LEFT].bbox.top);
  TEST_ASSERT_EQUAL(1, piece.shapes[LEFT].bbox.right);
  TEST_ASSERT_EQUAL(2, piece.shapes[LEFT].bbox.bottom);
  TEST_ASSERT_EQUAL(0, piece.shapes[LEFT].bbox.left);
}

void test_valid_placement(void) {
  const char *shapes_J[] = {
    [UP] =    "J   JJJ         ",
    [RIGHT] = " JJ  J   J      ",
    [DOWN] =  "    JJJ   J     ",
    [LEFT] =  " J   J  JJ      "
  };
  int rows = 4;
  int cols = 4;
  piece_t piece;
  generateFromShapes(shapes_J, rows, cols, piece);

  shape_t grid;
  shapeFromChars(board_empty_grid, 20, 10, grid);

  TEST_ASSERT_TRUE(validPlacement(grid, piece.shapes[UP], 0, 0));
  TEST_ASSERT_TRUE(validPlacement(grid, piece.shapes[UP], 0, 8));
  TEST_ASSERT_TRUE(validPlacement(grid, piece.shapes[UP], 19, 0));
  TEST_ASSERT_TRUE(validPlacement(grid, piece.shapes[UP], 19, 8));

  TEST_ASSERT_FALSE(validPlacement(grid, piece.shapes[UP], -1, 0));
  TEST_ASSERT_FALSE(validPlacement(grid, piece.shapes[UP], 0, 9));
  TEST_ASSERT_FALSE(validPlacement(grid, piece.shapes[UP], 20, 0));
  TEST_ASSERT_FALSE(validPlacement(grid, piece.shapes[UP], 18, 9));
}

void test_collide(void) {
  const char *board_chars = "    "
                            "    "
                            "    "
                            "xxx "
                            "xx  "
                            "xx  ";
  const char *shape_chars = " o"
                            " o";
  shape_t board;
  shape_t shape;
  shapeFromChars(board_chars, 6, 4, board);
  shapeFromChars(shape_chars, 2, 2, shape);

  // Valid, all-on-screen placement.
  TEST_ASSERT_FALSE(collide(board, shape, 0, 0));

  // Part of the shape is off the screen. That's ok.
  TEST_ASSERT_FALSE(collide(board, shape, 0, -1));
  TEST_ASSERT_FALSE(collide(board, shape, -1, -1));

  // Bottom tip overlaps.
  TEST_ASSERT_TRUE(collide(board, shape, 2, 0));

  // Top overlaps.
  TEST_ASSERT_TRUE(collide(board, shape, 3, 1));

  // Top and bottom overlap.
  TEST_ASSERT_TRUE(collide(board, shape, 4, -1));
  TEST_ASSERT_TRUE(collide(board, shape, 4, 0));

  // Hidden under the lip of the thing.
  TEST_ASSERT_FALSE(collide(board, shape, 4, 1));
}

int main(int argc, char** argv) {
  UNITY_BEGIN();

  RUN_TEST(test_empty_rows);
  RUN_TEST(test_non_empty_rows);

  RUN_TEST(test_rotate_clockwise);
  RUN_TEST(test_rotate_anticlockwise);

  RUN_TEST(test_row_empty);
  RUN_TEST(test_col_empty);
  RUN_TEST(test_bounding_box_empty_grid);
  RUN_TEST(test_bounding_box);

  RUN_TEST(test_generate_shapes);
  RUN_TEST(test_valid_placement);
  RUN_TEST(test_collide);

  UNITY_END();
}

