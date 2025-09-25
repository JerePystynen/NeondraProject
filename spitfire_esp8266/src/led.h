#include <FastLED.h>
#include <EEPROM.h>
#include <array>
#include <cstdint>
#include "./led_def.h"
#include "./led_matrix.h"
#include "./led_rings.h"

#define LED_PIN D6

//#define LED_PIN_MOSFET_R D1
//#define LED_PIN_MOSFET_G D2
//#define LED_PIN_MOSFET_B D3
//#define LED_PIN_MOSFET_W D4

void set_color_mode(uint8_t val) {
	EEPROM.put(0x00, val);
	color_mode = val;
}

void setup_leds() {
  color_mode = EEPROM.read(0x00);
	FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);

  LEFT_MATRIX = get_leds(0);
  //BOTH_MATRIX = get_leds(1);
  //SHAFT = get_leds(2);
  //HANDLE_RINGS = get_leds(3);
  //ALL_RINGS = get_leds(4);
  
  FastLED.showColor(get_color(color_mode));
  FastLED.setBrightness(10);
	FastLED.show();
}

// gets called every 60ms.
void update_leds(int offset_x, int offset_y) {
  update_matrix(offset_x, offset_y);
}