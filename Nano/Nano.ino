/*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^
  Author: Neondra Studio (by "Jentle")
  Project: Project Neondra V1
  Year: 2024
  Website: https://neondra.com/
  Components:
    + Nano (aka. "Brain_2"
    + 2x35 WS2812B Rings (Ears)
  [Optional:]
    + 2x35 WS2812B Rings (Legs)
^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*/
#include <FastLED.h>
#include <FastLED_NeoMatrix.h>

// RGB MATRICES //

// Modes (MOUTH, HEADBAND, EYE_LEFT, EYE_RIGHT, FOREHEAD)
#include "matrix_default.h"
#include "matrix_rave.h"
#include "matrix_fury.h"
#include "matrix_love.h"
#include "visor_texts.h"

/// HELMET ///
//// Mouth Matrix
#define PIN_MOUTH_LED 2
#define NUM_MOUTH_LEDS 320

//// Eye Matrices
#define PIN_EYE_LEFT_LED 5
#define PIN_EYE_RIGHT_LED 12
#define NUM_EYE_LEDS 64

//// Forehead Matrix
#define PIN_FOREHEAD_LED 4
#define NUM_FOREHEAD_LEDS 64

//// Headband Matrix
#define PIN_HELMET_HEADBAND 3
#define HEADBAND_WIDTH 8
#define HEADBAND_HEIGHT 40
#define NUM_HEADBAND_LEDS (HEADBAND_WIDTH * HEADBAND_HEIGHT)
#define BRIGHTNESS_HEADBAND 14

//// Ear Ring Strips
#define PIN_EAR_LEFT_LED 13
#define PIN_EAR_RIGHT_LED 14
#define NUM_EAR_LEDS 35
#define SPEED 30

//// Helmet CRGB Arrays
CRGB mouth_leds[NUM_MOUTH_LEDS];
CRGB headband_leds[NUM_HEADBAND_LEDS];
CRGB eyeleft_leds[NUM_EYE_LEDS];
CRGB eyeright_leds[NUM_EYE_LEDS];
CRGB forehead_leds[NUM_FOREHEAD_LEDS];
CRGB earleft_leds[NUM_EAR_LEDS];
CRGB earright_leds[NUM_EAR_LEDS];

/// TORSO AND OTHER ///
//// Other CRGB Arrays
// CRGB torso_leds[];
// CRGB arm_leds[];
// CRGB leg_leds[];
// CRGB wing_leds[];
// CRGB tail_leds[NUM_TAIL_LEDS];

/// Scrolling Text

String headband_text;
int headband_x = 0;
int pass = 0;
unsigned long previousMillis = 0;

const uint8_t SCROLL_INTERVAL_MS = 500;

FastLED_NeoMatrix *headband_matrix = new FastLED_NeoMatrix(headband_leds, HEADBAND_HEIGHT, HEADBAND_WIDTH, HEADBAND_WIDTH / 8, 1,
    NEO_MATRIX_TOP  + NEO_MATRIX_LEFT + NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG);

/// Color Theme

const uint16_t COLORS_DATETIME[] PROGMEM = {
  headband_matrix->Color(200, 10, 0),   // 00:00 - 01:00 (Night Deep Red)
  headband_matrix->Color(180, 10, 0),   // 01:00 - 02:00 (Night Deep Red)
  headband_matrix->Color(160, 10, 0),   // 02:00 - 03:00 (Night Deep Red)
  headband_matrix->Color(140, 10, 0),   // 03:00 - 04:00 (Night Deep Red)
  headband_matrix->Color(150, 10, 0),   // 04:00 - 05:00 (Dawn Dark Red)
  headband_matrix->Color(180, 30, 0),   // 05:00 - 06:00 (Morning Red)
  headband_matrix->Color(200, 100, 0),  // 06:00 - 07:00 (Orange)
  headband_matrix->Color(200, 140, 10), // 07:00 - 08:00 (Orange)
  headband_matrix->Color(200, 180, 20), // 08:00 - 09:00 (Orange)
  headband_matrix->Color(200, 220, 30), // 09:00 - 10:00 (Orange)
  headband_matrix->Color(0, 0, 255),    // 10:00 - 11:00 (Blue)
  headband_matrix->Color(0, 0, 255),    // 11:00 - 12:00 (Blue)
  headband_matrix->Color(0, 0, 255),    // 12:00 - 13:00 (Blue)
  headband_matrix->Color(0, 0, 255),    // 13:00 - 14:00 (Blue)
  headband_matrix->Color(0, 0, 255),    // 14:00 - 15:00 (Blue)
  headband_matrix->Color(0, 0, 255),    // 15:00 - 16:00 (Blue)
  headband_matrix->Color(0, 0, 255),    // 16:00 - 17:00 (Blue)
  headband_matrix->Color(0, 0, 255),    // 17:00 - 18:00 (Blue)
  headband_matrix->Color(200, 220, 30), // 18:00 - 19:00 (Orange)
  headband_matrix->Color(200, 180, 20), // 19:00 - 20:00 (Orange)
  headband_matrix->Color(200, 140, 10), // 20:00 - 21:00 (Orange)
  headband_matrix->Color(200, 100, 0),  // 21:00 - 22:00 (Orange)
  headband_matrix->Color(250, 60, 0),   // 22:00 - 23:00 (Night Dark Red)
  headband_matrix->Color(230, 10, 0)    // 23:00 - 00:00 (Night Deep Red)
};

const CRGB COLORS_PIXEL[] = {
  CRGB(17, 47, 75),   // DBLUE 0x112F4B
  CRGB(228, 224, 60), // YELLO 0xE4E03C
  CRGB(152, 10, 155), // PINK  0x980A9B
  CRGB(211, 0, 0),    // RED   0x6F2610
  CRGB(10, 9, 4)      // BLACK 0x000000
};

// Neondra suit
// Returns true if the torso is attached and returns a signal.
bool isTorsoAttached() {
  return false;
}

bool isWingsAttached() {
  return false;
}

bool isTailAttached() {
  return false;
}

bool isClawsAttached() {
  return false;
}

bool isLegsAttached() {
  return false;
}

void initializeLeds() {
  // Helmet
  FastLED.addLeds<WS2812, PIN_MOUTH_LED, GRB>(mouth_leds, NUM_MOUTH_LEDS);
  FastLED.addLeds<WS2812, PIN_FOREHEAD_LED, GRB>(forehead_leds, NUM_FOREHEAD_LEDS);
  FastLED.addLeds<WS2812, PIN_EYE_LEFT_LED, GRB>(eyeleft_leds, NUM_EYE_LEDS);
  FastLED.addLeds<WS2812, PIN_EYE_RIGHT_LED, GRB>(eyeright_leds, NUM_EYE_LEDS);
  FastLED.addLeds<NEOPIXEL, PIN_HELMET_HEADBAND>(headband_leds, NUM_HEADBAND_LEDS);
  headband_matrix->begin();
  headband_matrix->setTextWrap(false);
  headband_matrix->setBrightness(BRIGHTNESS_HEADBAND);
  headband_matrix->setTextColor(COLORS_DATETIME[0]);
  FastLED.addLeds<NEOPIXEL, PIN_EAR_LEFT_LED>(earleft_leds, NUM_EAR_LEDS);
  FastLED.addLeds<NEOPIXEL, PIN_EAR_RIGHT_LED>(earright_leds, NUM_EAR_LEDS);
  // Torso
  if (isTorsoAttached()) {
    
  }
  // Hands
  if (isClawsAttached()) {
    
  }
  // Legs
  if (isLegsAttached()) {
    
  }
  // Wing
  if (isWingsAttached()) {
    
  }
  // Tail
  if (isTailAttached()) {
    
  }
  headband_text = "xx:xx:xx";
}

// Communicates with the Brain_1 to get information what the time is.
String requestTimeUpdated(void) {
  return "";
}

// Display the time on the NeoPixel headband_matrix
void headbandDatetimeLoop(void) {
  headband_matrix->fillScreen(0);
  headband_matrix->setCursor(headband_x, 0);
  headband_text = requestTimeUpdated();
  headband_matrix->print(headband_text.c_str());

  uint8_t hours = 23; // PARSE_FIRST_2_CHARACTERS_OF_TIME; // "232202" => 23
  
  unsigned long currentMillis = millis();
  // Scroll text from right to left
  if (SCROLL_INTERVAL_MS > 0) {
    if (currentMillis - previousMillis >= SCROLL_INTERVAL_MS) {
      previousMillis = currentMillis;
      if (--headband_x < -(int)headband_text.length() * 6) {
        headband_x = headband_matrix->width();
        headband_matrix->setTextColor(COLORS_DATETIME[hours]); // update every hour
      }
    }
  }
  headband_matrix->show();
}

// 0: Blue
// 1: Red
// 2: Green
// 3: Yellow
void setColorTheme(uint8_t color) {
  // Add a hue to the colors that the loop is displaying.
}

// Clears all Matrixes and sets their color to 0x112F4B
void clearMatrices(void) {
  FastLED.setBrightness(BRIGHTNESS_HEADBAND);
  fill_solid(mouth_leds, NUM_MOUTH_LEDS, CRGB(0x11, 0x2F, 0x4B));
  fill_solid(forehead_leds, NUM_FOREHEAD_LEDS, CRGB(0x11, 0x2F, 0x4B));
  fill_solid(eyeleft_leds, NUM_EYE_LEDS, CRGB(0x11, 0x2F, 0x4B));
  fill_solid(eyeright_leds, NUM_EYE_LEDS, CRGB(0x11, 0x2F, 0x4B));
  FastLED.show();
}

void setMatrix(CRGB leds[], const uint8_t headband_matrix[], uint8_t numColumns) {
  for (uint8_t col = 0; col < numColumns; col++) {
    for (uint8_t row = 0; row < 8; row++) {
      uint8_t i = pgm_read_byte(&(headband_matrix[col * 8 + row]));
      leds[col * 8 + row] = COLORS_PIXEL[i];
    }
  }
  FastLED.show();
}

void setMatrix64(CRGB leds[64], const uint8_t headband_matrix[64]) {
  setMatrix(leds, headband_matrix, (uint8_t)64);
}

void setMatrix320(CRGB leds[320], const uint8_t headband_matrix[320]) {
  setMatrix(leds, headband_matrix, (uint8_t)320);
}

void setup(void) {
  // Initialize a hardware serial connection with the ESP8266 "brain"
  Serial.begin(9600);
  // Wait for serial connection to be established
  while (!Serial) { ; }
}

void loop(void) {
  // Check if data is available to read from ESP8266
  if (Serial.available() > 0) {
    // ONLY READ WHAT HAS BEEN RECEIVED BY SERIAL
    //Serial.println("Nano Serial Received: " + Serial.read());
  }
}
