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

^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*/

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

  Serial.begin(9600);

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

  // Setup for core0.h
  SETUP_CORE_0();

  Serial.print("Core0Task running on core ");
  Serial.println(xPortGetCoreID());

  for (;;) {
    LOOP_CORE_0();
    delay(1000);
  }
}

void core1_task(void* pvParameters) {

  // Setup for core1.h
  SETUP_CORE_1();

  Serial.print("Core1Task running on core ");
  Serial.println(xPortGetCoreID());

  for (;;) {
    LOOP_CORE_1();
    delay(500);
  }
}

void loop(void) {
}