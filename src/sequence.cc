#include <sequence.h>

void swap(piece_name_t *seq, int i, int j);

Sequence::Sequence(int seed) {
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
  piece_name_t temp[NUM_PIECES];
  for (uint8_t i = 0; i < NUM_PIECES; ++i) {
    swap(seq, i, NUM_PIECES - i);
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


