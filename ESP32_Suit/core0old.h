/*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^
  
[CORE 0]
  Communicates with the suit's parts and their speakers and LEDs.
  Controls a DFPlayer Mini.
  Analyses the audio output to visualize the music accordingly.

Components:
  DFPlayer Mini
  LCD Monitor
  Helmet Matrices
    "Mouth" Matrix 1x 32x8 + 2x 8x8
    "Eye" Matrices 2x 8x8 + "Forehead" Matrix 1x 8x8
    Headband Matrix 32x8 + 8x8
  Resistor network to read which components are connected.

Pin definitions:
  ===========================
  ESP32   LCD
  ===========================
  3V3     VCC
  GND     GND
  D1      A0 (D/C)
  D2      CS (CS)
  RX      RESET (RES)
  D7      SDA (DIN)
  D5      SCK (CLK)
          LED (Left open)
  ============================
  ESP32   Ultra-Sonic Sensor
  ============================
  D3      Trigger
  D6      Echo
  ============================
  ESP32   DFPlayer
  ============================
  TX      RX (Play/Stop/Pause)
  RX      TX
  D8      If playing
  ============================
  ESP32   Helmet Matrices
  ============================
  D9      Din
  ============================
  ESP32   Parts
  ============================
  D4      Resistor Network check suit parts
  ============================

^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*/
#include <Adafruit_GFX.h>
#include <FastLED_NeoMatrix.h>
#include <FastLED.h>
#include "font.h"

// DFPLAYER MINI MP3 PLAYER //

#include "dfplayer.h"

// RGB MATRICES //

#include "anim.h"
#include "texts.h"

/// HELMET ///
#define PIN_MOUTH_LED 14
#define PIN_HEADBAND_LED 13
#define PIN_EYES_LED 12
#define PIN_EARS_LED 0

//// "Mouth" Matrix 1x 32x8 + 2x 8x8
#define MOUTH_WIDTH 8
#define MOUTH_HEIGHT 48
#define NUM_MOUTH_LEDS (MOUTH_WIDTH * MOUTH_HEIGHT) // 32x8 + 2x 8x8

//// "Eye" Matrices 2x 8x8 + "Forehead" Matrix 1x 8x8
#define NUM_EYES_LEDS 192 // 3x 8x8

//// Headband Matrix 32x8 + 8x8
#define HEADBAND_WIDTH 8
#define HEADBAND_HEIGHT 40
#define NUM_HEADBAND_LEDS (HEADBAND_WIDTH * HEADBAND_HEIGHT)

uint8_t brightness_mouth = 12;
uint8_t brightness_eyes = 12;
uint8_t brightness_headband = 12;

//// Ear Ring Strips 2x 35
#define NUM_EARS_LEDS 70 // 2x35
#define SPEED 30

uint8_t brightness_ears = 1;

DEFINE_GRADIENT_PALETTE( example_gp ) {
  0,    0, 0, 255,
  5,    0, 0, 255,
  80,    0, 255, 0,
  130,    0, 0, 255,
  135,    0, 0, 255,
  205,    255, 0, 255,
  238,    255, 0, 0,    // I'm making this my last color choice
  255,    255, 0, 0     // And then just duplicating it here
};

CRGBPalette16 myPal = example_gp;



/// Scrolling Text

unsigned long previous_ms = 0;
uint8_t animationFrame = 0;
uint8_t selectedMode = 0;

bool isAnimatingMouth = false;
bool isAnimatingHeadband = false;

String mouth_text;
int mouth_x;
unsigned long previous_mouth_ms;
uint8_t scroll_interval_mouth_ms = 50;

FastLED_NeoMatrix *mouth_matrix = new FastLED_NeoMatrix(leds_mouth, MOUTH_HEIGHT, MOUTH_WIDTH, MOUTH_WIDTH / 8, 1,
    NEO_MATRIX_TOP + NEO_MATRIX_LEFT + NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG);
  
String headband_text;
int headband_x;
unsigned long previous_headband_ms;
uint8_t scroll_interval_headband_ms = 450;

FastLED_NeoMatrix *headband_matrix = new FastLED_NeoMatrix(leds_headband, HEADBAND_HEIGHT, HEADBAND_WIDTH, HEADBAND_WIDTH / 8, 1,
  NEO_MATRIX_TOP + NEO_MATRIX_LEFT + NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG);

/// Color Theme

// Define a macro to convert RGB & GRB values to a uint16_t color value
#define RGB_TO_COLOR(r, g, b) (((uint16_t)r & 0xF8) << 8 | ((uint16_t)g & 0xFC) << 3 | ((uint16_t)b) >> 3)
#define GRB_TO_COLOR(g, r, b) (((uint16_t)r & 0xF8) << 8 | ((uint16_t)g & 0xFC) << 3 | ((uint16_t)b) >> 3)
const uint16_t COLORS_DATETIME[] PROGMEM = {
  GRB_TO_COLOR(200, 10, 0),   // 00:00 - 01:00 (Night Deep Red)
  GRB_TO_COLOR(180, 10, 0),   // 01:00 - 02:00 (Night Deep Red)
  GRB_TO_COLOR(160, 10, 0),   // 02:00 - 03:00 (Night Deep Red)
  GRB_TO_COLOR(140, 10, 0),   // 03:00 - 04:00 (Night Deep Red)
  GRB_TO_COLOR(150, 10, 0),   // 04:00 - 05:00 (Dawn Dark Red)
  GRB_TO_COLOR(180, 30, 0),   // 05:00 - 06:00 (Morning Red)
  GRB_TO_COLOR(200, 100, 0),  // 06:00 - 07:00 (Orange)
  GRB_TO_COLOR(200, 140, 10), // 07:00 - 08:00 (Orange)
  GRB_TO_COLOR(200, 180, 20), // 08:00 - 09:00 (Orange)
  GRB_TO_COLOR(200, 220, 30), // 09:00 - 10:00 (Orange)
  GRB_TO_COLOR(0, 0, 255),    // 10:00 - 11:00 (Blue)
  GRB_TO_COLOR(0, 0, 255),    // 11:00 - 12:00 (Blue)
  GRB_TO_COLOR(0, 0, 255),    // 12:00 - 13:00 (Blue)
  GRB_TO_COLOR(0, 0, 255),    // 13:00 - 14:00 (Blue)
  GRB_TO_COLOR(0, 0, 255),    // 14:00 - 15:00 (Blue)
  GRB_TO_COLOR(0, 0, 255),    // 15:00 - 16:00 (Blue)
  GRB_TO_COLOR(0, 0, 255),    // 16:00 - 17:00 (Blue)
  GRB_TO_COLOR(0, 0, 255),    // 17:00 - 18:00 (Blue)
  GRB_TO_COLOR(200, 220, 30), // 18:00 - 19:00 (Orange)
  GRB_TO_COLOR(200, 180, 20), // 19:00 - 20:00 (Orange)
  GRB_TO_COLOR(200, 140, 10), // 20:00 - 21:00 (Orange)
  GRB_TO_COLOR(200, 100, 0),  // 21:00 - 22:00 (Orange)
  GRB_TO_COLOR(250, 60, 0),   // 22:00 - 23:00 (Night Dark Red)
  GRB_TO_COLOR(230, 10, 0)    // 23:00 - 00:00 (Night Deep Red)
};

uint16_t COLORS_PIXEL[] = {
  GRB_TO_COLOR(17, 47, 75),   // DBLUE  0x112F4B
  RGB_TO_COLOR(228, 224, 60), // YELLO  0xE4E03C
  GRB_TO_COLOR(152, 10, 155), // PINK   0x980A9B
  GRB_TO_COLOR(211, 0, 0),    // RED    0x6F2610
  GRB_TO_COLOR(10, 9, 4),     // BLACK  0x000000
  GRB_TO_COLOR(225, 215, 225),// WHITE
  GRB_TO_COLOR(255, 165, 10), // ORANGE
  GRB_TO_COLOR(20, 168, 30),  // GREEN
  GRB_TO_COLOR(20, 215, 215), // CYAN
  GRB_TO_COLOR(40, 40, 215)   // BLUE
};

const uint16_t COLORS_PIXEL_ORIG[] = COLORS_PIXEL;

// Neondra suit

int analogPin = 0;
int raw = 0;
int Vin = 5;
float Vout = 0;
float R1 = 1000;
float R2 = 0;
float buffer = 0;

// Source:
// https://www.circuitbasics.com/arduino-ohm-meter/

// Examples of the resistors are in this sheet:
// https://docs.google.com/spreadsheets/d/1W_uhhL5tW4ke7RFwoA-nKQAPrWWVSFHEoSCmMDLBSd4/edit?usp=sharing
int getConnectedPiecesResistorValue() {
  raw = analogRead(analogPin);
  if (!raw) return 0;
  buffer = raw * Vin;
  Vout = buffer / 1024.0;
  buffer = (Vin / Vout) - 1;
  R2= R1 * buffer;
  Serial.print("Vout: ");
  Serial.println(Vout);
  Serial.print("R2: ");
  Serial.println(R2);
  delay(1000);
  return 0;
}

const double SUIT_PARTS_RESISTOR_VALUES[] PROGMEM = {
  1,      // 0: Torso
  2,      // 1: Left Claw
  4,      // 2: Right Claw
  8,      // 3: Left Foot
  16,     // 4: Right Foot
  32,     // 5: Tail
  64      // 6: Wings
};

bool connected_pieces[] = {
  false,  // 0: Torso
  false,  // 1: Left Claw
  false,  // 2: Right Claw
  false,  // 3: Left Foot
  false,  // 4: Right Foot
  false,  // 5: Tail
  false   // 6: Wings
};

bool[] getConnectedPieces() {
  bool connections[] = { false, false, false, false, false, false };
  /*
  TOTAL_VALUE - READ_RESISTOR = MISSING_IF_ANY

  [EXAMPLE: Wings missing]
  float value = 127 - 63 = 64;

  float margin = 0.5;

  for (uint8_t i = 0; i < 7; i++) {
    int part_value = SUIT_PARTS_RESISTOR_VALUES[i];
    // check the value with margin
    if (part.value > value - margin && part.value < value + margin) {
      // Aight, wing has a value of 64
      // 64-64 == 0 so only the wings are missing!
    }
  }
  */
  return connections;
}
bool isTorsoAttached() { return connected_pieces[0]; }
bool isLeftClawAttached() { return connected_pieces[1]; }
bool isRightClawAttached() { return connected_pieces[2]; }
bool isLeftFootAttached() { return connected_pieces[3]; }
bool isRightFootAttached() { return connected_pieces[4]; }
bool isTailAttached() { return connected_pieces[5]; }
bool isWingsAttached() { return connected_pieces[6]; }

// Communicates with the core1 to get information what the time is.
char* requestTimeUpdated(void) {
  return "12:24:48";
}

const char* getMatrixText(uint8_t mode) {
  if (mode == 1) return DEFAULT_TEXTS[random(20)];
  if (mode == 2) return RAVE_TEXTS[random(9)];
  return requestTimeUpdated();
}

// Clears all Matrixes and sets their color to 0x112F4B
void clearMatrices(void) {
  FastLED.setBrightness(brightness_mouth);
  fill_solid(leds_mouth, NUM_MOUTH_LEDS, CRGB(0x11, 0x2F, 0x4B));
  FastLED.setBrightness(brightness_eyes);
  fill_solid(leds_eyes, NUM_EYES_LEDS, CRGB(0x11, 0x2F, 0x4B));
  FastLED.setBrightness(brightness_headband);
  fill_solid(leds_headband, NUM_HEADBAND_LEDS, CRGB(0x11, 0x2F, 0x4B));
  FastLED.show();
}

void setMatrix(CRGB leds[], const uint8_t matrix[], uint8_t numColumns) {
  for (uint8_t col = 0; col < numColumns; col++) {
    for (uint8_t row = 0; row < 8; row++) {
      uint8_t i = pgm_read_byte(&(matrix[col * 8 + row]));
      leds[col * 8 + row] = COLORS_PIXEL[i];
    }
  }
  FastLED.show();
}

void textWipe(CRGB leds[], uint8_t num_leds, char* text) {

}

void setRingWipe(uint16_t color) {
  uint8_t half = round(NUM_EARS_LEDS / 2);
  for (uint8_t i = 0; i < half; i++) {
    leds_ears[i] = color;
    leds_ears[half + i] = color;
    FastLED.show();
    delay(SPEED);
  }
}

void SETUP_CORE_0(void) {
  // Initialize a hardware serial connection with the ESP8266 "brain"
  Serial.begin(9600);
  // Wait for serial connection to be established
  while (!Serial) { ; }
  
  // Initialize the Ultra Sonic Boop-the-snoot Sensor

  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  Serial.begin(9600); // Starts the serial communication

  // Initialize Matrices
  
  // Helmet
  FastLED.setBrightness(14);
  FastLED.addLeds<WS2812B, PIN_MOUTH_LED, GRB>(leds_mouth, NUM_MOUTH_LEDS);
  FastLED.addLeds<WS2812B, PIN_EYES_LED, GRB>(leds_eyes, NUM_EYES_LEDS);
  FastLED.addLeds<WS2812B, PIN_HEADBAND_LED, GRB>(leds_headband, NUM_HEADBAND_LEDS);

  mouth_x = MOUTH_WIDTH;
  mouth_matrix->begin();
  mouth_matrix->setTextWrap(false);
  mouth_matrix->setBrightness(brightness_mouth);
  mouth_matrix->setTextColor(RGB_TO_COLOR(228, 224, 60));

  headband_x = HEADBAND_WIDTH;
  headband_matrix->begin();
  headband_matrix->setTextWrap(false);
  headband_matrix->setBrightness(brightness_headband);
  headband_matrix->setTextColor(RGB_TO_COLOR(228, 224, 60));
  //headband_matrix->setFont( &FontElectroharmonix8x12Data[size] )

  //FastLED.addLeds<NEOPIXEL, PIN_EAR_LEFT_LED>(leds_earleft, NUM_EARS_LEDS);
  FastLED.addLeds<NEOPIXEL, PIN_EARS_LED>(leds_ears, NUM_EARS_LEDS);

  // Torso
  if (isTorsoAttached()) {
    ();
  }
  // Hands
  if (isLeftClawAttached()) {
    ();
  }
  if (isRightClawAttached()) {
    ();
  }
  // Legs
  if (isLeftFootAttached()) {
    ();
  }
  if (isRightFootAttached()) {
    ();
  }
  // Tail
  if (isTailAttached()) {
    ();
  }
  // Wing
  if (isWingsAttached()) {
    ();
  }

  mouth_text = "xx:xx:xx";
  headband_text = "xx:xx:xx";
  clearMatrices();
  setSuitMode(1);

  // TEST Startup Animation
  setMatrix(leds_eyes, startup_central_1, 24);
  delay(300);
  setMatrix(leds_eyes, startup_central_2, 24);
  delay(300);
  setMatrix(leds_eyes, startup_central_3, 24);
  delay(300);
  setMatrix(leds_eyes, startup_central_4, 24);
  delay(300);
  setMatrix(leds_eyes, startup_central_5, 24);
  delay(300);
}

/*
uint8_t[][320] mouthAnimationFrames;
uint8_t[][192] eyesAnimationFrames;
uint8_t[][384] headbandAnimationFrames;

uint8_t mouthAnimationFrameCount;
uint8_t mouthAnimationFrameCount;
uint8_t mouthAnimationFrameCount;
uint8_t totalAnimationFrameCount;
*/

uint8_t mouth_scrolling_text_timer;
uint8_t headband_scrolling_text_timer;

void setSuitMode(uint8_t mode) {
  Serial.println("Mode set to " + String(mode));
  selectedMode = mode;
  isAnimatingMouth = false;
  isAnimatingHeadband = false;
  mouth_text = getMatrixText(selectedMode);
  headband_text = getMatrixText(selectedMode);
  if (selectedMode == 0) { // Theme: Offline
    scroll_interval_headband_ms = 1000; // Slow
    return;
  }
  if (selectedMode == 1) { // Theme: Online/Default
    scroll_interval_headband_ms = 350; // Fast
    COLORS_PIXEL[0] = COLORS_PIXEL_ORIG[0];
    COLORS_PIXEL[1] = COLORS_PIXEL_ORIG[1];
    COLORS_PIXEL[2] = COLORS_PIXEL_ORIG[2];
    COLORS_PIXEL[3] = COLORS_PIXEL_ORIG[3];
    COLORS_PIXEL[4] = COLORS_PIXEL_ORIG[4];
    COLORS_PIXEL[5] = COLORS_PIXEL_ORIG[5];
    COLORS_PIXEL[6] = COLORS_PIXEL_ORIG[6];
    COLORS_PIXEL[7] = COLORS_PIXEL_ORIG[7];
    COLORS_PIXEL[8] = COLORS_PIXEL_ORIG[8];
    COLORS_PIXEL[9] = COLORS_PIXEL_ORIG[9];
    return;
  }
  if (selectedMode == 2) { // Theme: Rave
    COLORS_PIXEL[0] = RGB_TO_COLOR(211, 0, 0); // Background to YELLOW
    COLORS_PIXEL[1] = RGB_TO_COLOR(10, 9, 4);  // Primary to BLUE
    return;
  }
  if (selectedMode == 3) { // Theme: Cute
    COLORS_PIXEL[0] = GRB_TO_COLOR(152, 10, 155); // Background to PINK
    COLORS_PIXEL[1] = GRB_TO_COLOR(255, 165, 10); // Primary to ORANGE
    return;
  }
  if (selectedMode == 4) { // Theme: Fury
    COLORS_PIXEL[0] = GRB_TO_COLOR(211, 0, 0); // Background to red
    COLORS_PIXEL[1] = GRB_TO_COLOR(10, 9, 4);  // Primary to black
    return;
  }
}

void playAnim(uint8_t anim) {
  // Play an anim from start to finish and then return to default state.
}

/*
  Modified code of Ultrasonic Sensor HC-SR04 and Arduino Tutorial
  by Dejan Nedelkovski, https://www.howtomechatronics.com
*/
const int trigPin = 0;
const int echoPin = 4;
long duration;
int distance;

bool readBoopSensor(void) {
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2;
  // Prints the distance on the Serial Monitor
  Serial.print("Distance: ");
  Serial.println(distance);
  if (distance < 10) {
    Serial.println("Boop!");
    delay(2000);
    return true;
  }
  return false;
}

void checkControls(void) {

  // Check if data is available to read from ESP8266
  if (Serial.available() == 0) return;

  // ONLY READ WHAT HAS BEEN RECEIVED BY SERIAL
  //Serial.println("Nano Serial Received: " + Serial.read());
  byte serial_bytes = Serial.read();

  // MANUAL CONTROL
  if (serial_bytes == '1') playAnim(0); // Anim: SmugWink
  if (serial_bytes == '2') playAnim(1); // Anim:
  if (serial_bytes == '3') playAnim(2); // Anim:
  if (serial_bytes == '4') playAnim(3); // Anim:
  if (serial_bytes == '5') playAnim(4); // Anim:

  if (serial_bytes == 'q') setSuitMode(0); // Mode: Offline
  if (serial_bytes == 'w') setSuitMode(1); // Mode: Normal
  if (serial_bytes == 'e') setSuitMode(2); // Mode: Rave
  if (serial_bytes == 'r') setSuitMode(3); // Mode: Cute
  if (serial_bytes == 't') setSuitMode(4); // Mode: Fury

  if (serial_bytes == 'a') setRingWipe(RGB_TO_COLOR(255, 0, 0));
  if (serial_bytes == 's') setRingWipe(RGB_TO_COLOR(0, 255, 0));
  if (serial_bytes == 'd') setRingWipe(RGB_TO_COLOR(0, 0, 255));
  if (serial_bytes == 'f') setRingWipe(RGB_TO_COLOR(255, 255, 0));
  if (serial_bytes == 'g') setRingWipe(RGB_TO_COLOR(0, 255, 255));
  if (serial_bytes == 'h') setRingWipe(RGB_TO_COLOR(255, 0, 255));
  if (serial_bytes == 'j') setRingWipe(RGB_TO_COLOR(255, 100, 0));
  if (serial_bytes == 'k') setRingWipe(RGB_TO_COLOR(100, 0, 255));
}

void LOOP_CORE_0(void) {

  if (isAnimatingMouth) {
    fill_solid(leds_mouth, NUM_MOUTH_LEDS, COLORS_PIXEL[0]);
    mouth_matrix->setCursor(mouth_x, 0);
    mouth_matrix->print(mouth_text.c_str());
    uint8_t hours = 23; // PARSE_FIRST_2_CHARACTERS_OF_TIME; // "232202" => 23
    unsigned long current_mouth_ms = millis();
    // Scroll text from right to left
    if (scroll_interval_mouth_ms > 0) {
      if (current_mouth_ms - previous_mouth_ms >= scroll_interval_mouth_ms) {
        previous_mouth_ms = current_mouth_ms;
        if (--mouth_x < -(int)mouth_text.length() * 6) {
          mouth_x = MOUTH_HEIGHT;
          //mouth_matrix->setTextColor(COLORS_DATETIME[hours]); // update every hour
          mouth_text = DEFAULT_TEXTS[random(20)];
          isAnimatingMouth = false;
        }
        mouth_matrix->show();
      }
    }
  }
  
  if (isAnimatingHeadband) {
    fill_solid(leds_headband, NUM_HEADBAND_LEDS, COLORS_PIXEL[0]);
    headband_matrix->setCursor(headband_x, 0);
    headband_matrix->print(headband_text.c_str());
    uint8_t hours = 23; // PARSE_FIRST_2_CHARACTERS_OF_TIME; // "232202" => 23
    unsigned long current_headband_ms = millis();
    // Scroll text from right to left
    if (scroll_interval_headband_ms > 0) {
      if (current_headband_ms - previous_headband_ms >= scroll_interval_headband_ms) {
        previous_headband_ms = current_headband_ms;
        if (--headband_x < -(int)headband_text.length() * 6) {
          headband_x = HEADBAND_HEIGHT;
          //headband_matrix->setTextColor(COLORS_DATETIME[hours]); // update every hour
          headband_text = DEFAULT_TEXTS[random(20)];
          isAnimatingHeadband = false;
        }
        headband_matrix->show();
      }
    }
  }

  // 1-second timer without the use of delay
  unsigned long current_ms = millis();
  if (current_ms - previous_ms >= 1000) {
    previous_ms = current_ms;

    // If the mode has scrolling text, Offline is the only mode one which doesn't scroll
    if (selectedMode > 0) {
      // Mouth
      if (!isAnimatingMouth) {
        if (mouth_scrolling_text_timer == 0) {
          mouth_scrolling_text_timer = random(30, 255);
          isAnimatingMouth = true;
        }
        --mouth_scrolling_text_timer;
      }
      // Headband
      if (!isAnimatingHeadband) {
        if (headband_scrolling_text_timer == 0) {
          headband_scrolling_text_timer = random(30, 255);
          isAnimatingHeadband = true;
        }
        --headband_scrolling_text_timer;
      }
      //Serial.println(String(mouth_scrolling_text_timer) + " | " + String(headband_scrolling_text_timer) + " | " + String(isAnimatingMouth) + " | " + String(isAnimatingHeadband));
    }
    
    if (++animationFrame > 3) animationFrame = 0;

    //ANIM_DEFAULT_SMUGWINK
    
    /*if (!isAnimatingMouth) {
      if (animationFrame < mouthAnimationFrameCount) setMatrix(leds_mouth, mouthAnimationFrames[animationFrame], 48);
    }
    if (animationFrame < mouthAnimationFrameCount) {
      setMatrix(leds_eyes, eyesAnimationFrames[animationFrame], 24);
    }
    if (!isAnimatingHeadband) {
      if (animationFrame < headbandAnimationFrameCount) setMatrix(leds_headband, headbandAnimationFrames[animationFrame], 40);
    }*/

    if (animationFrame == 0) {
      
      if (!isAnimatingMouth) {
        setMatrix(leds_mouth, neondra_default_mouth_1, 48);
      }
      setMatrix(leds_eyes, neondra_default_eyes_1, 24);
      if (!isAnimatingHeadband) {
        setMatrix(leds_headband, neondra_default_headband_1, 40);
      }
      
    } else if (animationFrame == 1) {
      
      if (!isAnimatingMouth) setMatrix(leds_mouth, neondra_default_mouth_2, 48);
      setMatrix(leds_eyes, neondra_default_eyes_2, 24);
      
    } else if (animationFrame == 2) {
      
      setMatrix(leds_eyes, rave_central_0, 24);
      //setMatrix(leds_eyes, neondra_default_eyes_3, 24);
    }
  }

  // Update the Ear LEDs
  for (uint8_t i = 0; i < NUM_EARS_LEDS; i++) {
    uint8_t paletteIndex = map(i, 0, NUM_EARS_LEDS - 1, 0, 240);
    leds_ears[i] = ColorFromPalette(myPal, paletteIndex, brightness_ears, LINEARBLEND);
  }
  FastLED.show();
}