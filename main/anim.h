#pragma region ANIM_CLASS

#define NUM_MATRIX_LEDS 896
#define NUM_MOUTH_MATRIX_LEDS 384
#define NUM_LEFTEYE_MATRIX_LEDS 64
#define NUM_RIGHTEYE_MATRIX_LEDS 64
#define NUM_FOREHEAD_MATRIX_LEDS 64
#define NUM_HEADBAND_MATRIX_LEDS 320

#include <array>
#include <vector>

class Anim {
public:
  Anim(std::vector<std::array<uint8_t, NUM_MATRIX_LEDS>>&& frames, uint8_t accelgyro_momentum_behaviour, uint8_t sfx_index, uint16_t frame_duration_ms);
  void print_info();
private:
  std::vector<std::array<uint8_t, NUM_MATRIX_LEDS>> _frames;
  uint8_t _accelgyro_momentum_behaviour;
  uint8_t _sfx_index;
  uint16_t _frame_duration_ms;
};

/*
  0: Default, up-down-left-right out of screen.
  1: Split from middle, up-down, down y creates pixel copies to extend the frame.
*/

Anim::Anim(std::vector<std::array<uint8_t, NUM_MATRIX_LEDS>>&& frames, uint8_t accelgyro_momentum_behaviour, uint8_t sfx_index, uint16_t frame_duration_ms)
  : _frames(std::move(frames)), _accelgyro_momentum_behaviour(accelgyro_momentum_behaviour), _sfx_index(sfx_index), _frame_duration_ms(frame_duration_ms) {
}

void Anim::print_info() {
  Serial.println("Accelgyro: " + String(_accelgyro_momentum_behaviour));
  Serial.println("SFX index: " + String(_sfx_index));
  Serial.println("Frame duration (ms): " + String(_frame_duration_ms));
}
#pragma endregion

void do_test() {
  
  // TEST ANIM FRAMES
  std::vector<std::array<uint8_t, NUM_MATRIX_LEDS>> frames = {
    // Mouth
    std::array<uint8_t, NUM_MATRIX_LEDS>{
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 1, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 1, 0, 0,
      0, 1, 1, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 1, 1, 0,
      0, 0, 0, 1, 0, 0, 0, 0,
      0, 0, 0, 1, 1, 1, 0, 0,
      0, 0, 0, 0, 1, 0, 0, 0,
      0, 0, 1, 1, 0, 0, 0, 0,
      0, 0, 0, 1, 1, 0, 0, 0,
      0, 0, 0, 0, 1, 1, 0, 0,
      0, 0, 1, 1, 0, 0, 0, 0,
      0, 0, 0, 1, 1, 0, 0, 1,
      1, 0, 0, 1, 1, 0, 0, 0,
      0, 1, 1, 1, 1, 0, 1, 1,
      1, 1, 0, 0, 0, 1, 1, 1,
      1, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 1,
      1, 1, 1, 0, 0, 0, 1, 1,
      1, 1, 0, 1, 1, 1, 1, 0,
      0, 0, 0, 1, 1, 0, 0, 1,
      1, 0, 0, 1, 1, 0, 0, 0,
      0, 0, 0, 0, 1, 1, 0, 0,
      0, 0, 1, 1, 0, 0, 0, 0,
      0, 0, 0, 1, 1, 0, 0, 0,
      0, 0, 0, 0, 1, 1, 0, 0,
      0, 0, 0, 1, 0, 0, 0, 0,
      0, 0, 1, 1, 1, 0, 0, 0,
      0, 0, 0, 0, 1, 0, 0, 0,
      0, 1, 1, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 1, 1, 0,
      0, 0, 1, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 1, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,

      // Center
      // LEFT EYE OPEN
      0, 0, 0, 1, 1, 0, 0, 0,
      0, 0, 1, 1, 1, 1, 0, 0,
      0, 0, 1, 1, 1, 1, 0, 0,
      0, 0, 1, 1, 1, 1, 0, 0,
      0, 0, 1, 1, 1, 1, 0, 0,
      0, 0, 1, 1, 1, 0, 0, 0,
      0, 0, 0, 0, 1, 1, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      // FOREHEAD EMPTY
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      // RIGHT EYE OPEN
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 1, 1, 0, 0, 0, 0,
      0, 0, 0, 1, 1, 1, 0, 0,
      0, 0, 1, 1, 1, 1, 0, 0,
      0, 0, 1, 1, 1, 1, 0, 0,
      0, 0, 1, 1, 1, 1, 0, 0,
      0, 0, 1, 1, 1, 1, 0, 0,
      0, 0, 0, 1, 1, 0, 0, 0,

      // Headband
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 1, 1, 1, 0, 0, 1, 0,
      0, 0, 1, 1, 1, 0, 0, 0,
      0, 0, 0, 1, 0, 1, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 1, 0, 1, 0, 0, 0,
      0, 0, 0, 1, 1, 1, 0, 0,
      0, 1, 0, 0, 1, 1, 1, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0 },
    std::array<uint8_t, NUM_MATRIX_LEDS>{
      // Mouth
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 1, 0, 0,
      0, 0, 0, 1, 0, 0, 0, 0,
      0, 0, 0, 0, 1, 0, 0, 0,
      0, 0, 1, 1, 1, 0, 0, 0,
      0, 0, 0, 0, 1, 1, 0, 1,
      1, 0, 0, 1, 0, 0, 0, 0,
      0, 0, 0, 0, 1, 0, 1, 1,
      1, 1, 0, 1, 1, 1, 0, 0,
      0, 1, 1, 1, 1, 0, 0, 0,
      0, 0, 0, 1, 1, 1, 1, 0,
      0, 0, 1, 1, 1, 0, 1, 1,
      1, 1, 0, 1, 0, 0, 0, 0,
      0, 0, 0, 0, 1, 0, 0, 1,
      1, 0, 1, 1, 0, 0, 0, 0,
      0, 0, 0, 1, 1, 1, 0, 0,
      0, 0, 0, 1, 0, 0, 0, 0,
      0, 0, 0, 0, 1, 0, 0, 0,
      0, 0, 1, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,

      // Center
      // LEFT EYE CLOSED
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 1, 0, 0, 0,
      0, 1, 1, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 1, 0, 0,
      0, 1, 1, 1, 0, 0, 0, 0,
      0, 0, 0, 0, 1, 1, 0, 0,
      0, 0, 0, 1, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      // FOREHEAD EMPTY
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      // RIGHT EYE OPEN
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 1, 1, 0, 0, 0, 0,
      0, 0, 0, 1, 1, 1, 0, 0,
      0, 0, 1, 1, 1, 1, 0, 0,
      0, 0, 1, 1, 1, 1, 0, 0,
      0, 0, 1, 1, 1, 1, 0, 0,
      0, 0, 1, 1, 1, 1, 0, 0,
      0, 0, 0, 1, 1, 0, 0, 0,

      // Headband
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 1, 1, 1, 0, 0, 1, 0,
      0, 0, 1, 1, 1, 0, 0, 0,
      0, 0, 0, 1, 0, 1, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 1, 0, 1, 0, 0, 0,
      0, 0, 0, 1, 1, 1, 0, 0,
      0, 1, 0, 0, 1, 1, 1, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0 },
    std::array<uint8_t, NUM_MATRIX_LEDS>{
      // Mouth
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 1, 0, 0,
      0, 0, 0, 1, 0, 0, 0, 0,
      0, 0, 0, 0, 1, 0, 0, 0,
      0, 0, 1, 1, 1, 0, 0, 0,
      0, 0, 0, 0, 1, 1, 0, 1,
      1, 0, 0, 1, 0, 0, 0, 0,
      0, 0, 0, 0, 1, 0, 1, 1,
      1, 1, 0, 1, 1, 1, 0, 0,
      0, 1, 1, 1, 1, 0, 0, 0,
      0, 0, 0, 1, 1, 1, 1, 0,
      0, 0, 1, 1, 1, 0, 1, 1,
      1, 1, 0, 1, 0, 0, 0, 0,
      0, 0, 0, 0, 1, 0, 0, 1,
      1, 0, 1, 1, 0, 0, 0, 0,
      0, 0, 0, 1, 1, 1, 0, 0,
      0, 0, 0, 1, 0, 0, 0, 0,
      0, 0, 0, 0, 1, 0, 0, 0,
      0, 0, 1, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,

      // LEFT EYE WINK
      0, 1, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 1, 1, 0,
      0, 0, 1, 0, 0, 0, 1, 0,
      0, 1, 0, 0, 0, 1, 1, 0,
      0, 0, 1, 1, 0, 0, 1, 0,
      0, 0, 1, 0, 1, 0, 0, 0,
      0, 0, 0, 0, 1, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      // FOREHEAD EMPTY
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      // RIGHT EYE CLOSED
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 1, 0, 0, 0,
      0, 0, 1, 1, 0, 0, 0, 0,
      0, 0, 0, 0, 1, 1, 1, 0,
      0, 0, 1, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 1, 1, 0,
      0, 0, 0, 1, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,

      // Headband
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 1, 1, 1, 0, 0, 1, 0,
      0, 0, 1, 1, 1, 0, 0, 0,
      0, 0, 0, 1, 0, 1, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 1, 0, 1, 0, 0, 0,
      0, 0, 0, 1, 1, 1, 0, 0,
      0, 1, 0, 0, 1, 1, 1, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0 }
  };
  Anim test(std::move(frames), 0, 1, 120);
  
  int COUNT_FRAMES = 3;
  for (int x = 0; x < COUNT_FRAMES; x++) {
    std::array<uint8_t, NUM_MATRIX_LEDS> pixels = frames[x];
    // Mouth
    for (int i = 0; i < NUM_MOUTH_MATRIX_LEDS; i++) {
      Serial.print(pixels[i]);
    }
    // Left Eye
    for (int i = 0; i < NUM_LEFTEYE_MATRIX_LEDS; i++) {
      Serial.print(pixels[NUM_MOUTH_MATRIX_LEDS + i]);
    }
    // Forehead
    for (int i = 0; i < NUM_FOREHEAD_MATRIX_LEDS; i++) {
      Serial.print(pixels[NUM_LEFTEYE_MATRIX_LEDS + NUM_MOUTH_MATRIX_LEDS + i]);
    }
    // Right Eye
    for (int i = 0; i < NUM_RIGHTEYE_MATRIX_LEDS; i++) {
      Serial.print(pixels[NUM_FOREHEAD_MATRIX_LEDS + NUM_LEFTEYE_MATRIX_LEDS + NUM_MOUTH_MATRIX_LEDS + i]);
    }
    // Headband
    for (int i = 0; i < NUM_HEADBAND_MATRIX_LEDS; i++) {
      Serial.print(pixels[NUM_RIGHTEYE_MATRIX_LEDS + NUM_FOREHEAD_MATRIX_LEDS + NUM_LEFTEYE_MATRIX_LEDS + NUM_MOUTH_MATRIX_LEDS + i]);
    }
  }
}