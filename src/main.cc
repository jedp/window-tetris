#ifndef UNIT_TEST

#include "Arduino.h"
// #include "Adafruit_NeoPixel.h"
#include <game.h>

#define ROTATE     7
#define LEFT       8
#define RIGHT      9
#define DROP      10

#define PIN        6
// When setting up the NeoPixel library, we tell it how many pixels,
// and which pin to use to send signals. Note that for older NeoPixel
// strips you might need to change the third parameter -- see the
// strandtest example for more information on possible values.
// Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

sequence_t sequence;
game_t game;

uint32_t tick_ms = 0;
uint32_t serialUpdate_ms = 0;
uint32_t debounceRotate_ms = 0;
uint32_t debounceLeft_ms = 0;
uint32_t debounceRight_ms = 0;
uint32_t debounceDrop_ms = 0;

uint32_t tickInterval_ms = 500;
uint32_t shortInterval_ms = 50;
uint32_t longInterval_ms = 100;

void setup() {
  // Controllers
  pinMode(ROTATE, INPUT_PULLUP);
  pinMode(LEFT, INPUT_PULLUP);
  pinMode(RIGHT, INPUT_PULLUP);
  pinMode(DROP, INPUT_PULLUP);

  // analogRead maps to 0..1023, so there are only 1024 possible games
  // with this approach for generating the random seed. Also, the values
  // are probably not randomly distributed.
  initSequence(&sequence, analogRead(0));
  reset(&game, &sequence);

  Serial.begin(28800);
}

void loop() {
  if (game.state == GAME_OVER) {
    reset(&game, &sequence);
  }

  if (digitalRead(ROTATE) == LOW) {
    if (millis() > debounceRotate_ms + shortInterval_ms) {
      debounceRotate_ms = millis();
      rotatePiece(&game);
    }
  }

  if (digitalRead(LEFT) == LOW) {
    if (millis() > debounceLeft_ms + shortInterval_ms) {
      debounceLeft_ms = millis();
      movePieceLeft(&game);
    }
  }

  if (digitalRead(RIGHT) == LOW) {
    if (millis() > debounceRight_ms + shortInterval_ms) {
      debounceRight_ms = millis();
      movePieceRight(&game);
    }
  }

  if (digitalRead(DROP) == LOW) {
    if (millis() > debounceDrop_ms + longInterval_ms) {
      debounceDrop_ms = millis();
      dropPiece(&game);
    }
  }

  if (millis() > tick_ms + tickInterval_ms) {
    tick_ms = millis();
    tick(&game);
  }

  if (millis() > serialUpdate_ms + longInterval_ms) {
    // For debugging. This is very slow and blocking.
    serialUpdate_ms = millis();
    Serial.print("----------");
    Serial.println(game.canvas.grid);
  }
}

#endif
