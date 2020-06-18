#include <board.h>
#include <piece.h>
#include <geom.h>
#include <game.h>
#include <constants.h>
#include <unity.h>

const char *board_empty =
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

const char *board0 =
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

const char *board1 =
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
  getFullRowsRange(board0, start, end);
  TEST_ASSERT_EQUAL(-1, start);
  TEST_ASSERT_EQUAL(-1, end);
}

void test_non_empty_rows(void) {
  int start = -2;
  int end = -2;
  getFullRowsRange(board1, start, end);
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

  TEST_ASSERT_EQUAL(0, piece.shapes[UP].bbox.top);
  TEST_ASSERT_EQUAL(2, piece.shapes[UP].bbox.right);
  TEST_ASSERT_EQUAL(1, piece.shapes[UP].bbox.bottom);
  TEST_ASSERT_EQUAL(0, piece.shapes[UP].bbox.left);

  TEST_ASSERT_EQUAL(0, piece.shapes[RIGHT].bbox.top);
  TEST_ASSERT_EQUAL(2, piece.shapes[RIGHT].bbox.right);
  TEST_ASSERT_EQUAL(2, piece.shapes[RIGHT].bbox.bottom);
  TEST_ASSERT_EQUAL(1, piece.shapes[RIGHT].bbox.left);

  TEST_ASSERT_EQUAL(1, piece.shapes[DOWN].bbox.top);
  TEST_ASSERT_EQUAL(2, piece.shapes[DOWN].bbox.right);
  TEST_ASSERT_EQUAL(2, piece.shapes[DOWN].bbox.bottom);
  TEST_ASSERT_EQUAL(0, piece.shapes[DOWN].bbox.left);

  TEST_ASSERT_EQUAL(0, piece.shapes[LEFT].bbox.top);
  TEST_ASSERT_EQUAL(1, piece.shapes[LEFT].bbox.right);
  TEST_ASSERT_EQUAL(2, piece.shapes[LEFT].bbox.bottom);
  TEST_ASSERT_EQUAL(0, piece.shapes[LEFT].bbox.left);
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

  UNITY_END();
}

