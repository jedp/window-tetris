#pragma once

#include <piece.h>

/**
 * Endlessly generates the next piece to play.
 */
class Sequence {

  public:
    /**
     * Seed to use for RNG.
     */
    Sequence(int seed);

    /**
     * Return the next piece to display.
     *
     * According to the rules of Tetris, each of the seven pieces will be
     * generated once before another is generated again.
     */
    piece_name_t next();

  private:
    piece_name_t seq[NUM_PIECES];
    int current_piece_index;
    void reshuffle();
    void reset();
};
