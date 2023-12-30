/*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^
  Author: Neondra Studio (by "Jentle")
  Project: Spitfire
  Year: 2023
  Website: https://neondra.com/
  Board: Generic ESP8266 Module
^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*/
#include <FastLED.h>

#define PIN_STRIP 2
#define PIN_CIRCLES 3
#define NUM_STRIP_LEDS 80
#define NUM_BIGCIRCLE_LEDS 35
#define NUM_SMALLCIRCLE_LEDS 24

// Try to connect to Neondra suit's ESP8266 if that exists.
// Ping every 6s.
const char* NEONDRA_SUIT_IP "127.168.1.34";

CRGB ledstrip[NUM_STRIP_LEDS];

void setup(void) {
  // Initialize the Matrices.

  // 80
  ledstrip = FastLED.addLeds<WS2812B, GRB>(PIN_STRIP, NUM_STRIP_LEDS);

  // 35x2
  // FastLED.addLeds<>();
  // FastLED.addLeds<>();

  // 24x2
  // FastLED.addLeds<>();
  // FastLED.addLeds<>();
}

void loop(void) {
  // Animate the matrices...
}