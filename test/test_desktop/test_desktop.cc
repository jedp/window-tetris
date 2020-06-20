#include <board.h>
#include <piece.h>
#include <geom.h>
#include <game.h>
#include <constants.h>
#include <sequence.h>
#include <unity.h>
#include <iostream>

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
  bbox_t bbox;
  int rows = 3;
  int cols = 4;
  const char *grid = "    "
                     "    "
                     "    ";
  boundingBox(grid, rows, cols, bbox);
  // An empty thing has a zero-size bounding box.
  TEST_ASSERT_EQUAL(0, bbox.uLeft.row);
  TEST_ASSERT_EQUAL(0, bbox.uLeft.col);
  TEST_ASSERT_EQUAL(0, bbox.lRight.row);
  TEST_ASSERT_EQUAL(0, bbox.lRight.col);
}

void test_bounding_box(void) {
  bbox_t bbox;
  int rows = 4;
  int cols = 5;
  const char *grid = "     "
                     " xxx "
                     "  xxx"
                     "     ";
  boundingBox(grid, rows, cols, bbox);
  TEST_ASSERT_EQUAL(1, bbox.uLeft.row);
  TEST_ASSERT_EQUAL(1, bbox.uLeft.col);
  TEST_ASSERT_EQUAL(2, bbox.lRight.row);
  TEST_ASSERT_EQUAL(4, bbox.lRight.col);
}

void test_update_bounding_box(void) {
  shape_t shape;
  const char *empty = "    "
                      "    "
                      "    ";
  shapeFromChars(empty, 3, 4, shape);
  TEST_ASSERT_EQUAL(0, shape.bbox.uLeft.row);
  TEST_ASSERT_EQUAL(0, shape.bbox.uLeft.col);
  TEST_ASSERT_EQUAL(0, shape.bbox.lRight.row);
  TEST_ASSERT_EQUAL(0, shape.bbox.lRight.col);

  // Put a shape inside.
  shape.grid[5] = 'x';
  shape.grid[6] = 'x';
  updateBoundingBox(shape);
  TEST_ASSERT_EQUAL(1, shape.bbox.uLeft.row);
  TEST_ASSERT_EQUAL(1, shape.bbox.uLeft.col);
  TEST_ASSERT_EQUAL(1, shape.bbox.lRight.row);
  TEST_ASSERT_EQUAL(2, shape.bbox.lRight.col);

  // Remove the shape. It's empty again.
  shape.grid[5] = ' ';
  shape.grid[6] = ' ';
  updateBoundingBox(shape);
  TEST_ASSERT_EQUAL(0, shape.bbox.uLeft.row);
  TEST_ASSERT_EQUAL(0, shape.bbox.uLeft.col);
  TEST_ASSERT_EQUAL(0, shape.bbox.lRight.row);
  TEST_ASSERT_EQUAL(0, shape.bbox.lRight.col);
}

void test_fill(void) {
  shape_t shape;
  shape_t fullShape;
  const char *empty = "    "
                      "    "
                      "    ";
  const char *full =  "cccc"
                      "cccc"
                      "cccc";

  shapeFromChars(empty, 3, 4, shape);
  shapeFromChars(full, 3, 4, fullShape);

  // Initially, the bbox is empty because the shape has no visible contents.
  TEST_ASSERT_EQUAL(4, shape.cols);
  TEST_ASSERT_EQUAL(3, shape.rows);
  TEST_ASSERT_EQUAL(0, shape.bbox.uLeft.col);
  TEST_ASSERT_EQUAL(0, shape.bbox.uLeft.row);
  TEST_ASSERT_EQUAL(0, shape.bbox.lRight.col);
  TEST_ASSERT_EQUAL(0, shape.bbox.lRight.row);
  TEST_ASSERT_FALSE(shapesEqual(shape, fullShape));

  fillShape(shape, 'c');
  // After filling, the bbox is updated.
  TEST_ASSERT_EQUAL(4, shape.cols);
  TEST_ASSERT_EQUAL(3, shape.rows);
  TEST_ASSERT_EQUAL(0, shape.bbox.uLeft.col);
  TEST_ASSERT_EQUAL(0, shape.bbox.uLeft.row);
  TEST_ASSERT_EQUAL(3, shape.bbox.lRight.col);
  TEST_ASSERT_EQUAL(2, shape.bbox.lRight.row);
  TEST_ASSERT_TRUE(shapesEqual(shape, fullShape));
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
  TEST_ASSERT_EQUAL(0, piece.shapes[UP].bbox.uLeft.row);
  TEST_ASSERT_EQUAL(2, piece.shapes[UP].bbox.lRight.col);
  TEST_ASSERT_EQUAL(1, piece.shapes[UP].bbox.lRight.row);
  TEST_ASSERT_EQUAL(0, piece.shapes[UP].bbox.uLeft.col);

  TEST_ASSERT_EQUAL(4, piece.shapes[RIGHT].rows);
  TEST_ASSERT_EQUAL(4, piece.shapes[RIGHT].cols);
  TEST_ASSERT_EQUAL(0, piece.shapes[RIGHT].bbox.uLeft.row);
  TEST_ASSERT_EQUAL(2, piece.shapes[RIGHT].bbox.lRight.col);
  TEST_ASSERT_EQUAL(2, piece.shapes[RIGHT].bbox.lRight.row);
  TEST_ASSERT_EQUAL(1, piece.shapes[RIGHT].bbox.uLeft.col);

  TEST_ASSERT_EQUAL(4, piece.shapes[DOWN].rows);
  TEST_ASSERT_EQUAL(4, piece.shapes[DOWN].cols);
  TEST_ASSERT_EQUAL(1, piece.shapes[DOWN].bbox.uLeft.row);
  TEST_ASSERT_EQUAL(2, piece.shapes[DOWN].bbox.lRight.col);
  TEST_ASSERT_EQUAL(2, piece.shapes[DOWN].bbox.lRight.row);
  TEST_ASSERT_EQUAL(0, piece.shapes[DOWN].bbox.uLeft.col);

  TEST_ASSERT_EQUAL(4, piece.shapes[LEFT].rows);
  TEST_ASSERT_EQUAL(4, piece.shapes[LEFT].cols);
  TEST_ASSERT_EQUAL(0, piece.shapes[LEFT].bbox.uLeft.row);
  TEST_ASSERT_EQUAL(1, piece.shapes[LEFT].bbox.lRight.col);
  TEST_ASSERT_EQUAL(2, piece.shapes[LEFT].bbox.lRight.row);
  TEST_ASSERT_EQUAL(0, piece.shapes[LEFT].bbox.uLeft.col);
}

void test_in_bounds(void) {
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

  TEST_ASSERT_TRUE(inBounds(grid, piece.shapes[UP], (struct point) { 0, 0 }));
  TEST_ASSERT_TRUE(inBounds(grid, piece.shapes[UP], (struct point) { 0, 8 }));
  TEST_ASSERT_TRUE(inBounds(grid, piece.shapes[UP], (struct point) { 19, 0 }));
  TEST_ASSERT_TRUE(inBounds(grid, piece.shapes[UP], (struct point) { 19, 8 }));

  TEST_ASSERT_FALSE(inBounds(grid, piece.shapes[UP], (struct point) { -1, 0 }));
  TEST_ASSERT_FALSE(inBounds(grid, piece.shapes[UP], (struct point) { 0, 9 }));
  TEST_ASSERT_FALSE(inBounds(grid, piece.shapes[UP], (struct point) { 20, 0 }));
  TEST_ASSERT_FALSE(inBounds(grid, piece.shapes[UP], (struct point) { 18, 9 }));
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
  TEST_ASSERT_FALSE(collide(board, shape, (struct point) { 0, 0 }));

  // Part of the shape is off the screen. That's ok.
  TEST_ASSERT_FALSE(collide(board, shape, (struct point) { 0, -1 }));
  TEST_ASSERT_FALSE(collide(board, shape, (struct point) { -1, -1 }));

  // Bottom tip overlaps.
  TEST_ASSERT_TRUE(collide(board, shape, (struct point) { 2, 0 }));

  // Top overlaps.
  TEST_ASSERT_TRUE(collide(board, shape, (struct point) { 3, 1 }));

  // Top and bottom overlap.
  TEST_ASSERT_TRUE(collide(board, shape, (struct point) { 4, -1 }));
  TEST_ASSERT_TRUE(collide(board, shape, (struct point) { 4, 0 }));

  // Hidden under the lip of the thing.
  TEST_ASSERT_FALSE(collide(board, shape, (struct point) { 4, 1 }));
}

void test_stick(void) {
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

  stick(board, shape, (struct point) { 4, 1 });

  // This is what it looks like after sticking.
  const char *stuck_chars = "    "
                            "    "
                            "    "
                            "xxx "
                            "xxo "
                            "xxo ";
  shape_t expected;
  shapeFromChars(stuck_chars, 6, 4, expected);
  TEST_ASSERT_TRUE(shapesEqual(board, expected));
}

void test_sequence(void) {
  Sequence sequence = Sequence(0);

  // Sequence is deterministic in tests.
  TEST_ASSERT_EQUAL(I, sequence.next());
  TEST_ASSERT_EQUAL(J, sequence.next());
  TEST_ASSERT_EQUAL(L, sequence.next());
  TEST_ASSERT_EQUAL(O, sequence.next());
  TEST_ASSERT_EQUAL(S, sequence.next());
  TEST_ASSERT_EQUAL(T, sequence.next());
  TEST_ASSERT_EQUAL(Z, sequence.next());

  // It resets and starts again.
  TEST_ASSERT_EQUAL(I, sequence.next());
  TEST_ASSERT_EQUAL(J, sequence.next());
  TEST_ASSERT_EQUAL(L, sequence.next());
  TEST_ASSERT_EQUAL(O, sequence.next());
  TEST_ASSERT_EQUAL(S, sequence.next());
  TEST_ASSERT_EQUAL(T, sequence.next());
  TEST_ASSERT_EQUAL(Z, sequence.next());
}

void test_make_canvas(void) {
  shape_t canvas;
  makeCanvas(canvas);

  TEST_ASSERT_EQUAL(20, canvas.rows);
  TEST_ASSERT_EQUAL(10, canvas.cols);
  for (uint8_t i = 0; i < 200; ++i) {
    TEST_ASSERT_EQUAL(' ', canvas.grid[i]);
  }
}

void test_new_game_renders_empty_canvas(void) {
  shape_t canvas;
  Sequence sequence = Sequence(0);

  makeCanvas(canvas);
  // Dirty the canvas with some pixels.
  for (uint8_t i = 0; i < 200; ++i) {
    canvas.grid[i] = 'x';
  }

  // Confirm that did what we think it did.
  TEST_ASSERT_EQUAL(20, canvas.rows);
  TEST_ASSERT_EQUAL(10, canvas.cols);

  Game game = Game(canvas, sequence);
  for (uint8_t i = 0; i < 200; ++i) {
    TEST_ASSERT_EQUAL(' ', canvas.grid[i]);
  }
  TEST_ASSERT_EQUAL(20, canvas.rows);
  TEST_ASSERT_EQUAL(10, canvas.cols);

  // TODO: I don't think the bounding box is updated correctly.
}

int main(int argc, char** argv) {
  UNITY_BEGIN();

  // board.h
  RUN_TEST(test_empty_rows);
  RUN_TEST(test_non_empty_rows);

  // piece.h
  RUN_TEST(test_rotate_clockwise);
  RUN_TEST(test_rotate_anticlockwise);

  // geom.h
  RUN_TEST(test_row_empty);
  RUN_TEST(test_col_empty);
  RUN_TEST(test_bounding_box_empty_grid);
  RUN_TEST(test_bounding_box);
  RUN_TEST(test_update_bounding_box);
  RUN_TEST(test_fill);

  // board.h
  RUN_TEST(test_generate_shapes);
  RUN_TEST(test_in_bounds);
  RUN_TEST(test_collide);
  RUN_TEST(test_stick);

  // sequence.h
  RUN_TEST(test_sequence);

  // game.h
  RUN_TEST(test_make_canvas);
  RUN_TEST(test_new_game_renders_empty_canvas);

  UNITY_END();
}

