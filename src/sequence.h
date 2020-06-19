#pragma once

#include <piece.h>

/**
 * Interface for a random number generator used by Sequence.
 */
class Random {

  public:
    Random(long seed);
    long choice(long from, long to);

  private:
    long seed;
};

/**
 * Endlessly generates the next piece to play.
 */
class Sequence {

  public:
    Sequence(long seed);

    /**
     * Return the next piece to display.
     *
     * According to the rules of Tetris, each of the seven pieces will be
     * generated once before another is generated again.
     */
    piece_name_t next();

  private:
    Random random;
    piece_name_t seq[NUM_PIECES];
    int current_piece_index;
    void reshuffle();
    void reset();
};
