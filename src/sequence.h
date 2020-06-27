#ifndef SRC_SEQUENCE_H_
#define SRC_SEQUENCE_H_

#include <stdint.h>

#include <pieces.h>

typedef struct {
  shape_name_t shapes[NUM_SHAPES];
  uint8_t iteration;
} sequence_t;

void initSequence(sequence_t *sequence, uint32_t seed);

void reshuffle(sequence_t *sequence);

shape_name_t next(sequence_t *sequence);

#endif  // SRC_SEQUENCE_H_

