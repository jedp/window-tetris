
#include <Adafruit_NeoPixel.h>

#define PIN 6
#define NUM_PIXELS 16

Adafruit_NeoPixel pixels(NUM_PIXELS, PIN, NEO_RGB + NEO_KHZ800);

void setup() {
  pixels.begin();
  pixels.show();
}

void loop() {
  for (long hue = 0; hue < 3 * 65536; hue += 8) {
    pixels.setPixelColor(0, pixels.gamma32(pixels.ColorHSV(hue)));
    pixels.setPixelColor(1, pixels.gamma32(pixels.ColorHSV(hue)));
    pixels.setPixelColor(2, pixels.gamma32(pixels.ColorHSV(hue)));
    pixels.setPixelColor(3, pixels.gamma32(pixels.ColorHSV(hue)));
    pixels.show();
  }
}

