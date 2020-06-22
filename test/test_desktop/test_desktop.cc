#include <board.h>
#include <piece.h>
#include <shape.h>
#include <game.h>
#include <sequence.h>
#include <unity.h>
#include "test_desktop_data.h"

void noOpCallback() {
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
  TEST_ASSERT_EQUAL(RIGHT, nextClockwiseOrientation(UP));
  TEST_ASSERT_EQUAL(DOWN, nextClockwiseOrientation(RIGHT));
  TEST_ASSERT_EQUAL(LEFT, nextClockwiseOrientation(DOWN));
  TEST_ASSERT_EQUAL(UP, nextClockwiseOrientation(LEFT));
}

void test_rotate_anticlockwise(void) {
  TEST_ASSERT_EQUAL(LEFT, nextAntiClockwiseOrientation(UP));
  TEST_ASSERT_EQUAL(DOWN, nextAntiClockwiseOrientation(LEFT));
  TEST_ASSERT_EQUAL(RIGHT, nextAntiClockwiseOrientation(DOWN));
  TEST_ASSERT_EQUAL(UP, nextAntiClockwiseOrientation(RIGHT));
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
  Shape shape = Shape(3, 4, "            ");
  TEST_ASSERT_EQUAL(0, shape.getBBox().uLeft.row);
  TEST_ASSERT_EQUAL(0, shape.getBBox().uLeft.col);
  TEST_ASSERT_EQUAL(0, shape.getBBox().lRight.row);
  TEST_ASSERT_EQUAL(0, shape.getBBox().lRight.col);

  // Put a shape inside.
  shape.setCharAt('x', (struct point) { 1, 1 });
  shape.setCharAt('x', (struct point) { 1, 2 });
  TEST_ASSERT_EQUAL(1, shape.getBBox().uLeft.row);
  TEST_ASSERT_EQUAL(1, shape.getBBox().uLeft.col);
  TEST_ASSERT_EQUAL(1, shape.getBBox().lRight.row);
  TEST_ASSERT_EQUAL(2, shape.getBBox().lRight.col);

  // Remove the shape. It's empty again.
  shape.setCharAt(' ', (struct point) { 1, 1 });
  shape.setCharAt(' ', (struct point) { 1, 2 });
  TEST_ASSERT_EQUAL(0, shape.getBBox().uLeft.row);
  TEST_ASSERT_EQUAL(0, shape.getBBox().uLeft.col);
  TEST_ASSERT_EQUAL(0, shape.getBBox().lRight.row);
  TEST_ASSERT_EQUAL(0, shape.getBBox().lRight.col);
}

void test_fill(void) {
  Shape shape = Shape(3, 4, "            ");
  Shape fullShape = Shape(3, 4, "            ");
  const char *full =  "cccc"
                      "cccc"
                      "cccc";

  fullShape.fillWithChars(full);

  // Initially, the bbox is empty because the shape has no visible contents.
  TEST_ASSERT_EQUAL(4, shape.getCols());
  TEST_ASSERT_EQUAL(3, shape.getRows());
  TEST_ASSERT_EQUAL(0, shape.getBBox().uLeft.col);
  TEST_ASSERT_EQUAL(0, shape.getBBox().uLeft.row);
  TEST_ASSERT_EQUAL(0, shape.getBBox().lRight.col);
  TEST_ASSERT_EQUAL(0, shape.getBBox().lRight.row);
  TEST_ASSERT_FALSE(shape == fullShape);

  shape.fillWith('c');
  // After filling, the bbox is updated.
  TEST_ASSERT_EQUAL(4, shape.getCols());
  TEST_ASSERT_EQUAL(3, shape.getRows());
  TEST_ASSERT_EQUAL(0, shape.getBBox().uLeft.col);
  TEST_ASSERT_EQUAL(0, shape.getBBox().uLeft.row);
  TEST_ASSERT_EQUAL(3, shape.getBBox().lRight.col);
  TEST_ASSERT_EQUAL(2, shape.getBBox().lRight.row);
  TEST_ASSERT_TRUE(shape == fullShape);
}

void test_generate_shapes(void) {
  Shape J_UP =    Shape(4, 4, "J   JJJ         ");
  Shape J_RIGHT = Shape(4, 4, " JJ  J   J      ");
  Shape J_DOWN =  Shape(4, 4, "    JJJ   J     ");
  Shape J_LEFT =  Shape(4, 4, " J   J  JJ      ");
  Piece piece = Piece(J_UP, J_RIGHT, J_DOWN, J_LEFT);

  TEST_ASSERT_EQUAL(4, piece.shapeFacing(UP).getRows());
  TEST_ASSERT_EQUAL(4, piece.shapeFacing(UP).getCols());
  TEST_ASSERT_EQUAL(0, piece.shapeFacing(UP).getBBox().uLeft.row);
  TEST_ASSERT_EQUAL(2, piece.shapeFacing(UP).getBBox().lRight.col);
  TEST_ASSERT_EQUAL(1, piece.shapeFacing(UP).getBBox().lRight.row);
  TEST_ASSERT_EQUAL(0, piece.shapeFacing(UP).getBBox().uLeft.col);

  TEST_ASSERT_EQUAL(4, piece.shapeFacing(RIGHT).getRows());
  TEST_ASSERT_EQUAL(4, piece.shapeFacing(RIGHT).getCols());
  TEST_ASSERT_EQUAL(0, piece.shapeFacing(RIGHT).getBBox().uLeft.row);
  TEST_ASSERT_EQUAL(2, piece.shapeFacing(RIGHT).getBBox().lRight.col);
  TEST_ASSERT_EQUAL(2, piece.shapeFacing(RIGHT).getBBox().lRight.row);
  TEST_ASSERT_EQUAL(1, piece.shapeFacing(RIGHT).getBBox().uLeft.col);

  TEST_ASSERT_EQUAL(4, piece.shapeFacing(DOWN).getRows());
  TEST_ASSERT_EQUAL(4, piece.shapeFacing(DOWN).getCols());
  TEST_ASSERT_EQUAL(1, piece.shapeFacing(DOWN).getBBox().uLeft.row);
  TEST_ASSERT_EQUAL(2, piece.shapeFacing(DOWN).getBBox().lRight.col);
  TEST_ASSERT_EQUAL(2, piece.shapeFacing(DOWN).getBBox().lRight.row);
  TEST_ASSERT_EQUAL(0, piece.shapeFacing(DOWN).getBBox().uLeft.col);

  TEST_ASSERT_EQUAL(4, piece.shapeFacing(LEFT).getRows());
  TEST_ASSERT_EQUAL(4, piece.shapeFacing(LEFT).getCols());
  TEST_ASSERT_EQUAL(0, piece.shapeFacing(LEFT).getBBox().uLeft.row);
  TEST_ASSERT_EQUAL(1, piece.shapeFacing(LEFT).getBBox().lRight.col);
  TEST_ASSERT_EQUAL(2, piece.shapeFacing(LEFT).getBBox().lRight.row);
  TEST_ASSERT_EQUAL(0, piece.shapeFacing(LEFT).getBBox().uLeft.col);
}

void test_in_bounds(void) {
  Shape J_UP =    Shape(4, 4, "J   JJJ         ");
  Shape J_RIGHT = Shape(4, 4, " JJ  J   J      ");
  Shape J_DOWN =  Shape(4, 4, "    JJJ   J     ");
  Shape J_LEFT =  Shape(4, 4, " J   J  JJ      ");
  Piece piece = Piece(J_UP, J_RIGHT, J_DOWN, J_LEFT);

  Shape grid = Shape(20, 10, board_empty_grid);

  TEST_ASSERT_TRUE(inBounds(grid, piece.shapeFacing(UP), (struct point) { 0, 0 }));
  TEST_ASSERT_TRUE(inBounds(grid, piece.shapeFacing(UP), (struct point) { 0, 8 }));
  TEST_ASSERT_TRUE(inBounds(grid, piece.shapeFacing(UP), (struct point) { 19, 0 }));
  TEST_ASSERT_TRUE(inBounds(grid, piece.shapeFacing(UP), (struct point) { 19, 8 }));

  TEST_ASSERT_FALSE(inBounds(grid, piece.shapeFacing(UP), (struct point) { -1, 0 }));
  TEST_ASSERT_FALSE(inBounds(grid, piece.shapeFacing(UP), (struct point) { 0, 9 }));
  TEST_ASSERT_FALSE(inBounds(grid, piece.shapeFacing(UP), (struct point) { 20, 0 }));
  TEST_ASSERT_FALSE(inBounds(grid, piece.shapeFacing(UP), (struct point) { 18, 9 }));
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
  Shape board = Shape(6, 4, board_chars);
  Shape shape = Shape(2, 2, shape_chars);

  // Valid, all-on-screen placement.
  TEST_ASSERT_FALSE(board.collides(shape, (struct point) { 0, 0 }));

  // Part of the shape is off the screen. That's ok.
  TEST_ASSERT_FALSE(board.collides(shape, (struct point) { 0, -1 }));
  TEST_ASSERT_FALSE(board.collides(shape, (struct point) { -1, -1 }));

  // Bottom tip overlaps.
  TEST_ASSERT_TRUE(board.collides(shape, (struct point) { 2, 0 }));

  // Top overlaps.
  TEST_ASSERT_TRUE(board.collides(shape, (struct point) { 3, 1 }));

  // Top and bottom overlap.
  TEST_ASSERT_TRUE(board.collides(shape, (struct point) { 4, -1 }));
  TEST_ASSERT_TRUE(board.collides(shape, (struct point) { 4, 0 }));

  // Hidden under the lip of the thing.
  TEST_ASSERT_FALSE(board.collides(shape, (struct point) { 4, 1 }));
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
  Shape board = Shape(6, 4, board_chars);
  Shape shape = Shape(2, 2, shape_chars);

  board.stick(shape, (struct point) { 4, 1 });

  // This is what it looks like after sticking.
  const char *stuck_chars = "    "
                            "    "
                            "    "
                            "xxx "
                            "xxo "
                            "xxo ";
  Shape expected = Shape(6, 4, stuck_chars);
  TEST_ASSERT_TRUE(board == expected);
}

void test_stick_at_edge(void) {
    const char *board_chars = "    "
                            "    "
                            "    "
                            " xx "
                            " x  "
                            " x  ";
  const char *shape_chars = "  o"
                            "  o";
  Shape board = Shape(6, 4, board_chars);
  Shape shape = Shape(2, 3, shape_chars);

  board.stick(shape, (struct point) { 4, -2 });

  // This is what it looks like after sticking.
  const char *stuck_chars = "    "
                            "    "
                            "    "
                            " xx "
                            "ox  "
                            "ox  ";
  Shape expected = Shape(6, 4, stuck_chars);
  TEST_ASSERT_TRUE(board == expected);
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
  Shape canvas = Shape(20, 10, board_empty_grid);

  TEST_ASSERT_EQUAL(20, canvas.getRows());
  TEST_ASSERT_EQUAL(10, canvas.getCols());
  for (int i = 0; i < 200; ++i) {
    TEST_ASSERT_EQUAL(' ', canvas.getGrid()[i]);
  }
}

void test_new_game_renders_empty_canvas(void) {
  Shape canvas = Shape(20, 10, board_empty_grid);
  // Dirty the canvas with some pixels. The game will clear these.
  canvas.fillWith('x');

  Sequence sequence = Sequence(0);
  Game game = Game(canvas, sequence, noOpCallback);

  // First and last squares are blank.
  TEST_ASSERT_EQUAL(' ', canvas.getGrid()[0]);
  TEST_ASSERT_EQUAL(' ', canvas.getGrid()[199]);
  TEST_ASSERT_EQUAL(20, canvas.getRows());
  TEST_ASSERT_EQUAL(10, canvas.getCols());

}

void test_new_game_renders_first_piece(void) {
  Shape canvas = Shape(20, 10, board_empty_grid);

  Game game = Game(canvas, Sequence(0), noOpCallback);

  // First piece has been dropped.
  Shape expected = Shape(20, 10, board_with_first_piece);

  TEST_ASSERT_TRUE(canvas == expected);
}

void test_move_left(void) {
  Shape canvas = Shape(20, 10, board_empty_grid);
  Sequence sequence = Sequence(0);
  Game game = Game(canvas, sequence, noOpCallback);

  // Move all the way to the edge of the board and stop when you run into it.
  TEST_ASSERT_TRUE(game.moveLeft());
  TEST_ASSERT_TRUE(game.moveLeft());
  TEST_ASSERT_TRUE(game.moveLeft());
  TEST_ASSERT_FALSE(game.moveLeft());

  // First piece is all the way to the left.
  Shape expected = Shape(20, 10, board_with_first_piece_far_left);
  TEST_ASSERT_TRUE(canvas == expected);
}

void test_move_right(void) {
  Shape canvas = Shape(20, 10, board_empty_grid);
  Sequence sequence = Sequence(0);
  Game game = Game(canvas, sequence, noOpCallback);

  // Move all the way to the edge of the board and stop when you run into it.
  TEST_ASSERT_TRUE(game.moveRight());
  TEST_ASSERT_TRUE(game.moveRight());
  TEST_ASSERT_TRUE(game.moveRight());
  TEST_ASSERT_FALSE(game.moveRight());

  // First piece is all the way to the left.
  Shape expected = Shape(20, 10, board_with_first_piece_far_right);
  TEST_ASSERT_TRUE(canvas == expected);
}

void test_move_left_then_tick(void) {
  Shape canvas = Shape(20, 10, board_empty_grid);
  Sequence sequence = Sequence(0);
  Game game = Game(canvas, sequence, noOpCallback);

  // Move all the way to the edge of the board and stop when you run into it.
  TEST_ASSERT_TRUE(game.moveLeft());
  TEST_ASSERT_TRUE(game.moveLeft());
  TEST_ASSERT_TRUE(game.moveLeft());
  TEST_ASSERT_FALSE(game.moveLeft());

  game.tick();
  Shape expected = Shape(20, 10, board_with_first_piece_far_left_dropped_1);
  TEST_ASSERT_TRUE(canvas == expected);
}

void test_move_left_then_tick_down_see_new_piece() {
  Shape canvas = Shape(20, 10, board_empty_grid);
  Sequence sequence = Sequence(0);
  Game game = Game(canvas, sequence, noOpCallback);

  // Move all the way to the edge of the board and stop when you run into it.
  TEST_ASSERT_TRUE(game.moveLeft());
  TEST_ASSERT_TRUE(game.moveLeft());
  TEST_ASSERT_TRUE(game.moveLeft());
  TEST_ASSERT_FALSE(game.moveLeft());

  game.tick();
  game.tick();
  game.tick();
  game.tick();
  game.tick();
  game.tick();
  game.tick();
  game.tick();
  game.tick();
  game.tick();
  game.tick();
  game.tick();
  game.tick();
  game.tick();
  game.tick();
  game.tick();
  game.tick();
  game.tick();
  Shape expected = Shape(20, 10, board_with_first_piece_far_left_dropped_entirely);
  TEST_ASSERT_TRUE(canvas == expected);

  // The piece is stuck now. New piece emerges.
  game.tick();
  Shape expected2 = Shape(20, 10, board_with_first_piece_far_left_dropped_new_piece_showing);
  TEST_ASSERT_TRUE(canvas == expected2);
}

void test_game_rotate_clockwise(void) {
  Shape canvas = Shape(20, 10, board_empty_grid);
  Sequence sequence = Sequence(0);
  Game game = Game(canvas, sequence, noOpCallback);

  TEST_ASSERT_TRUE(canvas == Shape(20, 10, board_with_first_piece));
  TEST_ASSERT_TRUE(game.rotateClockwise());
  TEST_ASSERT_TRUE(canvas == Shape(20, 10, board_with_first_piece_r1));
  TEST_ASSERT_TRUE(game.rotateClockwise());
  TEST_ASSERT_TRUE(canvas == Shape(20, 10, board_with_first_piece_r2));
  TEST_ASSERT_TRUE(game.rotateClockwise());
  TEST_ASSERT_TRUE(canvas == Shape(20, 10, board_with_first_piece_r3));
  TEST_ASSERT_TRUE(game.rotateClockwise());
  TEST_ASSERT_TRUE(canvas == Shape(20, 10, board_with_first_piece));
}

void test_game_rotate_clockwise_orientation_resets(void) {
  Shape canvas = Shape(20, 10, board_empty_grid);
  Sequence sequence = Sequence(0);
  Game game = Game(canvas, sequence, noOpCallback);
  game.rotateClockwise();
  game.drop(); // Drop I; J appears
  game.drop(); // Drop J; L appears
  game.drop(); // Drop L; O appears
  game.drop(); // Drop O; S appears
  game.drop(); // Drop S; T appears
  game.drop(); // Drop T; Z appears
  game.drop(); // Drop Z; I appears again
  // Rotation on I has been reset and it is now facing UP again.
  TEST_ASSERT_TRUE(canvas == Shape(20, 10, board_with_first_piece_r1_drop_and_drop_six_more));
}

void test_drop(void) {
  Shape canvas = Shape(20, 10, board_empty_grid);
  Sequence sequence = Sequence(0);
  Game game = Game(canvas, sequence, noOpCallback);

  TEST_ASSERT_TRUE(canvas == Shape(20, 10, board_with_first_piece));

  game.moveLeft();
  game.moveLeft();
  game.moveLeft();
  game.drop();
  Shape expected = Shape(20, 10, board_with_first_piece_far_left_dropped_new_piece_showing);
  TEST_ASSERT_TRUE(canvas == expected);
  game.drop();

  Shape expected2 = Shape(20, 10, board_with_second_piece_dropped);
  TEST_ASSERT_TRUE(canvas == expected2);
}

void test_game_over(void) {
  Shape canvas = Shape(20, 10, board_empty_grid);
  Sequence sequence = Sequence(0);

  static bool gameOver = false;
  auto callback = []() { gameOver = true; };
  Game game = Game(canvas, sequence, callback);

  // Drop pieces until the game ends.
  game.drop();
  game.drop();
  game.drop();
  game.drop();
  game.drop();
  game.drop();
  game.drop();
  game.drop();
  game.drop();
  TEST_ASSERT_FALSE(gameOver);
  game.drop();
  TEST_ASSERT_TRUE(gameOver);
  const char *gridAtGameOver = canvas.getGrid();

  // Moving and ticking do nothing.
  game.moveLeft();
  game.moveRight();
  game.rotateClockwise();
  game.tick();
  game.tick();
  game.tick();
  TEST_ASSERT_EQUAL(canvas.getGrid(), gridAtGameOver);
}

void test_drop_points(void) {
  Shape canvas = Shape(20, 10, board_empty_grid);
  Sequence sequence = Sequence(0);
  Game game = Game(canvas, sequence, noOpCallback);

  TEST_ASSERT_EQUAL(0, game.getStats().score);
  game.drop();

  // One point for each line dropped.
  TEST_ASSERT_EQUAL(18, game.getStats().score);
  game.drop();

  TEST_ASSERT_EQUAL(35, game.getStats().score);
}

void test_reset(void) {
  Shape canvas = Shape(20, 10, board_empty_grid);
  Sequence sequence = Sequence(0);
  Game game = Game(canvas, sequence, noOpCallback);
  // Mutate the canvas and the score.
  game.rotateClockwise();
  game.drop();

  game.reset();
  TEST_ASSERT_TRUE(canvas == Shape(20, 10, board_empty_grid));
  TEST_ASSERT_EQUAL(0, game.getStats().score);
}

void test_clear_one_row_after_dropping(void) {
  Shape canvas = Shape(20, 10, board_empty_grid);
  Sequence sequence = Sequence(0);
  Game game = Game(canvas, sequence, noOpCallback);

  // Drop the I in the middle.
  game.drop();

  // Drop the J on the left.
  game.moveLeft();
  game.moveLeft();
  game.moveLeft();
  game.drop();

  // Drop the L on the right.
  game.moveRight();
  game.moveRight();
  game.moveRight();
  game.moveRight();
  game.drop();

  // The bottom row is full and is marked for deletion.
  TEST_ASSERT_TRUE(canvas.noEmptySpacesInRow(19));
  TEST_ASSERT_TRUE(canvas == Shape(20, 10, board_with_full_row_marked_for_deletion));

  // Ticking forward in time removes the row.
  game.tick();
  // Another tick exposes the next piece.
  game.tick();
  TEST_ASSERT_TRUE(canvas == Shape(20, 10, board_with_full_row_deleted_and_next_piece_showing));
}

void test_clear_one_row_after_ticking_down() {
  Shape canvas = Shape(20, 10, board_empty_grid);
  Sequence sequence = Sequence(0);
  Game game = Game(canvas, sequence, noOpCallback);

  // Drop the I in the middle.
  game.drop();

  // Drop the J on the left.
  game.moveLeft();
  game.moveLeft();
  game.moveLeft();
  game.drop();

  // Move the L to the right and let it eventually tick down.
  game.moveRight();
  game.moveRight();
  game.moveRight();
  game.moveRight();
  game.tick();
  game.tick();
  game.tick();
  game.tick();
  game.tick();
  game.tick();
  game.tick();
  game.tick();
  game.tick();
  game.tick();
  game.tick();
  game.tick();
  game.tick();
  game.tick();
  game.tick();
  game.tick();
  game.tick();
  game.tick();
  game.tick();

  // The bottom row is full and is marked for deletion.
  TEST_ASSERT_TRUE(canvas.noEmptySpacesInRow(19));
  TEST_ASSERT_TRUE(canvas == Shape(20, 10, board_with_full_row_marked_for_deletion));

  // Ticking forward in time removes the row.
  game.tick();
  // Another tick exposes the next piece.
  game.tick();
  TEST_ASSERT_TRUE(canvas == Shape(20, 10, board_with_full_row_deleted_and_next_piece_showing));
}

void test_scoring(void) {
  Shape canvas = Shape(20, 10, board_empty_grid);
  Sequence sequence = Sequence(0);
  Game game = Game(canvas, sequence, noOpCallback);

  game.setGrid(board0_grid);
  game.rotateClockwise();

  game.moveRight();
  game.moveRight();
  game.moveRight();
  game.moveRight();
  game.drop();

  game.tick();
  game.tick();
  // Score 16 rows dropped, and 1200 for a "tetris".
  TEST_ASSERT_EQUAL(1216, game.getStats().score);
}

int main(int argc, char** argv) {
  UNITY_BEGIN();

  // piece.h
  RUN_TEST(test_rotate_clockwise);
  RUN_TEST(test_rotate_anticlockwise);

  // shape.h
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
  RUN_TEST(test_stick_at_edge);

  // sequence.h
  RUN_TEST(test_sequence);

  // game.h
  RUN_TEST(test_make_canvas);
  RUN_TEST(test_new_game_renders_empty_canvas);
  RUN_TEST(test_new_game_renders_first_piece);
  RUN_TEST(test_move_left);
  RUN_TEST(test_move_right);
  RUN_TEST(test_move_left_then_tick);
  RUN_TEST(test_move_left_then_tick_down_see_new_piece);
  RUN_TEST(test_game_rotate_clockwise);
  RUN_TEST(test_game_rotate_clockwise_orientation_resets);
  RUN_TEST(test_drop);
  RUN_TEST(test_game_over);
  RUN_TEST(test_drop_points);
  RUN_TEST(test_clear_one_row_after_dropping);
  RUN_TEST(test_clear_one_row_after_ticking_down);
  RUN_TEST(test_scoring);

  UNITY_END();
}

