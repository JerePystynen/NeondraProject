#include <FastLED.h>
#include "./led_def.h"

#define NUM_SMALLRING_LEDS 35
#define RING_SPEED 30

uint8_t ring_1_position = 0;

CRGB strip_ring_1[NUM_SMALLRING_LEDS];

void color_wipe(uint32_t color, int wait) {
  for (int i = 0; i < NUM_SMALLRING_LEDS; i++) {
    strip_ring_1[i] = color;
    FastLED.delay(wait);
  }
}

DEFINE_GRADIENT_PALETTE(example_gp) {
  0,   0,   0,   255,
  5,   0,   0,   255,
  80,  0,   255, 0,
  130, 0,   0,   255,
  135, 0,   0,   255,
  205, 255, 0,   255,
  238, 255, 0,   0,
  255, 255, 0,   0
};
CRGBPalette16 ring_palette = example_gp;
uint8_t BRIGHTNESS = 10;

void rotate_ring(uint8_t pos) {
  for (uint8_t i = 0; i < NUM_LEDS; i++) {
    uint8_t paletteIndex = map(i, 0, NUM_LEDS - 1, 0, 240);
    LEFT_MATRIX[i] = ColorFromPalette(ring_palette, paletteIndex, BRIGHTNESS, LINEARBLEND);
  }
  //FastLED.show();
  //FastLED.delay(1000);
}

void update_led_rings(void) {
  // Rotate ring clockwise.
  ring_1_position = ring_1_position + 1;
  if (ring_1_position == 360) {
    ring_1_position = 0;
  }
  rotate_ring(ring_1_position);
}
