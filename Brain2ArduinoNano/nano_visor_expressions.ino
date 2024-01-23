// Author: Neondra Studio
// Project: Project Neondra
// Year: 2023
// Source: https://neondra.com/

#include <FastLED.h>

// MATRIX DATA OUTPUTS
#define PIN_MOUTH_LED D2
//#define PIN_HEADBAND_LED 3
#define PIN_FOREHEAD_LED 4
#define PIN_EYE_LEFT_LED 5
#define PIN_EYE_RIGHT_LED 6

//#define NUM_MATRICES 5
#define NUM_MOUTH_LEDS 384
//#define NUM_HEADBAND_LEDS 320
#define NUM_FOREHEAD_LEDS 64
#define NUM_EYE_LEDS 64

// LED SETTINGS
#define BRIGHTNESS 20

// EXPRESSIONS (MOUTH, HEADBAND, FOREHEAD, EYE_LEFT, EYE_RIGHT)
#include "Exp_Default.h"
#include "Exp_Fury.h"
#include "Exp_Love.h"

const CRGB customColors[] = {
  CRGB(17, 47, 75),   // DBLUE 0x112F4B
  CRGB(228, 224, 60), // YELLO 0xE4E03C
  CRGB(152, 10, 155), // PINK  0x980A9B
  CRGB(211, 0, 0),    // RED   0x6F2610
  CRGB(10, 9, 4)      // BLACK 0x000000
};

CRGB mouth_leds[NUM_MOUTH_LEDS];
CRGB forehead_leds[NUM_FOREHEAD_LEDS];
CRGB eyeleft_leds[NUM_EYE_LEDS];
CRGB eyeright_leds[NUM_EYE_LEDS];

void setup() {
  Serial.begin(9600);
  FastLED.addLeds<WS2812, PIN_MOUTH_LED, GRB>(mouth_leds, NUM_MOUTH_LEDS);
  //FastLED.addLeds<WS2812, PIN_FOREHEAD_LED, GRB>(forehead_leds, NUM_FOREHEAD_LEDS);
  //FastLED.addLeds<WS2812, PIN_EYE_LEFT_LED, GRB>(eyeleft_leds, NUM_EYE_LEDS);
  //FastLED.addLeds<WS2812, PIN_EYE_RIGHT_LED, GRB>(eyeright_leds, NUM_EYE_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
  setMatrix320(mouth_leds, neondra_default_mouth_1);
  setMatrix64(forehead_leds, neondra_default_forehead_1);
  //setMatrix64(eyeleft_leds, neondra_default_eyeleft_1);
  //setMatrix64(eyeright_leds, neondra_default_eye_right_1);
  animate();
}

// Clears all Matrixes and sets their color to 0x112F4B
void clearMatrix() {
  FastLED.setBrightness(BRIGHTNESS);
  fill_solid(mouth_leds, NUM_MOUTH_LEDS, CRGB(0x11, 0x2F, 0x4B));
  //fill_solid(forehead_leds, NUM_FOREHEAD_LEDS, CRGB(0x11, 0x2F, 0x4B));
  //fill_solid(eyeleft_leds, NUM_EYE_LEDS, CRGB(0x11, 0x2F, 0x4B));
  //fill_solid(eyeright_leds, NUM_EYE_LEDS, CRGB(0x11, 0x2F, 0x4B));
  FastLED.show();
}

void setMatrix64(const CRGB leds[64], const uint8_t matrix[64]) {
  for (uint8_t col = 0; col < 8; col++) {
    for (uint8_t row = 0; row < 8; row++) {
      uint8_t i = pgm_read_byte(&(matrix[col * 8 + row]));
      leds[col * 8 + row] = customColors[i];
    }
  }
  FastLED.show();
}

void setMatrix320(const CRGB leds[320], const uint8_t matrix[320]) {
  for (uint8_t col = 0; col < 40; col++) {
    for (uint8_t row = 0; row < 8; row++) {
      uint8_t i = pgm_read_byte(&(matrix[col * 8 + row]));
      leds[col * 8 + row] = customColors[i];
    }
  }
  FastLED.show();
}

uint8_t abc = 0;
void animate() {
  while (abc < 1) {
    //setMatrix64(eyeleft_leds, neondra_default_eyeleft_wink);
    //setMatrix64(eyeright_leds, neondra_default_eyeright_closed);
    //setMatrix64(forehead_leds, neondra_love_forehead_1);
    setMatrix320(mouth_leds, neondra_default_mouth_2);
    delay(2000);
    //setMatrix64(eyeleft_leds, neondra_default_eyeleft_open);
    //setMatrix64(eyeright_leds, neondra_default_eyeright_open);
    //setMatrix64(forehead_leds, neondra_default_forehead_1);
    setMatrix320(mouth_leds, neondra_default_mouth_1);
    delay(2000);
  }
}

void loop() {
  if (Serial.available() > 0) {
    uint8_t value = Serial.parseInt();
    switch (value) {
      case 0:
        Serial.println("Clearing Matrix...");
        clearMatrix();
        break;
      case 1:
        Serial.println("Switching to Matrix 1");
        //setMatrix320(neondra_default_mouth_1);
        //setMatrix64(forehead_leds, neondra_angry_forehead_1);
        //setMatrix64(eyeleft_leds, neondra_default_eyeleft_1);
        //setMatrix64(eyeright_leds, neondra_default_eyeright_1);
        break;
      case 2:
        Serial.println("Switching to Matrix 2");
        //setMatrix320(neondra_default_mouth_2);
        //setMatrix64(forehead_leds, neondra_default_forehead_1);
        //setMatrix64(eyeleft_leds, neondra_default_eyeleft_2);
        //setMatrix64(eyeright_leds, neondra_default_eyeright_2);
        break;
      default:
        Serial.println("Invalid input. Please enter 1, 2, or 3.");
        break;
    }
    // Clear the Serial buffer
    while (Serial.available() > 0) {
        Serial.read();
    }
    delay(200); // Optional delay between matrix switches
  }
}
