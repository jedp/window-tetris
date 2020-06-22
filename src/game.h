#pragma once

#include <piece.h>
#include <sequence.h>

const int H_BOARD = 20;
const int W_BOARD = 10;

const point_t START_COORDINATES = (struct point) { 0, 3 };
const point_t OFFSCREEN_COORDINATES = (struct point) { -4, 3 };

const bbox_t BOARD_AREA = (struct bbox) {
  (struct point) { 0, 0 },
  (struct point) { H_BOARD - 1, W_BOARD - 1}
};

typedef struct stats {
  int score;
} stats_t;

typedef enum state {
  PLAYING,
  SHOWING_FULL_ROWS,
  DELETING_FULL_ROWS,
  GAME_OVER,
} state_t;

class Game {
 public:
    Game(const Shape &canvas_,
         const Sequence &sequence_,
         void (*gameOverCallback_)(void));

    /** Set the board to an arbitrary grid. */
    void setGrid(const char *grid);

    /** Start the game. */
    void play();

    /** Move the piece left, if unobstructed. */
    bool moveLeft();

    /** Move the piece right, if unobstructed. */
    bool moveRight();

    /** Move the piece clockwise, if unobstructed. */
    bool rotateClockwise();

    /**
     * Drop the piece. It will stick where it lands.
     *
     * May result in rows being marked for deletion.
     */

    void drop();

    /**
     * Advance one arbitrary tick in time.
     *
     * When time advances, the current pieces falls one space, if unobstructed.
     * Additionally, we transition from showing full rows to obliterating them.
     */
    void tick();

    /**
     * Reset the game to to a clean start state.
     */
    void reset();

    /**
     * Get current stats.
     */
    stats_t getStats() const { return stats; }

 private:
    /** The canvas used for rendering. Mutate this after each move or tick. */
    Shape canvas;

    /** A Sequence for generating random numbers. */
    Sequence sequence;

    /** A function to call when the game is over. */
    void (*gameOverCallback)(void);

    /** Current stats. */
    stats_t stats;

    /** Current play state. */
    state_t state;

    /** Name of the current piece (I, J, L, etc.). */
    piece_name_t currentPieceName;

    /** The board on which pieces are accumulated when they stick. */
    Shape board;

    /** The set of pieces to play with. */
    Piece pieces[NUM_PIECES];

    /** Move the piece in direction dir, if unimpeded. */
    bool move(orientation_t dir);

    /** Cause the current piece to stick where it is on the board. */
    void stickCurrentPiece();

    /** Hide the current piece so it won't be visible in renderings. */
    void hideCurrentPiece();

    /** Pick the next piece and put it at the start location. */
    void produceNextPiece();

    /** Update score for number of rows removed. */
    void scoreRemovedRows(int rows);

    /** Update score for number of rows dropped. */
    void scoreDroppedRows(int rows);

    /**
     * Render the image of the game on the canvas.
     *
     * The rendering is a composite of the board and the piece.
     */
    void render();

    /**
     * Move to the game over state. Calls the gameOverCallback.
     */
    void gameOver();
};

