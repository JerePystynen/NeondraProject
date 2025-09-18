uint8_t brightness_matrix = 10;
uint8_t brightness_ears = 10;

#include <Adafruit_GFX.h>
#include <FastLED.h>
#include <FastLED_NeoMatrix.h>

#include "animation.h"
#include "matrix_texts.h"
#include "matrix_frames.h"
#include "ring.cpp"

//// "Mouth" Matrix 1x 32x8 + 2x 8x8
#define MOUTH_WIDTH 48
#define MOUTH_HEIGHT 8
#define NUM_MOUTH_LEDS (MOUTH_WIDTH * MOUTH_HEIGHT)  // 32x8 + 2x 8x8

#define NUM_LEFTEYE_LEDS 64
#define NUM_RIGHTEYE_LEDS 64
#define NUM_FOREHEAD_LEDS 64

//// Headband Matrix 32x8 + 8x8
#define HEADBAND_WIDTH 40
#define HEADBAND_HEIGHT 8
#define NUM_HEADBAND_LEDS (HEADBAND_WIDTH * HEADBAND_HEIGHT)

CRGB leds_matrix[NUM_MATRIX_LEDS];
CRGB leds_mouth[NUM_MOUTH_LEDS];
CRGB leds_lefteye[64];
CRGB leds_righteye[64];
CRGB leds_forehead[64];
CRGB leds_headband[NUM_HEADBAND_LEDS];

#define NUM_EARS_LEDS 70  // 2x35
CRGB leds_ears[NUM_EARS_LEDS];

/// Scrolling Text

uint8_t current_anim_frame_index = 0;
uint8_t current_anim_frame_target = 255;
int offset_x, offset_y;

unsigned long previous_ms = 0;
uint8_t animationFrame = 0;
uint8_t selectedMode = 0;

bool isAnimatingMouth = false;
bool isAnimatingHeadband = false;

String mouth_text;
uint8_t mouth_x;
unsigned long previous_mouth_ms;
uint8_t scroll_interval_mouth_ms = 50;

String headband_text;
uint8_t headband_x;
unsigned long previous_headband_ms;
uint16_t scroll_interval_headband_ms = 450;

// FastLED_NeoMatrix* mouth_matrix = new FastLED_NeoMatrix(leds_mouth, MOUTH_WIDTH, MOUTH_HEIGHT, MOUTH_WIDTH / 8, 1,
//                                                         NEO_MATRIX_TOP + NEO_MATRIX_LEFT + NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG);

// FastLED_NeoMatrix* headband_matrix = new FastLED_NeoMatrix(leds_headband, HEADBAND_WIDTH, HEADBAND_HEIGHT, HEADBAND_WIDTH / 8, 1,
//                                                            NEO_MATRIX_TOP + NEO_MATRIX_LEFT + NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG);

// Define a macro to convert RGB & GRB values to a uint16_t color value
#define RGB_TO_COLOR(r, g, b) (((uint16_t)r & 0xF8) << 8 | ((uint16_t)g & 0xFC) << 3 | ((uint16_t)b) >> 3)
#define GRB_TO_COLOR(g, r, b) (((uint16_t)r & 0xF8) << 8 | ((uint16_t)g & 0xFC) << 3 | ((uint16_t)b) >> 3)

const uint16_t COLORS_DATETIME[] PROGMEM = {
  GRB_TO_COLOR(200, 10, 0),    // 00:00 - 01:00 (Night Deep Red)
  GRB_TO_COLOR(180, 10, 0),    // 01:00 - 02:00 (Night Deep Red)
  GRB_TO_COLOR(160, 10, 0),    // 02:00 - 03:00 (Night Deep Red)
  GRB_TO_COLOR(140, 10, 0),    // 03:00 - 04:00 (Night Deep Red)
  GRB_TO_COLOR(150, 10, 0),    // 04:00 - 05:00 (Dawn Dark Red)
  GRB_TO_COLOR(180, 30, 0),    // 05:00 - 06:00 (Morning Red)
  GRB_TO_COLOR(200, 100, 0),   // 06:00 - 07:00 (Orange)
  GRB_TO_COLOR(200, 140, 10),  // 07:00 - 08:00 (Orange)
  GRB_TO_COLOR(200, 180, 20),  // 08:00 - 09:00 (Orange)
  GRB_TO_COLOR(200, 220, 30),  // 09:00 - 10:00 (Orange)
  GRB_TO_COLOR(0, 0, 255),     // 10:00 - 11:00 (Blue)
  GRB_TO_COLOR(0, 0, 255),     // 11:00 - 12:00 (Blue)
  GRB_TO_COLOR(0, 0, 255),     // 12:00 - 13:00 (Blue)
  GRB_TO_COLOR(0, 0, 255),     // 13:00 - 14:00 (Blue)
  GRB_TO_COLOR(0, 0, 255),     // 14:00 - 15:00 (Blue)
  GRB_TO_COLOR(0, 0, 255),     // 15:00 - 16:00 (Blue)
  GRB_TO_COLOR(0, 0, 255),     // 16:00 - 17:00 (Blue)
  GRB_TO_COLOR(0, 0, 255),     // 17:00 - 18:00 (Blue)
  GRB_TO_COLOR(200, 220, 30),  // 18:00 - 19:00 (Orange)
  GRB_TO_COLOR(200, 180, 20),  // 19:00 - 20:00 (Orange)
  GRB_TO_COLOR(200, 140, 10),  // 20:00 - 21:00 (Orange)
  GRB_TO_COLOR(200, 100, 0),   // 21:00 - 22:00 (Orange)
  GRB_TO_COLOR(250, 60, 0),    // 22:00 - 23:00 (Night Dark Red)
  GRB_TO_COLOR(230, 10, 0)     // 23:00 - 00:00 (Night Deep Red)
};

uint16_t COLORS_PIXEL[] = {
  GRB_TO_COLOR(17, 47, 75),     // DBLUE  0x112F4B
  RGB_TO_COLOR(228, 224, 60),   // YELLO  0xE4E03C
  GRB_TO_COLOR(152, 10, 155),   // PINK   0x980A9B
  GRB_TO_COLOR(211, 0, 0),      // RED    0x6F2610
  GRB_TO_COLOR(10, 9, 4),       // BLACK  0x000000
  GRB_TO_COLOR(225, 215, 225),  // WHITE
  GRB_TO_COLOR(255, 165, 10),   // ORANGE
  GRB_TO_COLOR(20, 168, 30),    // GREEN
  GRB_TO_COLOR(20, 215, 215),   // CYAN
  GRB_TO_COLOR(40, 40, 215)     // BLUE
};

const uint16_t COLORS_PIXEL_ORIG[] = {
  GRB_TO_COLOR(17, 47, 75),     // DBLUE  0x112F4B
  RGB_TO_COLOR(228, 224, 60),   // YELLO  0xE4E03C
  GRB_TO_COLOR(152, 10, 155),   // PINK   0x980A9B
  GRB_TO_COLOR(211, 0, 0),      // RED    0x6F2610
  GRB_TO_COLOR(10, 9, 4),       // BLACK  0x000000
  GRB_TO_COLOR(225, 215, 225),  // WHITE
  GRB_TO_COLOR(255, 165, 10),   // ORANGE
  GRB_TO_COLOR(20, 168, 30),    // GREEN
  GRB_TO_COLOR(20, 215, 215),   // CYAN
  GRB_TO_COLOR(40, 40, 215)     // BLUE
};

const char* get_time_updated(void) {
  String timeString = "12:24:48";
  return timeString.c_str();
}

const char* get_matrix_text(uint8_t mode) {
  if (mode == 1) return DEFAULT_TEXTS[random(20)];
  if (mode == 2) return RAVE_TEXTS[random(9)];
  return get_time_updated();
}

// // Clears all Matrixes and sets their color to 0x112F4B
// void clear_matrices(void) {
//   FastLED.setBrightness(brightness_matrix);
//   fill_solid(leds_matrix, NUM_MATRIX_LEDS, CRGB(0x11, 0x2F, 0x4B));
//   //fill_solid(leds_mouth, NUM_MOUTH_LEDS, CRGB(0x11, 0x2F, 0x4B));
//   //fill_solid(leds_lefteye, NUM_RIGHTEYE_LEDS, CRGB(0x11, 0x2F, 0x4B));
//   //fill_solid(leds_righteye, NUM_LEFTEYE_LEDS, CRGB(0x11, 0x2F, 0x4B));
//   //fill_solid(leds_forehead, NUM_FOREHEAD_LEDS, CRGB(0x11, 0x2F, 0x4B));
//   //fill_solid(leds_headband, NUM_HEADBAND_LEDS, CRGB(0x11, 0x2F, 0x4B));
//   FastLED.show();
// }

void set_matrix(CRGB leds[], const uint8_t matrix[], uint8_t numColumns, uint16_t offset) {
  for (uint8_t col = 0; col < numColumns; col++) {
    for (uint8_t row = 0; row < 8; row++) {
      uint8_t i = pgm_read_byte(&(matrix[col * 8 + row + offset]));
      leds[col * 8 + row + offset] = COLORS_PIXEL[i];
    }
  }
  FastLED.show();
}

void textWipe(CRGB leds[], uint8_t num_leds, char* text) {
}

uint8_t mouth_scrolling_text_timer;
uint8_t headband_scrolling_text_timer;

void set_color_mode(uint8_t mode) {
  Serial.println("Mode set to " + String(mode));
  selectedMode = mode;
  isAnimatingMouth = false;
  isAnimatingHeadband = false;
  mouth_text = get_matrix_text(selectedMode);
  headband_text = get_matrix_text(selectedMode);
  if (selectedMode == 0) {               // Theme: Offline
    scroll_interval_headband_ms = 1000;  // Slow
    return;
  }
  if (selectedMode == 1) {              // Theme: Online/Default
    scroll_interval_headband_ms = 350;  // Fast
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
  if (selectedMode == 2) {                      // Theme: Rave
    COLORS_PIXEL[0] = RGB_TO_COLOR(211, 0, 0);  // Background to YELLOW
    COLORS_PIXEL[1] = RGB_TO_COLOR(10, 9, 4);   // Primary to BLUE
    return;
  }
  if (selectedMode == 3) {                         // Theme: Cute
    COLORS_PIXEL[0] = GRB_TO_COLOR(152, 10, 155);  // Background to PINK
    COLORS_PIXEL[1] = GRB_TO_COLOR(255, 165, 10);  // Primary to ORANGE
    return;
  }
  if (selectedMode == 4) {                      // Theme: Fury
    COLORS_PIXEL[0] = GRB_TO_COLOR(211, 0, 0);  // Background to red
    COLORS_PIXEL[1] = GRB_TO_COLOR(10, 9, 4);   // Primary to black
    return;
  }
}

bool is_playing_matrix_anim = false;

// Play an animation from start to finish and then return to default state.
void play_matrix_anim(Animation animation) {
  is_playing_matrix_anim = true;
  // anim
  // delay(3000);
  is_playing_matrix_anim = false;
}

void set_matrix_with_offset(const uint8_t framedata[], int matrix_width, int matrix_height, int offset_x, int offset_y, int offset_index, uint8_t direction) {
  for (uint8_t col = 0; col < matrix_width; col++) {
    for (uint8_t row = 0; row < matrix_height; row++) {
      int offset = offset = offset_x * matrix_height + offset_y;
      //if (direction == 0) offset = offset_x * matrix_height + offset_y;
      //if (direction == 1) offset = offset_x * matrix_height;

     

      uint8_t i = pgm_read_byte(&(framedata[col * matrix_height + row + offset + offset_index]));
      leds_matrix[col * 8 + row + offset_index] = COLORS_PIXEL[i];
    }
  }
}

void set_matrix(const uint8_t framedata[], int matrix_width, int matrix_height, int offset_x, int offset_y, int offset_index) {
  /*for (int col = 0; col < matrix_width; col++) {
    for (int row = 0; row < 8; row++) {
      int offset = (row + (offset_y * 0.1)) * matrix_width + (col + (offset_x * 0.5));
      int ledIndex = (col % 2 == 0 ? row * matrix_width + col : (row + 1) * matrix_width - col - 1) + offset_index;
      uint8_t i = pgm_read_byte(&(framedata[col * 8 + row + offset_index]));
      CRGB color = (offset >= 0 && offset < matrix_width * matrix_height) ? COLORS_PIXEL[i] : COLORS_PIXEL[0];
      leds_matrix[col * 8 + row + offset_index] = color;
    }
  }*/
  for (int x = 0; x < matrix_width; x++) {
    for (int y = 0; y < matrix_height; y++) {
      // Apply offsets.
      int offset = (y + (offset_y * 0.1)) * matrix_width + (x + (offset_x * 0.5));
      uint8_t i = pgm_read_byte(&(framedata[offset + offset_index]));
      CRGB color = (offset >= 0 && offset < matrix_width * matrix_height) ? COLORS_PIXEL[i] : COLORS_PIXEL[0];

      // Calculate the ledIndex considering the snakelike pattern and offsets.
      // Even rows are indexed regularly, odd rows are indexed in reverse.
      int ledIndex = (y % 2 == 0 ? y * matrix_width + x : (y + 1) * matrix_width - x - 1) + offset_index;

      // Assign color to the LED.
      // If inside bounds, get the color by the value inside the frame array.
      leds_matrix[ledIndex] = color;
    }
  }
}

void set_matrix_frame(const uint8_t target_index, const uint8_t framedata[]) {
  //if (current_anim_frame_index == current_anim_frame_target) return;
  //current_anim_frame_target = target_index;
  fill_solid(leds_matrix, NUM_MATRIX_LEDS, CRGB(0x11, 0x2F, 0x4B));
  set_matrix_with_offset(framedata, MOUTH_WIDTH, 8, offset_x, offset_y, 0, 0);                                                                             // Mouth
  set_matrix_with_offset(framedata, 8, 8, offset_x, offset_y, NUM_MOUTH_LEDS, 1);                                                                          // Left Eye
  set_matrix_with_offset(framedata, 8, 8, offset_x, offset_y, NUM_MOUTH_LEDS + NUM_LEFTEYE_LEDS, 1);                                                       // Forehead
  set_matrix_with_offset(framedata, 8, 8, offset_x, offset_y, NUM_MOUTH_LEDS + NUM_LEFTEYE_LEDS + NUM_FOREHEAD_LEDS, 1);                                   // Right Eye
  set_matrix_with_offset(framedata, HEADBAND_WIDTH, 8, offset_x, offset_y, NUM_MOUTH_LEDS + NUM_LEFTEYE_LEDS + NUM_FOREHEAD_LEDS + NUM_RIGHTEYE_LEDS, 1);  // Headband
  FastLED.show();
}

void setup_suit_leds(void) {
  FastLED.addLeds<WS2812B, PIN_HELMET_MATRIX, GRB>(leds_matrix, NUM_MATRIX_LEDS);
  FastLED.setBrightness(brightness_matrix);

  //mouth_x = MOUTH_WIDTH;
  //mouth_matrix->begin();
  //mouth_matrix->setTextWrap(false);
  //mouth_matrix->setBrightness(brightness_matrix);
  //mouth_matrix->setTextColor(RGB_TO_COLOR(228, 224, 60));

  //headband_x = HEADBAND_WIDTH;
  //headband_matrix->begin();
  //headband_matrix->setTextWrap(false);
  //headband_matrix->setBrightness(brightness_matrix);
  //headband_matrix->setTextColor(RGB_TO_COLOR(228, 224, 60));
  ////headband_matrix->setFont(&FontElectroharmonix8x12Data[size]);

  // FastLED.addLeds<NEOPIXEL, PIN_LEDS_OTHER>(leds_ears, NUM_EARS_LEDS);

  // // Torso
  // if (is_torso_attached()) {
  // }
  // // Hands
  // if (isLeftClawAttached()) {
  // }
  // if (isRightClawAttached()) {
  // }
  // // Legs
  // if (isLeftFootAttached()) {
  // }
  // if (isRightFootAttached()) {
  // }
  // // Tail
  // if (isTailAttached()) {
  // }
  // // Wing
  // if (isWingsAttached()) {
  // }

  mouth_text = "xx:xx:xx";
  headband_text = "xx:xx:xx";

  set_color_mode(1);
  set_matrix_frame(0, neondra_normal_1);
}

void update_gyro(void) {
  // 60ms timer
  get_gyro(&offset_x, &offset_y);
  if (current_anim_frame_index == 0) set_matrix_frame(0, neondra_normal_1);
  if (current_anim_frame_index == 1) set_matrix_frame(1, neondra_normal_2);
  if (current_anim_frame_index == 2) set_matrix_frame(2, neondra_normal_3);
}

void update_matrix_framedata(void) {
  // 1000ms timer
  current_anim_frame_index++;
  if (current_anim_frame_index > 2) current_anim_frame_index = 0;
}