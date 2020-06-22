#pragma once

#include <piece.h>

/**
 * Interface for a random number generator used by Sequence.
 */
class Random {
 public:
    explicit Random(uint32_t seed_);
    uint32_t choice(uint32_t from, uint32_t to);

 private:
    uint32_t seed;
};

/**
 * Endlessly generates the next piece to play.
 */
class Sequence {
 public:
    explicit Sequence(uint32_t seed);

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
