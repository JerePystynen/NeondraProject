#pragma once

#include <FastLED.h>
#include "./helpers.h"

#define MATRIX_WIDTH 8
#define MATRIX_HEIGHT 8
#define NUM_LEDS_MATRIX (MATRIX_WIDTH * MATRIX_HEIGHT)
#define NUM_LEDS_RING_SMALL 24
#define NUM_LEDS_RING_BIG 35
#define NUM_LEDS_SHAFT 124

constexpr int LED_PARTS[] = {
  // [Handle]
  NUM_LEDS_RING_SMALL,
  NUM_LEDS_MATRIX,
  NUM_LEDS_RING_SMALL,
  //NUM_LEDS_RING_BIG,
  //NUM_LEDS_MATRIX,
  // [Blade]
  //NUM_LEDS_SHAFT,
  //NUM_LEDS_RING_SMALL,
  //NUM_LEDS_RING_SMALL
};

constexpr int NUM_LEDS = array_sum(LED_PARTS);
CRGB leds[NUM_LEDS];

CRGB* get_leds(uint8_t start, uint8_t end) {
  if (end <= start || end > NUM_LEDS) return nullptr;
  return &leds[start];
}

CRGB* get_leds(uint8_t type) {
  switch (type) {
    default: return nullptr;
    case 0: return get_leds(24, 24 + NUM_LEDS_MATRIX);
    // case 1: return BOTH_MATRIX;
    // case 2: return SHAFT;
    // case 3: return HANDLE_RINGS;
    // case 4: return ALL_RINGS;
  }
}

// Individual parts of the LEDs can be referred by their 'Type', such as 'LEFT_MATRIX'.
CRGB* LEFT_MATRIX   = nullptr;
CRGB* BOTH_MATRIX   = nullptr;
CRGB* SHAFT         = nullptr;
CRGB* HANDLE_RINGS  = nullptr;
CRGB* ALL_RINGS     = nullptr;

CRGB COLORS[] = {
	CRGB::CadetBlue,    // 0: Default/Inactive
	CRGB::YellowGreen,  // 1: Active
	CRGB::IndianRed,    // 2: Highlight
  CRGB::Orange,
  CRGB::Violet,
  CRGB::Pink,
  CRGB::Cyan
};

uint8_t color_mode = 0;

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
