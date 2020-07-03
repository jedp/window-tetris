[![Build Status](https://travis-ci.org/jedp/window-tetris.svg?branch=master)](https://travis-ci.org/jedp/window-tetris)

# Window Tetris

Arduino-based Tetris game to render using a large grid of 5050 RGB LEDs,
controlled by means of a Covid-friendly foot controller.

The goal is to mount the display outside, facing the street, so that passers-by
can play using the foot controller on the sidewalk.

## Current Status

Done: game engine that plays and scores according to the classic rules.

TBD:

- Build and drive 5050 LED display (parts ordered)
- Build and be driven by external foot controller (parts ordered)

## Unit Tests

On native: `pio test -e native`

On attached Arduino device: `pio test -e uno`

The native tests are run automatically by travis. But please be sure to test
on a device before submitting changes.

## LED Reference

They look the same, but they're not!

[SK6812](https://cdn-shop.adafruit.com/product-files/2686/SK6812MINI_REV.01-1-2.pdf)

Viewed from the front, pin 3 is behind the corner with a triangular notch taken out.
Oriented with pin 3 at the lower right, pin 1 is at the upper left (again, seen from
the side with the LED visible).

1. Data out
2. Ground
3. Data in
4. 5V in

[WS2812B](https://cdn-shop.adafruit.com/product-files/3094/WS2812B.pdf)

Viewed from the front, pin 3 is behind the corner with a triangular notch taken out.
Oriented with pin 3 at the lower right, pin 1 is at the upper left (again, seen from
the side with the LED visible).

1. 5V in
2. Data out
3. Ground
4. Data in

