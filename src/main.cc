#ifndef UNIT_TEST

#include "Arduino.h"
#include "Adafruit_NeoPixel.h"
#include "src/game.h"

#define PIN        6
#define NUMPIXELS 200
// When setting up the NeoPixel library, we tell it how many pixels,
// and which pin to use to send signals. Note that for older NeoPixel
// strips you might need to change the third parameter -- see the
// strandtest example for more information on possible values.
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
}

void loop() {
}

#endif
