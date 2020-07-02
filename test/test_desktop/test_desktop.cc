#include "test_desktop_data.h"

#include <unity.h>

#include <iostream>

#include <pieces.h>
#include <board.h>
#include <sequence.h>
#include <game.h>

void assertEqualGrids(const char *expected, const char *observed) {
  for (int i = 0; i < BOARD_CELLS; ++i) {
    TEST_ASSERT_EQUAL(expected[i], observed[i]);
  }
}

void test_clockwise(void) {
  TEST_ASSERT_EQUAL(RIGHT, nextClockwise(UP));
  TEST_ASSERT_EQUAL(DOWN, nextClockwise(RIGHT));
  TEST_ASSERT_EQUAL(LEFT, nextClockwise(DOWN));
  TEST_ASSERT_EQUAL(UP, nextClockwise(LEFT));
}

void test_anticlockwise(void) {
  TEST_ASSERT_EQUAL(LEFT, nextAntiClockwise(UP));
  TEST_ASSERT_EQUAL(DOWN, nextAntiClockwise(LEFT));
  TEST_ASSERT_EQUAL(RIGHT, nextAntiClockwise(DOWN));
  TEST_ASSERT_EQUAL(UP, nextAntiClockwise(RIGHT));
}

void test_init_shape(void) {
  shape_t shape;
  initShape(&shape, S);
  const char *expectedGrid = " SS SS          ";

  TEST_ASSERT_EQUAL(S, shape.name);
  TEST_ASSERT_EQUAL(UP, shape.orientation);
  TEST_ASSERT_EQUAL(0, shape.location.row);
  TEST_ASSERT_EQUAL(3, shape.location.col);

  for (int i = 0; i < PIECE_CELLS; ++i) {
    TEST_ASSERT_EQUAL(expectedGrid[i], shape.grid[i]);
  }
}

void test_rotate_shape(void) {
  shape_t shape;
  initShape(&shape, T);

  TEST_ASSERT_EQUAL(UP, shape.orientation);
  rotateShape(&shape);
  TEST_ASSERT_EQUAL(RIGHT, shape.orientation);
  rotateShape(&shape);
  TEST_ASSERT_EQUAL(DOWN, shape.orientation);
  rotateShape(&shape);
  TEST_ASSERT_EQUAL(LEFT, shape.orientation);
  rotateShape(&shape);
  TEST_ASSERT_EQUAL(UP, shape.orientation);
}

void test_update_grid(void) {
  shape_t shape;
  initShape(&shape, Z);
  rotateShape(&shape);
  const char *expected = "  Z  ZZ  Z      ";

  for (int i = 0; i < PIECE_CELLS; ++i) {
    TEST_ASSERT_EQUAL(expected[i], shape.grid[i]);
  }
}

void test_init_board(void) {
  board_t board;
  initBoard(&board);

  for (int i = 0; i < BOARD_CELLS; ++i) {
    TEST_ASSERT_EQUAL(EMPTY_CELL, board.grid[i]);
  }
}

void test_fill_with(void) {
  board_t board;
  initBoard(&board);
  fillWith(&board, 'm');

  for (int i = 0; i < BOARD_CELLS; ++i) {
    TEST_ASSERT_EQUAL('m', board.grid[i]);
  }
}

void test_fill_row_with(void) {
  board_t board;
  initBoard(&board);
  fillRowWith(&board, 10, 'x');

  for (int i = 0; i < 10 * BOARD_COLS; ++i) {
    TEST_ASSERT_EQUAL(EMPTY_CELL, board.grid[i]);
  }
  for (int i = 10 * BOARD_COLS; i < 11 * BOARD_COLS; ++i) {
    TEST_ASSERT_EQUAL('x', board.grid[i]);
  }
  for (int i = 11 * BOARD_COLS; i < BOARD_CELLS; ++i) {
    TEST_ASSERT_EQUAL(EMPTY_CELL, board.grid[i]);
  }
}

void test_inside(void) {
  board_t board;
  initBoard(&board);

  shape_t shape;
  initShape(&shape, Z);
  rotateShape(&shape);
  // We now have the RIGHT-facing Z, bbox { 0, 1, 2, 2 }.

  // Upper-left of board.
  point_t dst = { 0, -1 };
  TEST_ASSERT_TRUE(inside(&board, shape.name, shape.orientation, &dst));

  // Upper-right of board;
  dst = { 0, 7 };
  TEST_ASSERT_TRUE(inside(&board, shape.name, shape.orientation, &dst));

  // Lower-right of board.
  dst = { 17, 7 };
  TEST_ASSERT_TRUE(inside(&board, shape.name, shape.orientation, &dst));

  // Lower-left of board.
  dst = { 17, -1 };
  TEST_ASSERT_TRUE(inside(&board, shape.name, shape.orientation, &dst));

  // Outside top.
  dst = { -1, -1 };
  TEST_ASSERT_FALSE(inside(&board, shape.name, shape.orientation, &dst));

  // Outside right.
  dst = { 0, 8 };
  TEST_ASSERT_FALSE(inside(&board, shape.name, shape.orientation, &dst));

  // Outside bottom.
  dst = { 18, 7 };
  TEST_ASSERT_FALSE(inside(&board, shape.name, shape.orientation, &dst));

  // Outside left.
  dst = { 17, -2 };
  TEST_ASSERT_FALSE(inside(&board, shape.name, shape.orientation, &dst));
}

void test_collides_empty(void) {
  board_t board;
  initBoard(&board);

  shape_t shape;
  initShape(&shape, I);

  point_t dst = { 5, 5 };

  TEST_ASSERT_FALSE(collides(&board, shape.name, shape.orientation, &dst));
}

void test_collides_full(void) {
  board_t board;
  initBoard(&board);
  fillWith(&board, 'x');

  shape_t shape;
  initShape(&shape, I);

  point_t dst = { 5, 5 };

  TEST_ASSERT_TRUE(collides(&board, shape.name, shape.orientation, &dst));
}

void test_collides_bottom_piece(void) {
  board_t board;
  initBoard(&board);
  setGrid(&board, board_with_first_piece_far_left_dropped_entirely);

  shape_t shape;
  initShape(&shape, J);
  TEST_ASSERT_EQUAL(J, shape.name);
  TEST_ASSERT_EQUAL(UP, shape.orientation);

  point_t dst = { 18, 3 };

  TEST_ASSERT_TRUE(collides(&board, shape.name, shape.orientation, &dst));

  // Stick it one row above that. Looks right.
  dst = { 17, 3 };
  stick(&board, &shape, &dst);
  assertEqualGrids(board_with_second_piece_dropped, board.grid);
}

void test_stick(void) {
  board_t board;
  initBoard(&board);

  shape_t shape;
  initShape(&shape, I);

  point_t dst = { START_ROW, START_COL };
  stick(&board, &shape, &dst);

  assertEqualGrids(board_with_first_piece, board.grid);
}

void test_no_empty_spaces_in_row(void) {
  board_t board;
  initBoard(&board);
  setGrid(&board, board1_grid);
  TEST_ASSERT_TRUE(noEmptySpacesInRow(&board, 19));
  TEST_ASSERT_FALSE(noEmptySpacesInRow(&board, 0));
}

void test_mark_dead_rows(void) {
  board_t board;
  initBoard(&board);
  setGrid(&board, board1_grid);
  markDeadRows(&board);

  assertEqualGrids(board1_grid_dead_rows_marked, board.grid);
}

void test_remove_dead_rows(void) {
  board_t board;
  initBoard(&board);
  setGrid(&board, board1_grid);
  markDeadRows(&board);
  removeDeadRows(&board);

  assertEqualGrids(board1_grid_dead_rows_removed, board.grid);
}

void test_sequence(void) {
  sequence_t sequence;
  initSequence(&sequence, 0);

  // Sequence is deterministic in tests.
  TEST_ASSERT_EQUAL(I, next(&sequence));
  TEST_ASSERT_EQUAL(J, next(&sequence));
  TEST_ASSERT_EQUAL(L, next(&sequence));
  TEST_ASSERT_EQUAL(O, next(&sequence));
  TEST_ASSERT_EQUAL(S, next(&sequence));
  TEST_ASSERT_EQUAL(T, next(&sequence));
  TEST_ASSERT_EQUAL(Z, next(&sequence));

  // It resets and starts again.
  TEST_ASSERT_EQUAL(I, next(&sequence));
  TEST_ASSERT_EQUAL(J, next(&sequence));
  TEST_ASSERT_EQUAL(L, next(&sequence));
  TEST_ASSERT_EQUAL(O, next(&sequence));
  TEST_ASSERT_EQUAL(S, next(&sequence));
  TEST_ASSERT_EQUAL(T, next(&sequence));
  TEST_ASSERT_EQUAL(Z, next(&sequence));
}

void test_reshuffle(void) {
  sequence_t sequence;
  initSequence(&sequence, 0);

  // Start of sequence ...
  TEST_ASSERT_EQUAL(I, next(&sequence));
  TEST_ASSERT_EQUAL(J, next(&sequence));

  // (Reshuffling uses original sequence in tests.)
  reshuffle(&sequence);
  TEST_ASSERT_EQUAL(I, next(&sequence));
}

void test_reset_game(void) {
  sequence_t sequence;
  initSequence(&sequence, 0);
  game_t game;
  reset(&game, &sequence);

  // Canvas and board grid are empty.
  for (int i = 0; i < BOARD_CELLS; ++i) {
    TEST_ASSERT_EQUAL(EMPTY_CELL, game.canvas.grid[i]);
    TEST_ASSERT_EQUAL(EMPTY_CELL, game.board.grid[i]);
  }

  // Piece is initialized.
  TEST_ASSERT_EQUAL(I, game.piece.name);

  // Game can get next piece.
  TEST_ASSERT_EQUAL(J, next(game.sequence));
}

void test_game_sequence(void) {
  sequence_t sequence;
  initSequence(&sequence, 0);

  game_t game;
  reset(&game, &sequence);

  // Initial piece for the game is in sync.
  TEST_ASSERT_EQUAL(0, game.piece.name);

  // The iterator is waiting the next tick.
  // (Sanity-chcking an implementation detail.)
  TEST_ASSERT_EQUAL(1, game.sequence->iteration);

  // Producing a new piece advances the iterator.
  produceNextPiece(&game);
  TEST_ASSERT_EQUAL(2, game.sequence->iteration);

  // Getting the next piece name works as expected.
  next(game.sequence);
  TEST_ASSERT_EQUAL(3, game.sequence->iteration);

  // Resetting the game resets the iterator.
  reset(&game, &sequence);

  // Initial piece for the game is in sync.
  TEST_ASSERT_EQUAL(0, game.piece.name);
}

void test_render(void) {
  sequence_t sequence;
  initSequence(&sequence, 0);
  game_t game;
  reset(&game, &sequence);

  setGrid(&(game.board), board1_grid_dead_rows_removed);

  // Rendering sticks the grid and the new piece on the canvas.
  render(&game);
  assertEqualGrids(board1_grid_resolved_with_new_I, game.canvas.grid);

  // Board has not been mutated.
  assertEqualGrids(board1_grid_dead_rows_removed, game.board.grid);
}

void test_move_left(void) {
  sequence_t sequence;
  initSequence(&sequence, 0);

  game_t game;
  reset(&game, &sequence);

  render(&game);
  // First piece is there.
  for (int i = 0; i < BOARD_CELLS; ++i) {
    TEST_ASSERT_EQUAL(board_with_first_piece[i], game.canvas.grid[i]);
  }

  TEST_ASSERT_TRUE(movePieceLeft(&game));
  TEST_ASSERT_TRUE(movePieceLeft(&game));
  TEST_ASSERT_TRUE(movePieceLeft(&game));
  // After three moves, the piece is at the far left.
  assertEqualGrids(board_with_first_piece_far_left, game.canvas.grid);

  // Trying to move once more does nothing, because we are hitting the wall.
  TEST_ASSERT_FALSE(movePieceLeft(&game));
  assertEqualGrids(board_with_first_piece_far_left, game.canvas.grid);
}

void test_move_right(void) {
  sequence_t sequence;
  initSequence(&sequence, 0);

  game_t game;
  reset(&game, &sequence);

  render(&game);
  // First piece is there.
  assertEqualGrids(board_with_first_piece, game.canvas.grid);

  TEST_ASSERT_TRUE(movePieceRight(&game));
  TEST_ASSERT_TRUE(movePieceRight(&game));
  TEST_ASSERT_TRUE(movePieceRight(&game));
  // After three moves, the piece is at the far right.
  assertEqualGrids(board_with_first_piece_far_right, game.canvas.grid);

  // Trying to move once more does nothing, because we are hitting the wall.
  TEST_ASSERT_FALSE(movePieceRight(&game));
  assertEqualGrids(board_with_first_piece_far_right, game.canvas.grid);
}

void test_move_left_then_tick(void) {
  sequence_t sequence;
  initSequence(&sequence, 0);

  game_t game;
  reset(&game, &sequence);

  render(&game);
  // Move all the way left until we run into the wall.
  TEST_ASSERT_TRUE(movePieceLeft(&game));
  TEST_ASSERT_TRUE(movePieceLeft(&game));
  TEST_ASSERT_TRUE(movePieceLeft(&game));
  TEST_ASSERT_FALSE(movePieceLeft(&game));

  tick(&game);
  // See the piece all the way left, moved down one row.
  assertEqualGrids(board_with_first_piece_far_left_dropped_1,
                   game.canvas.grid);
}

void test_move_left_then_tick_down_see_new_piece(void) {
  sequence_t sequence;
  initSequence(&sequence, 0);

  game_t game;
  reset(&game, &sequence);

  render(&game);
  // Move all the way left until we run into the wall.
  TEST_ASSERT_TRUE(movePieceLeft(&game));
  TEST_ASSERT_TRUE(movePieceLeft(&game));
  TEST_ASSERT_TRUE(movePieceLeft(&game));
  TEST_ASSERT_FALSE(movePieceLeft(&game));

  // Fall all the way down until we stick to the bottom row.
  tick(&game);
  tick(&game);
  tick(&game);
  tick(&game);
  tick(&game);
  tick(&game);
  tick(&game);
  tick(&game);
  tick(&game);
  tick(&game);
  tick(&game);
  tick(&game);
  tick(&game);
  tick(&game);
  tick(&game);
  tick(&game);
  tick(&game);
  tick(&game);
  assertEqualGrids(board_with_first_piece_far_left_dropped_entirely,
                   game.canvas.grid);

  // The new piece emerges on next tick.
  tick(&game);
  TEST_ASSERT_EQUAL(J, game.piece.name);
  assertEqualGrids(board_with_first_piece_far_left_dropped_new_piece_showing,
                   game.canvas.grid);
}

void test_game_drop(void) {
  sequence_t sequence;
  initSequence(&sequence, 0);

  game_t game;
  reset(&game, &sequence);

  render(&game);
  TEST_ASSERT_TRUE(movePieceLeft(&game));
  TEST_ASSERT_TRUE(movePieceLeft(&game));
  TEST_ASSERT_TRUE(movePieceLeft(&game));
  dropPiece(&game);

  assertEqualGrids(board_with_first_piece_far_left_dropped_new_piece_showing,
                   game.canvas.grid);

  dropPiece(&game);
  assertEqualGrids(board_with_second_piece_dropped_third_piece_showing,
                   game.canvas.grid);
}

void test_game_rotate_clockwise(void) {
  sequence_t sequence;
  initSequence(&sequence, 0);

  game_t game;
  reset(&game, &sequence);

  render(&game);
  assertEqualGrids(board_with_first_piece, game.canvas.grid);
  // Rotate ...
  TEST_ASSERT_TRUE(rotatePiece(&game));
  assertEqualGrids(board_with_first_piece_r1, game.canvas.grid);
  TEST_ASSERT_TRUE(rotatePiece(&game));
  assertEqualGrids(board_with_first_piece_r2, game.canvas.grid);
  TEST_ASSERT_TRUE(rotatePiece(&game));
  assertEqualGrids(board_with_first_piece_r3, game.canvas.grid);
  TEST_ASSERT_TRUE(rotatePiece(&game));
  // Back in the initial position.
  assertEqualGrids(board_with_first_piece, game.canvas.grid);
}

void test_game_rotate_clockwise_against_edge(void) {
  sequence_t sequence;
  initSequence(&sequence, 0);

  game_t game;
  reset(&game, &sequence);

  render(&game);
  TEST_ASSERT_TRUE(rotatePiece(&game));
  // I piece is now aligned vertically.
  TEST_ASSERT_EQUAL(RIGHT, game.piece.orientation);

  TEST_ASSERT_TRUE(movePieceRight(&game));
  TEST_ASSERT_TRUE(movePieceRight(&game));
  TEST_ASSERT_TRUE(movePieceRight(&game));
  TEST_ASSERT_TRUE(movePieceRight(&game));
  // I piece is now one square away from edge of board.

  TEST_ASSERT_FALSE(rotatePiece(&game));
  // No room to rotate at edge of board.
  TEST_ASSERT_EQUAL(RIGHT, game.piece.orientation);
}

void test_game_rotate_clockwise_orientation_resets(void) {
  sequence_t sequence;
  initSequence(&sequence, 0);

  game_t game;
  reset(&game, &sequence);

  rotatePiece(&game);
  dropPiece(&game);  // Drop rotated I; J appears
  dropPiece(&game);  // Drop J; L appears
  dropPiece(&game);  // Drop L; O appears
  dropPiece(&game);  // Drop O; S appears
  dropPiece(&game);  // Drop S; T appears
  dropPiece(&game);  // Drop T; Z appears
  dropPiece(&game);  // Drop Z; I appears again
  // Rotation on I has been reset and it is now facing UP again.
  assertEqualGrids(board_with_first_piece_r1_drop_and_drop_six_more,
                   game.canvas.grid);
}

void test_game_rotate_clockwise_against_occupied_cells(void) {
  sequence_t sequence;
  initSequence(&sequence, 0);

  game_t game;
  reset(&game, &sequence);

  rotatePiece(&game);
  dropPiece(&game);  // Drop rotated I; J appears
  dropPiece(&game);  // Drop J; L appears
  dropPiece(&game);  // Drop L; O appears
  dropPiece(&game);  // Drop O; S appears
  dropPiece(&game);  // Drop S; T appears
  dropPiece(&game);  // Drop T; Z appears
  dropPiece(&game);  // Drop Z; I appears again
  movePieceLeft(&game);
  tick(&game);
  assertEqualGrids(board_with_first_piece_r1_drop_and_drop_six_more_moved,
                      game.canvas.grid);
  // Can't rotate because the Z is in the way.
  TEST_ASSERT_FALSE(rotatePiece(&game));
}

void test_game_drop_points(void) {
  sequence_t sequence;
  initSequence(&sequence, 0);

  game_t game;
  reset(&game, &sequence);

  TEST_ASSERT_EQUAL(0, game.score);
  dropPiece(&game);

  // One point for each line dropped.
  TEST_ASSERT_EQUAL(18, game.score);
  dropPiece(&game);

  TEST_ASSERT_EQUAL(35, game.score);
}

void test_game_over(void) {
  sequence_t sequence;
  initSequence(&sequence, 0);

  game_t game;
  reset(&game, &sequence);

  // Drop pieces until the game ends.
  dropPiece(&game);
  dropPiece(&game);
  dropPiece(&game);
  dropPiece(&game);
  dropPiece(&game);
  dropPiece(&game);
  dropPiece(&game);
  dropPiece(&game);
  dropPiece(&game);
  TEST_ASSERT_FALSE(game.state == GAME_OVER);
  dropPiece(&game);
  TEST_ASSERT_TRUE(game.state == GAME_OVER);

  const char *gridAtGameOver = game.canvas.grid;

  // Moving and ticking do nothing.
  movePieceLeft(&game);
  movePieceRight(&game);
  rotatePiece(&game);
  tick(&game);
  tick(&game);
  tick(&game);
  TEST_ASSERT_EQUAL(gridAtGameOver, game.canvas.grid);
}

void test_game_drop_and_delete_rows_points(void) {
  sequence_t sequence;
  initSequence(&sequence, 0);

  game_t game;
  reset(&game, &sequence);

  setGrid(&(game.board), board0_grid);
  rotatePiece(&game);

  movePieceRight(&game);
  movePieceRight(&game);
  movePieceRight(&game);
  movePieceRight(&game);
  dropPiece(&game);

  // Score for rows dropped.
  TEST_ASSERT_EQUAL(16, game.score);

  // Delete rows marked for removal.
  tick(&game);

  // Score 16 rows dropped, and 1200 for a "tetris" (rows removed).
  TEST_ASSERT_EQUAL(1216, game.score);
}

int main(int argc, char** argv) {
  UNITY_BEGIN();

  // pieces.h
  RUN_TEST(test_clockwise);
  RUN_TEST(test_anticlockwise);
  RUN_TEST(test_init_shape);
  RUN_TEST(test_rotate_shape);
  RUN_TEST(test_update_grid);

  // board.h
  RUN_TEST(test_init_board);
  RUN_TEST(test_fill_with);
  RUN_TEST(test_fill_row_with);
  RUN_TEST(test_inside);
  RUN_TEST(test_collides_empty);
  RUN_TEST(test_collides_full);
  RUN_TEST(test_collides_bottom_piece);
  RUN_TEST(test_stick);
  RUN_TEST(test_no_empty_spaces_in_row);
  RUN_TEST(test_mark_dead_rows);
  RUN_TEST(test_remove_dead_rows);

  // sequence.h
  RUN_TEST(test_sequence);
  RUN_TEST(test_reshuffle);

  // game.h
  RUN_TEST(test_reset_game);
  RUN_TEST(test_game_sequence);
  RUN_TEST(test_render);
  RUN_TEST(test_move_left);
  RUN_TEST(test_move_right);
  RUN_TEST(test_move_left_then_tick);
  RUN_TEST(test_move_left_then_tick_down_see_new_piece);
  RUN_TEST(test_game_drop);
  RUN_TEST(test_game_rotate_clockwise);
  RUN_TEST(test_game_rotate_clockwise_against_edge);
  RUN_TEST(test_game_rotate_clockwise_orientation_resets);
  RUN_TEST(test_game_rotate_clockwise_against_occupied_cells);
  RUN_TEST(test_game_over);
  RUN_TEST(test_game_drop_points);
  RUN_TEST(test_game_drop_and_delete_rows_points);

  UNITY_END();
}

