#include <FastLED.h>
#include "g.h"

#define LED_PIN D6
#define NUM_LEDS 64
#define MATRIX_WIDTH 8
#define MATRIX_HEIGHT 8

CRGB leds[NUM_LEDS];

uint8_t art[] = {
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 1, 0, 0, 1, 0, 0,
  0, 0, 1, 0, 0, 1, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 1, 0, 0, 0, 0, 1, 0,
  0, 0, 1, 0, 0, 1, 0, 0,
  0, 0, 0, 1, 1, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 2
};

void setup(void) {
  Serial.begin(115200);
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(10);
  setup_gyro();
}

void loop(void) {
  int offset_x, offset_y;
  get_gyro(&offset_y, &offset_x);

  FastLED.clear();
  for (int y = 0; y < MATRIX_HEIGHT; y++) {
    for (int x = 0; x < MATRIX_WIDTH; x++) {
      // Calculate the ledIndex considering the snakelike pattern and offsets.
      // Even rows are indexed regularly.
      // Odd rows are indexed in reverse.
      int ledIndex = y % 2 == 0 ? y * MATRIX_WIDTH + x : (y + 1) * MATRIX_WIDTH - x - 1;

      // Apply offsets.
      int offset = (y + offset_y) * MATRIX_WIDTH + (x + offset_x);

      // Assign color to the LED.
      // If inside bounds, get the color by the value inside the frame array.
      leds[ledIndex] = (offset >= 0 && offset < NUM_LEDS) ? (art[offset] == 1 ? CRGB::Green : CRGB::Blue) : CRGB::Blue;
    }
  }
  FastLED.show();

  delay(60);
}