#include <FastLED.h>
#include "./led_def.h"

uint8_t timer_index = 0;
uint8_t frame = 0;

constexpr uint8_t get_pixel(std::size_t frame, std::size_t index) {
  if (frame >= 0 && frame < 14 && index >= 0 && index < 64) {
    return art[frame][index];
  }
  return 0;
}

void update_matrix(int offset_x, int offset_y) {
  // Frame switch timer loop.
  timer_index = timer_index + 1;
  if (timer_index > 4) {
    frame = frame + 1;
    if (frame > 13) {
      frame = 0;
    }
    timer_index = 0;
  }

  for (uint8_t y = 0; y < MATRIX_HEIGHT; y++) {
    for (uint8_t x = 0; x < MATRIX_WIDTH; x++) {
      // Calculate the ledIndex considering the snakelike pattern and offsets.
      // Even rows are indexed regularly.
      // Odd rows are indexed in reverse.
      int ledIndex = y % 2 == 0 ? y * MATRIX_WIDTH + x : (y + 1) * MATRIX_WIDTH - x - 1;

      // Apply offsets.
      int offset = (y + offset_y) * MATRIX_WIDTH + (x + offset_x);
      
      bool is_inside_matrix = (offset >= 0 && offset < NUM_LEDS);

      // Assign color to the LED.
      // If inside bounds, get the color by the value inside the frame array.
      LEFT_MATRIX[ledIndex] = is_inside_matrix ? get_color(get_pixel(frame, offset)) : get_color(0);
    }
  }
  FastLED.show();
}