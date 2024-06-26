/*
void set_matrix_with_offset2(const uint8_t framedata[], uint8_t matrix_width, uint8_t matrix_height, int offset_x, int offset_y, int offset_index) {
  for (uint8_t col = 0; col < matrix_width; col++) {
    for (uint8_t row = 0; row < 8; row++) {
      int offset = (y + (offset_y * 0.1)) * matrix_width + (x + (offset_x * 0.5));
      int ledIndex = (y % 2 == 0 ? y * matrix_width + x : (y + 1) * matrix_width - x - 1) + offset_index;

      uint8_t i = pgm_read_byte(&(framedata[col * 8 + row + offset_index]));
      CRGB color = (offset >= 0 && offset < matrix_width * matrix_height) ? COLORS_PIXEL[i] : COLORS_PIXEL[0];
      leds_matrix[col * 8 + row + offset_index] = color;
    }
  }
}

void set_matrix_with_offset3(const uint8_t framedata[], int matrix_width, int matrix_height, int offset_x, int offset_y, int offset_index) {
  for (int y = 0; y < matrix_height; y++) {
    for (int x = 0; x < matrix_width; x++) {
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

void test(void) {
  if (isAnimatingMouth) {
    fill_solid(leds_mouth, NUM_MOUTH_LEDS, COLORS_PIXEL[0]);
    mouth_matrix->setCursor(mouth_x, 0);
    mouth_matrix->print(mouth_text.c_str());
    uint8_t hours = 23;  // PARSE_FIRST_2_CHARACTERS_OF_TIME; // "232202" => 23
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
    uint8_t hours = 23;  // PARSE_FIRST_2_CHARACTERS_OF_TIME; // "232202" => 23
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

  if (millis() - previous_ms >= 1000) {
    // If the mode has scrolling text, Offline is the only mode one which doesn't scroll
    if (selectedMode > 0) {
      // Mouth
      if (!isAnimatingMouth) {
        if (mouth_scrolling_text_timer == 0) {
          mouth_scrolling_text_timer = random(55, 255);
          isAnimatingMouth = true;
        }
        --mouth_scrolling_text_timer;
      }
      // Headband
      if (!isAnimatingHeadband) {
        if (headband_scrolling_text_timer == 0) {
          headband_scrolling_text_timer = random(55, 255);
          isAnimatingHeadband = true;
        }
        --headband_scrolling_text_timer;
      }
      //Serial.println(String(mouth_scrolling_text_timer) + " | " + String(headband_scrolling_text_timer) + " | " + String(isAnimatingMouth) + " | " + String(isAnimatingHeadband));
    }
    previous_ms = millis();
    if (++animationFrame > 3) animationFrame = 0;

    //ANIM_DEFAULT_SMUGWINK
    if (!isAnimatingMouth) {
      if (animationFrame < mouthAnimationFrameCount) set_matrix(leds_mouth, mouthAnimationFrames[animationFrame], 48);
    }
    if (animationFrame < mouthAnimationFrameCount) {
      set_matrix(leds_eyes, eyesAnimationFrames[animationFrame], 24);
    }
    if (!isAnimatingHeadband) {
      if (animationFrame < headbandAnimationFrameCount) set_matrix(leds_headband, headbandAnimationFrames[animationFrame], 40);
    }
    // if (animationFrame == 0) {
    //   if (!isAnimatingMouth) {
    //     set_matrix(leds_mouth, neondra_default_mouth_1, 48);
    //   }
    //   set_matrix(leds_eyes, neondra_default_eyes_1, 24);
    //   if (!isAnimatingHeadband) {
    //     set_matrix(leds_headband, neondra_default_headband_1, 40);
    //   }
    // } else if (animationFrame == 1) {
    //   if (!isAnimatingMouth) set_matrix(leds_mouth, neondra_default_mouth_2, 48);
    //   set_matrix(leds_eyes, neondra_default_eyes_2, 24);
    // } else if (animationFrame == 2) {
    //   set_matrix(leds_eyes, rave_central_0, 24);
    //   //set_matrix(leds_eyes, neondra_default_eyes_3, 24);
    // }
  }

  // Update the Ear LEDs
  for (uint8_t i = 0; i < NUM_EARS_LEDS; i++) {
    uint8_t palette_index = map(i, 0, NUM_EARS_LEDS - 1, 0, 240);
    leds_ears[i] = ColorFromPalette(myPal, palette_index, brightness_ears, LINEARBLEND);
  }
}
*/