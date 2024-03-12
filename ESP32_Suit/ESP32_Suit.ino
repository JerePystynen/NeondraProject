/*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^
  Author: Neondra Studio
  Project: Project Neondra
  Year: 2024
  Source: https://neondra.com/

  This script is for the main ESP32 that controls Neondra Suit.

  The ESP32's dual-core microprocessor makes it ideal for the Neondra project
  as it can process the webserver's incoming and outgoing network traffic,
  while also managing the 128x128 LCD, audio analyzation via microphone, DFPlayer Mini,
  and control of WS2812B LED Matrices and rings.

Sources:
  Modified code of Ultrasonic Sensor HC-SR04 and Arduino Tutorial by Dejan Nedelkovski:
  https://www.howtomechatronics.com

  LCD TFT Display with ESP32 by Xtronical:
  https://www.xtronical.com/basics/displays/lcd-tft-colourcolor-display-128x128-pixelst7735-driver/128x128-colour-lcd-esp32/

  Making a WiFi Connected Audio Spectrum Analyzer with ESP32 by Element14 Presents:
  https://www.youtube.com/watch?v=1Z1bvs-FCww

^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*/
// [ PIN CONNECTIONS: ]
const uint8_t PIN_HELMET_MATRIX = 0;

const uint8_t PIN_LCD_CS = 5;
const uint8_t PIN_LCD_RST = 4;
const uint8_t PIN_LCD_DC = 2;
// PIN_LCD_SCL = 18

const uint8_t PIN_DFPLAYER_RX = 16;
const uint8_t PIN_DFPLAYER_TX = 17;
const uint8_t PIN_DFPLAYER_ISPLAYING = 34;

const uint8_t PIN_BUTTON_CHANGE_AUDIO_BITRATE = 15;
const uint8_t PIN_AUDIO_INPUT = 36; // (SENSOR_VP)

const uint8_t PIN_WING_MOTOR_DIN = 33;

const uint8_t PIN_ULTRASONIC_TRIGGER = 32;
const uint8_t PIN_ULTRASONIC_ECHO = 35;

const uint8_t PIN_RESISTOR_NETWORK = 19;

// [ UNUSED: ]
// SD_CLK (31)
// D0
// D1
// 21
// RX
// TX
// 22
// CMD (30)
// D2 (28)
// D3 (29)
// 12
// 14
// 27
// 26
// 25
// SENSOR_VN (39)
// EN (Reset ESP32)

#include <EEPROM.h>

void writeDataToEEPROM(int address, int data) {
  EEPROM.begin(512);          // Initialize EEPROM with size
  EEPROM.put(address, data);  // Commit data to EEPROM
  EEPROM.commit();            // Save the data to EEPROM
  EEPROM.end();               // Release EEPROM
}

int readDataFromEEPROM(int address) {
  int data;
  EEPROM.begin(512);          // Initialize EEPROM with size
  EEPROM.get(address, data);  // Get data from address of size
  EEPROM.end();               // Release EEPROM
  return data;
}

TaskHandle_t Core0Task;
TaskHandle_t Core1Task;

// Wi-Fi Credentials
const char* STATION_SSID = "Jentle";
const char* STATION_PASSWORD = "jemmu123";

#include "core0.h"
#include "core1.h"

void setup(void) {

  Serial.begin(9600); // Starts the serial communication

  // Start Core 0:
  xTaskCreatePinnedToCore(
    core0_task,    // Task function
    "core0_task",  // name of task
    10000,         // Stack size of task
    NULL,          // parameter of the task
    1,             // priority of the task
    &Core0Task,    // Task handle to keep track of created task
    0              // pin task to core 0
  );

  // Start Core 1:
  xTaskCreatePinnedToCore(
    core1_task,    // Task function
    "core1_task",  // name of task
    10000,         // Stack size of task
    NULL,          // parameter of the task
    1,             // priority of the task
    &Core1Task,    // Task handle to keep track of created task
    1              // pin task to core 1
  );

  delay(500);
}

void core0_task(void* pvParameters) {
  SETUP_CORE_0();
  Serial.print("Core0Task running on core " + xPortGetCoreID());
  delay(100);
  for (;;) LOOP_CORE_0();
}

void core1_task(void* pvParameters) {
  SETUP_CORE_1();
  Serial.print("Core1Task running on core " + xPortGetCoreID());
  delay(100);
  for (;;) LOOP_CORE_1();
}

void loop(void) {
  delay(1);
}