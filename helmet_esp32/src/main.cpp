/*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^
  @author:  Jentle
  @project: Project Neondra
  @year:    2025
  @source:  https://jentle.com/

  This script is for the main ESP32 that controls Neondra Suit.

  The ESP32's dual-core microprocessor makes it ideal for the Neondra project
  as it can process the webserver's incoming and outgoing network traffic,
  while also managing the 128x128 LCD, audio analyzation via microphone, DFPlayer Mini,
  and control of WS2812B LED Matrices and rings.

  !IN CASE THE UPLOAD DOES NOT WORK:!

  SWITCH 'FLASH MODE' to 'DIO' !

Sources:
  How to use ESP32 Dual Core with Arduino IDE, Random Nerd Tutorials:
  https://randomnerdtutorials.com/esp32-dual-core-arduino-ide/

  Modified code of Ultrasonic Sensor HC-SR04 and Arduino Tutorial by Dejan Nedelkovski:
  https://www.howtomechatronics.com

  LCD TFT Display with ESP32 by Xtronical:
  https://www.xtronical.com/basics/displays/lcd-tft-colourcolor-display-128x128-pixelst7735-driver/128x128-colour-lcd-esp32/

  Making a WiFi Connected Audio Spectrum Analyzer with ESP32 by Element14 Presents:
  https://www.youtube.com/watch?v=1Z1bvs-FCww

  4-Wire Fan Speed Control Hardware example, Argus Monitor:
  https://help.argusmonitor.com/HardwareexampleArduino.html

^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*/
#include <Arduino.h>

// [ PIN CONNECTIONS: ] =================== //
const uint8_t PIN_LCD_CS = 5;               // GPIO05 => LCD CS
const uint8_t PIN_LCD_RST = 4;              // GPIO04 => LCD RESET
const uint8_t PIN_LCD_DC = 2;               // GPIO02 => LCD A0
const uint8_t PIN_LCD_SCL = 18;             // GPIO18 => LCD SCK

const uint8_t PIN_DFPLAYER_M_RX = 17;       // GPIO17 => DFPLAYER M RX
const uint8_t PIN_DFPLAYER_M_TX = 16;       // GPIO16 => DFPLAYER M TX
const uint8_t PIN_DFPLAYER_SFX_RX = 15;     // GPIO15 => DFPLAYER SFX RX
const uint8_t PIN_DFPLAYER_SFX_TX = 14;     // GPIO14 => DFPLAYER SFX TX
const uint8_t PIN_DFPLAYER_ISPLAYING = 34;  // GPIO34 => DFPLAYER IsPlaying

const uint8_t PIN_HELMET_MATRIX = 33;       // GPIO33 => HELMET Matrix
const uint8_t PIN_LEDS_OTHER = 19;          // GPIO19 => OTHER Matrices

const uint8_t PIN_HELMET_FAN_PWM = 27;      // GPIO27 => HELMET FAN PWM
const uint8_t PIN_HELMET_FAN_SENSE = 13;    //  MISO  => HELMET FAN SENSE
const uint8_t PIN_HELMET_FAN_ENABLE = 23;   // GPIO23 => HELMET FAN ENABLE

const uint8_t PIN_ULTRASONIC_TRIGGER = 32;  // GPIO32 => ULTRASONIC TRIGGER
const uint8_t PIN_ULTRASONIC_ECHO = 35;     // GPIO35 => ULTRASONIC ECHO
const uint8_t PIN_MUSIC_SRC_SWITCH = 36;    //  (VP)  => MUSIC SOURCE SWITCH
const uint8_t PIN_RESISTOR_NETWORK = 23;    // GPIO23 => RESISTORS
const uint8_t PIN_WING_MOTOR_DIN = 39;      //  (VN)  => WING MOTOR

// Wi-Fi Credentials
const char* STATION_SSID = "JP";
const char* STATION_PASSWORD = "qwerty123";

// Access Point Credentials
const char* AP_SSID = "Neondra_AP";
const char* AP_PASSWORD = "neondra123";

void handle_serial_input(void);

#include "esp_task_wdt.h"
#include "core0.cpp"
#include "core1.cpp"
TaskHandle_t c0_f, c1_f;

void handle_serial_input(void) {
  if (Serial.available() == 0) return;
  byte serial_bytes = Serial.read();
  String inData = Serial.readStringUntil('\n');
  int pwm = Serial.parseInt();
  if (pwm >= 0 && pwm <= 255) {
    //setPWM(pwm);
    Serial.print("PWM set to ");
    Serial.println(pwm);
  }
  // MANUAL CONTROL
  // if (serial_bytes == '1') play_anim(0);  // Anim: SmugWink
  // if (serial_bytes == '2') play_anim(1);  // Anim:
  // if (serial_bytes == '3') play_anim(2);  // Anim:
  // if (serial_bytes == '4') play_anim(3);  // Anim:
  // if (serial_bytes == '5') play_anim(4);  // Anim:
  // if (serial_bytes == 'q') set_suit_mode(0);  // Mode: Offline
  // if (serial_bytes == 'w') set_suit_mode(1);  // Mode: Normal
  // if (serial_bytes == 'e') set_suit_mode(2);  // Mode: Rave
  // if (serial_bytes == 'r') set_suit_mode(3);  // Mode: Cute
  // if (serial_bytes == 't') set_suit_mode(4);  // Mode: Fury
  // if (serial_bytes == 'a') set_ear_leds(RGB_TO_COLOR(255, 0, 0));
  // if (serial_bytes == 's') set_ear_leds(RGB_TO_COLOR(0, 255, 0));
  // if (serial_bytes == 'd') set_ear_leds(RGB_TO_COLOR(0, 0, 255));
  // if (serial_bytes == 'f') set_ear_leds(RGB_TO_COLOR(255, 255, 0));
  // if (serial_bytes == 'g') set_ear_leds(RGB_TO_COLOR(0, 255, 255));
  // if (serial_bytes == 'h') set_ear_leds(RGB_TO_COLOR(255, 0, 255));
  // if (serial_bytes == 'j') set_ear_leds(RGB_TO_COLOR(255, 100, 0));
  // if (serial_bytes == 'k') set_ear_leds(RGB_TO_COLOR(100, 0, 255));
}

void c0_task(void* pv_parameters) {
  for (;;) {
    loop_c0();
    esp_task_wdt_reset();
    delay(50);
  }
}

void c1_task(void* pv_parameters) {
  for (;;) {
    loop_c1();
    esp_task_wdt_reset();
    delay(50);
  }
}

void setup(void) {
  Serial.begin(115200);  // Starts the serial communication
  esp_task_wdt_init(10, true);
  //// Start Core 0: ////
  setup_c0();
  xTaskCreatePinnedToCore(
    c0_task,                        // Task function
    "Core0 - DFPlayers, TFT, Fan",  // name of task
    60000,                          // Stack size of task
    NULL,                           // parameter of the task
    2,                              // priority of the task
    &c0_f,                          // Task handle to keep track of created task
    0                               // pin task to core 0
  );
  esp_task_wdt_add(c0_f);
  //// Start Core 1: ////
  setup_c1();
  xTaskCreatePinnedToCore(
    c1_task,                            // Task function
    "Core1 - Webserver, Serial Input",  // name of task
    30000,                              // Stack size of task
    NULL,                               // parameter of the task
    1,                                  // priority of the task
    &c1_f,                              // Task handle to keep track of created task
    1                                   // pin task to core 1
  );
  esp_task_wdt_add(c1_f);
}

void loop(void) {
  delayMicroseconds(1);
}