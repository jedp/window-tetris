#include <sequence.h>

#ifdef ARDUINO
#include "Arduino.h"
#endif

void swap(piece_name_t *seq, int i, int j);

Random::Random(long seed) {
  this->seed = seed;

#ifdef ARDUINO
  randomSeed(seed);
#endif
}

long Random::choice(long from, long to) {
#ifdef ARDUINO
  return random(from, to);
#else
  return from;
#endif
}

Sequence::Sequence(long seed) :random(seed) {
  reshuffle();
}

piece_name_t Sequence::next() {
  piece_name_t nextPiece = seq[current_piece_index++];

  if (current_piece_index == NUM_PIECES) {
    reshuffle();
  }

  return nextPiece;
}

void Sequence::reshuffle() {
  reset();

  // No-op; Swaps them back to their original order.
  for (uint8_t i = 0; i < NUM_PIECES; ++i) {
    uint8_t k = random.choice(i, NUM_PIECES);
    swap(seq, k, NUM_PIECES - k);
  }
}

void Sequence::reset() {
  current_piece_index = 0;
  for (uint8_t i = 0; i < NUM_PIECES; ++i) {
    seq[i] = (PieceName)i;
  }
}

void swap(piece_name_t *seq, int i, int j) {
  piece_name_t temp;
  temp = seq[i];
  seq[i] = seq[j];
  seq[j] = temp;
}

