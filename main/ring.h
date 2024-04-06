//// Ear Ring Strips 2x 35
#define SPEED 30

DEFINE_GRADIENT_PALETTE(example_gp){
  0, 0, 0, 255,
  5, 0, 0, 255,
  80, 0, 255, 0,
  130, 0, 0, 255,
  135, 0, 0, 255,
  205, 255, 0, 255,
  238, 255, 0, 0,  // I'm making this my last color choice
  255, 255, 0, 0   // And then just duplicating it here
};

CRGBPalette16 myPal = example_gp;

void set_ear_leds(uint16_t color) {
  uint8_t half = round(NUM_EARS_LEDS / 2);
  for (uint8_t i = 0; i < half; i++) {
    leds_ears[i] = color;
    leds_ears[half + i] = color;
    FastLED.show();
    delay(SPEED);
  }
}