#include <Arduino.h>
#include <ESP8266WebServer.h>
#include "./gyroscope.h"
#include "./led.h"
#include "./led_strip.h"

#define PIN_WIRE_SDA (4)
#define PIN_WIRE_SCL (5)
#define SDA PIN_WIRE_SDA
#define SCL PIN_WIRE_SCL
#define LED_BUILTIN 16
#define BUILTIN_LED 16
#define D0  16
#define D1  5
#define D2  4
#define D3  0
#define D4  2
#define D5  14
#define D6  12
#define D7  13
#define D8  15
#define D9  3
#define D10 1
#define ESP8266_RX D6
#define ESP8266_TX D5

#define WIFI_SSID "GalaxyS22"
#define WIFI_PASS "azae1234"

unsigned long previousMillis = 0;
int offset_x, offset_y;

ESP8266WebServer server(80);
byte is_wifi_connected = 0;

void setup_wifi(void) {
  uint8_t wifiConnectAttempt = 0;
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  WiFi.hostname("spitfire");
  Serial.println("Connecting to WiFi");
  while (wifiConnectAttempt < 60 && WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
    wifiConnectAttempt = wifiConnectAttempt + 1;
  }
  if (wifiConnectAttempt != 60) {
    Serial.println("Connected to WiFi");
    server.on("/", HTTP_GET, [] () {
      server.send(200, "text/plain", "Hello world!");
    });
    server.on("/red", HTTP_GET, []() {
      set_color_mode(0);
      server.send(200, "text/plain", "Command 'red' received!");
    });
    server.on("/green", HTTP_GET, []() {
      set_color_mode(1);
      server.send(200, "text/plain", "Command 'green' received!");
    });
    server.on("/blue", HTTP_GET, []() {
      set_color_mode(2);
      server.send(200, "text/plain", "Command 'blue' received!");
    });
    is_wifi_connected = 1;
    server.begin();
    return;
  }
  Serial.println("Error: WiFi connection failed, timeout!");
}

void setup(void) {
  EEPROM.begin(512);
  delay(100);
  Serial.begin(9600);
  delay(100);
  setup_wifi();
  setup_gyro();
  setup_leds();
}

void loop(void) {
  unsigned long current_millis = millis();
  if (current_millis - previousMillis >= 60) {
    get_gyro(&offset_y, &offset_x);
    update_leds(offset_x, offset_y);
  }
  if (is_wifi_connected) {
    server.handleClient();
  }
  delay(16);
}