#pragma once

typedef enum {
  UP,
  RIGHT,
  DOWN,
  LEFT,
} orientation_t;

orientation_t rotateClockwise(orientation_t from);

orientation_t rotateAntiClockwise(orientation_t from);

