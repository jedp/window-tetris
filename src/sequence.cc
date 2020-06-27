#include <sequence.h>

#include <string.h>

#ifdef ARDUINO
#include "Arduino.h"
#endif

void swapElems(shape_name_t *list, uint8_t p, uint8_t q) {
  shape_name_t temp;
  temp = list[p];
  list[p] = list[q];
  list[q] = temp;
}

void initSequence(sequence_t *sequence, uint32_t seed) {
#ifdef ARDUINO
  randomSeed(seed);
#endif

  shape_name_t names[] = { I, J, L, O, S, T, Z };
  memcpy(sequence->shapes, names, NUM_SHAPES * sizeof(shape_name_t));
  sequence->iteration = 0;

  reshuffle(sequence);
}

void reshuffle(sequence_t *sequence) {
  for (uint8_t i = 0; i < NUM_SHAPES; ++i) {
#ifdef ARDUINO
    uint8_t k = random(i, NUM_SHAPES);
#else  // We are running unit tests.
    uint8_t k = i;
#endif
    swapElems(sequence->shapes, k, NUM_SHAPES - 1 - k);
  }

  sequence->iteration = 0;
}

shape_name_t next(sequence_t *sequence) {
  shape_name_t shape = sequence->shapes[sequence->iteration++];

  if (sequence->iteration == NUM_SHAPES) {
    sequence->iteration = 0;
    reshuffle(sequence);
  }

  return shape;
}

