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

