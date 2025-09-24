#include <FastLED.h>
#include <EEPROM.h>
#include <array>
#include <cstdint>

#define LED_PIN D6

#define MATRIX_WIDTH 8
#define MATRIX_HEIGHT 8
#define NUM_LEDS_MATRIX MATRIX_WIDTH * MATRIX_HEIGHT

#define NUM_SHAFT_LEDS 80

#define NUM_BIGRING_LEDS 35
#define NUM_SMALLRING_LEDS 24
#define NUM_RINGS_LEDS (NUM_BIGRING_LEDS * 2 + NUM_SMALLRING_LEDS * 2)

// [Handle]
// Ring1			0-23
// Ring2			24-47
// Matrix1		48-111
// Matrix2		112-175
// Ring3			176-209
// Ring4			210-244
// [Blade]
// 40 LSTRIP (A)	245-284
// 40 LSTRIP (B)	285-324
#define NUM_LEDS (NUM_SMALLRING_LEDS * 2 + NUM_LEDS_MATRIX * 2 + NUM_BIGRING_LEDS * 2 + NUM_SHAFT_LEDS)

CRGB leds[NUM_LEDS];

#define X 1
#define _ 0

constexpr std::array<std::array<uint8_t, 64>, 14> art = {{
  // art1
  {{
    _,_,_,_,_,_,_,_,
    _,_,X,_,_,X,_,_,
    _,_,X,X,X,X,_,_,
    _,X,X,X,X,X,X,_,
    _,X,X,X,X,X,X,_,
    _,_,X,X,X,X,_,_,
    _,_,_,X,X,_,_,_,
    _,_,_,_,_,_,_,_
  }},
  // art2
  {{
    _,_,_,_,_,_,_,_,
    _,_,X,_,_,X,_,_,
    _,_,X,X,X,X,_,_,
    _,X,X,_,_,X,X,_,
    _,X,X,_,_,X,X,_,
    _,_,X,X,X,X,_,_,
    _,_,_,X,X,_,_,_,
    _,_,_,_,_,_,_,_
  }},
  // art3
  {{
    _,_,_,_,_,_,_,_,
    _,_,X,_,_,X,_,_,
    _,_,X,X,X,X,_,_,
    _,X,_,_,_,_,X,_,
    _,X,_,_,_,_,X,_,
    _,_,X,_,_,X,_,_,
    _,_,_,X,X,_,_,_,
    _,_,_,_,_,_,_,_
  }},
  // art4
  {{
    _,_,_,_,_,_,_,_,
    _,_,X,_,_,X,_,_,
    _,_,_,_,_,_,_,_,
    _,X,_,_,_,_,X,_,
    _,X,_,_,_,_,X,_,
    _,_,_,_,_,_,_,_,
    _,_,_,X,X,_,_,_,
    _,_,_,_,_,_,_,_
  }},
  // art5
  {{
    _,_,_,_,_,_,_,_,
    _,_,_,_,_,_,_,_,
    _,_,_,_,_,_,_,_,
    _,_,_,_,_,_,_,_,
    _,_,_,_,_,_,_,_,
    _,_,_,_,_,_,_,_,
    _,_,_,_,_,_,_,_,
    _,_,_,_,_,_,_,_
  }},
  // art6
  {{
    _,_,_,_,_,_,_,_,
    _,_,_,_,_,_,_,_,
    _,_,_,_,_,_,_,_,
    _,_,_,X,X,_,_,_,
    _,_,_,X,X,_,_,_,
    _,_,_,_,_,_,_,_,
    _,_,_,_,_,_,_,_,
    _,_,_,_,_,_,_,_
  }},
  // art7
  {{
    _,_,_,_,_,_,_,_,
    _,_,_,_,_,_,_,_,
    _,_,_,X,X,_,_,_,
    _,_,X,X,X,X,_,_,
    _,_,_,X,X,_,_,_,
    _,_,_,_,_,_,_,_,
    _,_,_,_,_,_,_,_,
    _,_,_,_,_,_,_,_
  }},
  // art8
  {{
    _,_,_,_,_,_,_,_,
    _,_,_,X,X,_,_,_,
    _,_,X,_,_,X,_,_,
    _,_,X,_,_,X,_,_,
    _,_,X,_,_,X,_,_,
    _,_,_,X,X,_,_,_,
    _,_,_,_,_,_,_,_,
    _,_,_,_,_,_,_,_
  }},
  // art9
  {{
    _,_,_,_,_,_,_,_,
    _,_,_,X,X,_,_,_,
    _,_,X,_,_,X,_,_,
    _,X,X,_,_,X,X,_,
    _,_,X,_,_,X,_,_,
    _,_,X,X,X,X,_,_,
    _,X,_,_,_,_,X,_,
    _,_,_,_,_,_,_,_
  }},
  // art10
  {{
    _,_,_,X,X,_,_,_,
    _,_,_,X,X,_,_,_,
    _,_,X,_,_,X,_,_,
    X,X,_,_,_,_,X,X,
    _,X,_,_,_,_,X,_,
    _,_,X,X,X,X,_,_,
    _,X,_,_,_,_,X,_,
    X,_,_,_,_,_,_,X
  }},
  // art11
  {{
    _,_,_,X,X,_,_,_,
    _,_,X,_,_,X,_,_,
    _,X,_,_,_,_,X,_,
    X,_,_,_,_,_,_,X,
    X,_,_,_,_,_,_,X,
    _,X,_,_,_,_,X,_,
    _,_,X,_,_,X,_,_,
    _,X,_,_,_,_,X,_
  }},
  // art12
  {{
    _,_,_,_,_,_,_,_,
    _,_,X,_,_,_,_,_,
    _,_,_,_,_,_,_,_,
    _,_,_,X,_,_,X,_,
    _,_,_,_,_,_,_,_,
    _,_,_,_,X,_,_,_,
    _,_,_,_,_,_,_,_,
    _,X,_,_,_,_,_,X
  }},
  // art13
  {{
    _,_,_,_,_,_,_,_,
    _,_,_,_,_,_,_,_,
    _,_,X,_,_,_,_,_,
    _,_,_,_,_,_,_,_,
    _,_,_,_,_,_,X,_,
    _,_,_,_,_,_,_,_,
    _,_,_,_,X,_,_,_,
    _,X,_,_,_,_,_,X
  }},
  // art14
  {{
    _,_,_,_,_,_,_,_,
    _,_,_,_,_,_,_,_,
    _,_,_,_,_,_,_,_,
    _,_,_,_,_,_,_,_,
    _,_,_,_,_,_,_,_,
    _,_,_,_,_,_,_,_,
    _,_,_,_,_,_,_,_,
    _,_,_,_,_,_,_,_
  }}
}};

#undef X
#undef _

constexpr uint8_t get_pixel(uint8_t frame, uint8_t row, uint8_t col) {
  return art[frame][row * 8 + col];
}

CRGB COLORS[] = {
	CRGB::CadetBlue,		// 0: Default/Inactive
	CRGB::YellowGreen,		// 1: Active
	CRGB::IndianRed,		// 2: Highlight
};

uint8_t color_mode = 0;

void set_color_mode(uint8_t val) {
	EEPROM.put(0x00, val);
	color_mode = val;
}

CRGB get_color(uint8_t index) {
	if (color_mode == 0) {
		if (index == 0) {
			return COLORS[0];
		} else if (index == 1) {
			return COLORS[1];
		} else if (index == 2) {
			return COLORS[2];
		}
	} else if (color_mode == 1) {
		if (index == 0) {
			return COLORS[1];
		} else if (index == 1) {
			return COLORS[2];
		} else if (index == 2) {
			return COLORS[0];
		}
	} else if (color_mode == 2) {
		if (index == 0) {
			return COLORS[2];
		} else if (index == 1) {
			return COLORS[1];
		} else if (index == 2) {
			return COLORS[0];
		}
	}
	return COLORS[index];
}

void setup_leds() {
	color_mode = EEPROM.read(0x00);
	FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
	FastLED.setBrightness(10);
	for (uint8_t i = 0; i < NUM_LEDS; i++) {
		leds[i] = COLORS[color_mode];
	}
	FastLED.show();
}

int frame = 0;

void update_leds(int offset_x, int offset_y) {
	FastLED.clear();
  for (int y = 0; y < MATRIX_HEIGHT; y++) {
    for (int x = 0; x < MATRIX_WIDTH; x++) {
      // Calculate the ledIndex considering the snakelike pattern and offsets.
      // Even rows are indexed regularly.
      // Odd rows are indexed in reverse.
      int ledIndex = y % 2 == 0 ? y * MATRIX_WIDTH + x : (y + 1) * MATRIX_WIDTH - x - 1;

      // Apply offsets.
      int row = (y + offset_y) * MATRIX_WIDTH;
      int column = (x + offset_x);
      int offset = (y + offset_y) * MATRIX_WIDTH + (x + offset_x);

      // Assign color to the LED.
      // If inside bounds, get the color by the value inside the frame array.
      leds[ledIndex] = (offset >= 0 && offset < NUM_LEDS) ? get_color(get_pixel(frame, row, column)) : get_color(0);
    }
  }
  FastLED.show();
}