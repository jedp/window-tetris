#ifndef UNIT_TEST

#include "Arduino.h"
// #include "Adafruit_NeoPixel.h"
#include <game.h>

#define PIN        6
// When setting up the NeoPixel library, we tell it how many pixels,
// and which pin to use to send signals. Note that for older NeoPixel
// strips you might need to change the third parameter -- see the
// strandtest example for more information on possible values.
// Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

sequence_t sequence;
game_t game;

void setup() {
  // analogRead maps to 0..1023, so there are only 1024 possible games
  // with this approach for generating the random seed. Also, the values
  // are probably not randomly distributed.
  initSequence(&sequence, analogRead(0));
  reset(&game, &sequence);
}

void loop() {
}

#endif
