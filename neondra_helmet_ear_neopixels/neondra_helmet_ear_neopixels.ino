//***************************************************************
// This example displays the full range of a custom gradient
// palette over the full range of a pixel string.  This is done
// by mapping the palette index to the pixel number.
//
// More info on gradient palettes here:
// https://github.com/FastLED/FastLED/wiki/Gradient-color-palettes
//
// *Also Note*:
// In order to support smooth blending for "circular" palettes,
// each color blends with "the next", and in the case of the last
// color, "the next" is the first color since it wraps around,
// so the last 15 numbers are blending back toward index 0.
// Sometime circular blending is very useful, other times it
// might not be what you want.  You can use index 240 as your
// last value to avoid blending.
//
// For this example I decided to Not have the display wrap back
// around, so I'm specifically using a palette index of 0-240.
//
// Marc Miller, Oct 2018
// https://github.com/marmilicious/FastLED_examples/blob/master/gradient_palette_full_range.ino
//***************************************************************

#include "FastLED.h"
#define LED_TYPE WS2812B
#define DATA_PIN 11
#define CLOCK_PIN 13
#define NUM_LEDS 32
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];
#define BRIGHTNESS  128

DEFINE_GRADIENT_PALETTE( example_gp ) {
    0,    0,0,255,
    5,    0,0,255,
   80,    0,255,0,
  130,    0,0,255, 
  135,    0,0,255, 
  205,    255,0,255,
  238,    255,0,0, // I'm making this my last color choice
  255,    255,0,0  // And then just duplicating it here
};
CRGBPalette16 myPal = example_gp;  

void setup() {
  Serial.begin(9600);
  delay(500);
  //FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  //FastLED.addLeds<LED_TYPE, DATA_PIN, CLOCK_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  //FastLED.setBrightness(BRIGHTNESS);
}

void loop () {
  for (uint8_t i = 0; i < NUM_LEDS; i++) {
    uint8_t paletteIndex = map(i, 0, NUM_LEDS - 1, 0, 240);
    leds[i] = ColorFromPalette(myPal, paletteIndex, BRIGHTNESS, LINEARBLEND);
  }
  FastLED.show();
  FastLED.delay(1000);
}
