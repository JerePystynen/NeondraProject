/*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^^*^*^*^*^*^*
  Author: Neondra Studio (by "Jentle")
  Project: Project Neondra V1
  Year: 2023
  Website: https://neondra.com/
  Board: Generic ESP8266 Module

  This script is used for the whole suit, if the sections are attached.
  Otherwise uses just the helmet and allows to control it.

  When speaking, the mouth is animated to look like it opens.

  The Neondra Suit's Modes:
  - Default: Everything is in default smug state.
  - Fury: Matrices are red, forehead has an animated yellow warning sign.
  - Love: Matrices are pink, forehead has an animated heart.
  - Sad: Matrices are blue, mouth is a frown.
  - Datetime: The helmet displays date on the headband headband_matrix, and time on the mouth headband_matrix.

  The Neondra Suit's Parts:
  ## The Helmet:
  ### Helmet Rings: Original script is "BT_RGB".
    
  Torso
  Left Arm
  Right Arm
  Left Leg
  Right Leg
  
  ## The Wings:
  ### Wings' Brightness:
  Left Wing and Right Wing have individual brightness controls for .
  ### Wings' Motor Controls:
  Left Wing and Right Wing individual controls UP/DOWN [1-100]. Controlled based on duration between moving difference.
  ## The Tail:
  The tail is held straight by fishlines and can wiggle from left to right and spring lets the tail jolt up and down.
  ### The Tail's Brightness:
  The tail has brightness control for [1-100].

  You can play any .mp3, .acc, . PlayAudio(String file), ie. PlayAudio("test-file-01.mp3");
  You can display text calling DisplayText(String text), ie. DisplayText("Neondra:Project");
  ^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*/
  
#include <SPI.h>
// Used by: Datetime & ControlPanel
#include <Adafruit_GFX.h>
#include <FastLED.h>
#include <FastLED_NeoMatrix.h>
#include <SD.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
// Used by ControlPanel
#include <ESP8266WebServer.h>
#include "control_html_content.h"
// Modes / "Expressions" (MOUTH, HEADBAND, FOREHEAD, EYE_LEFT, EYE_RIGHT)
#include "mode_default.h"
#include "mode_fury.h"
#include "mode_love.h"

// ControlPanel
#define USE_CONTROLPANEL true // If you don't want to initialise a remote control webserver.
const char* SSID = "Jentle";        // CREDENTIALS
const char* PASSWORD = "jemmu123";  // CREDENTIALS

char BTdata = 0; //Variable for storing received data

// NeoPixel Matrix Settings

/// HELMET ///

//// Mouth Matrix
#define PIN_MOUTH_LED 2
#define NUM_MOUTH_LEDS 320

//// Eye Matrices
#define PIN_EYE_LEFT_LED 5
#define PIN_EYE_RIGHT_LED 12
#define NUM_EYE_LEDS 64

//// Forehead Matrix
#define PIN_FOREHEAD_LED 4
#define NUM_FOREHEAD_LEDS 64

//// Headband Matrix
#define PIN_HELMET_HEADBAND 3
#define HEADBAND_WIDTH 8
#define HEADBAND_HEIGHT 40
#define NUM_HEADBAND_LEDS (HEADBAND_WIDTH * HEADBAND_HEIGHT)
#define BRIGHTNESS_HEADBAND 14

//// Ear Ring Strips
#define PIN_EAR_LEFT_LED 13
#define PIN_EAR_RIGHT_LED 14
#define NUM_EAR_LEDS 35
#define SPEED 30

/// TORSO ///

//// Left Arm
//// Right Arm

//// Left Leg
//// Right Leg

//// Left Wing
//// Right Wing

//// Tail

// Datetime Mode
#define USE_SECONDS true
#define SCROLL_INTERVAL 500 // if > 0, scrolls every X ms

const char* ntpServerName = "pool.ntp.org";
WiFiUDP udp; // Create an instance of WiFiUDP to send and receive NTP packets
const long utcOffsetInSeconds = 2 * 60 * 60; // UTC offset in seconds (UTC+2)
NTPClient timeClient(udp, ntpServerName, utcOffsetInSeconds); // Create an instance of the NTPClient to get time
int hours, minutes, seconds;

ESP8266WebServer server(80);

// Helmet
CRGB mouth_leds[NUM_MOUTH_LEDS];
CRGB headband_leds[NUM_HEADBAND_LEDS];
CRGB eyeleft_leds[NUM_EYE_LEDS];
CRGB eyeright_leds[NUM_EYE_LEDS];
CRGB forehead_leds[NUM_FOREHEAD_LEDS];
CRGB earleft_leds[NUM_EAR_LEDS];
CRGB earright_leds[NUM_EAR_LEDS];
// Other
// CRGB torso_leds[];
// CRGB _leds[];
// CRGB _leds[];
// CRGB tail_leds[NUM_TAIL_LEDS];

String headband_text = USE_SECONDS ? "xx:xx:xx" : "xx:xx"; // Placeholder
int headband_x = 0;
int pass = 0;
unsigned long previousMillis = 0;

FastLED_NeoMatrix *headband_matrix = new FastLED_NeoMatrix(headband_leds, HEADBAND_HEIGHT, HEADBAND_WIDTH, HEADBAND_WIDTH / 8, 1,
    NEO_MATRIX_TOP  + NEO_MATRIX_LEFT + NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG);

const uint16_t COLORS_DATETIME[] = {
  headband_matrix->Color(200, 10, 0),   // 00:00 - 01:00 (Night Deep Red)
  headband_matrix->Color(180, 10, 0),   // 01:00 - 02:00 (Night Deep Red)
  headband_matrix->Color(160, 10, 0),   // 02:00 - 03:00 (Night Deep Red)
  headband_matrix->Color(140, 10, 0),   // 03:00 - 04:00 (Night Deep Red)
  headband_matrix->Color(150, 10, 0),   // 04:00 - 05:00 (Dawn Dark Red)
  headband_matrix->Color(180, 30, 0),   // 05:00 - 06:00 (Morning Red)
  headband_matrix->Color(200, 100, 0),  // 06:00 - 07:00 (Orange)
  headband_matrix->Color(200, 140, 10), // 07:00 - 08:00 (Orange)
  headband_matrix->Color(200, 180, 20), // 08:00 - 09:00 (Orange)
  headband_matrix->Color(200, 220, 30), // 09:00 - 10:00 (Orange)
  headband_matrix->Color(0, 0, 255),    // 10:00 - 11:00 (Blue)
  headband_matrix->Color(0, 0, 255),    // 11:00 - 12:00 (Blue)
  headband_matrix->Color(0, 0, 255),    // 12:00 - 13:00 (Blue)
  headband_matrix->Color(0, 0, 255),    // 13:00 - 14:00 (Blue)
  headband_matrix->Color(0, 0, 255),    // 14:00 - 15:00 (Blue)
  headband_matrix->Color(0, 0, 255),    // 15:00 - 16:00 (Blue)
  headband_matrix->Color(0, 0, 255),    // 16:00 - 17:00 (Blue)
  headband_matrix->Color(0, 0, 255),    // 17:00 - 18:00 (Blue)
  headband_matrix->Color(200, 220, 30), // 18:00 - 19:00 (Orange)
  headband_matrix->Color(200, 180, 20), // 19:00 - 20:00 (Orange)
  headband_matrix->Color(200, 140, 10), // 20:00 - 21:00 (Orange)
  headband_matrix->Color(200, 100, 0),  // 21:00 - 22:00 (Orange)
  headband_matrix->Color(250, 60, 0),   // 22:00 - 23:00 (Night Dark Red)
  headband_matrix->Color(230, 10, 0)    // 23:00 - 00:00 (Night Deep Red)
};

const CRGB COLORS_PIXEL[] = {
  CRGB(17, 47, 75),   // DBLUE 0x112F4B
  CRGB(228, 224, 60), // YELLO 0xE4E03C
  CRGB(152, 10, 155), // PINK  0x980A9B
  CRGB(211, 0, 0),    // RED   0x6F2610
  CRGB(10, 9, 4)      // BLACK 0x000000
};

// Neondra suit
// Returns true if the torso is attached and returns a signal.
bool isTorsoAttached() {
  return false;
}

bool isWingsAttached() {
  return false;
}

bool isHandsAttached() {
  return false;
}

bool isLegsAttached() {
  return false;
}

int getBatteryPercentage() {
  return 0;
}

// Clears all Matrixes and sets their color to 0x112F4B
void clearMatrix() {
  FastLED.setBrightness(BRIGHTNESS_HEADBAND);
  fill_solid(mouth_leds, NUM_MOUTH_LEDS, CRGB(0x11, 0x2F, 0x4B));
  fill_solid(forehead_leds, NUM_FOREHEAD_LEDS, CRGB(0x11, 0x2F, 0x4B));
  fill_solid(eyeleft_leds, NUM_EYE_LEDS, CRGB(0x11, 0x2F, 0x4B));
  fill_solid(eyeright_leds, NUM_EYE_LEDS, CRGB(0x11, 0x2F, 0x4B));
  FastLED.show();
}

void setMatrix(CRGB leds[], const uint8_t headband_matrix[], uint8_t numColumns) {
  for (uint8_t col = 0; col < numColumns; col++) {
    for (uint8_t row = 0; row < 8; row++) {
      uint8_t i = pgm_read_byte(&(headband_matrix[col * 8 + row]));
      leds[col * 8 + row] = COLORS_PIXEL[i];
    }
  }
  FastLED.show();
}
void setMatrix64(CRGB leds[64], const uint8_t headband_matrix[64]) {
  setMatrix(leds, headband_matrix, (uint8_t)64);
}
void setMatrix320(CRGB leds[320], const uint8_t headband_matrix[320]) {
  setMatrix(leds, headband_matrix, (uint8_t)320);
}

void setColor(const String colorOverwriteString) {
  // ...
}

void loadFile(const String fileName) {
  SD.open(fileName);
}

void playAudio(const String playAudioParam) {
  // ...
}

// Control Panel
void handleRoot() {
  // If a command has been given
  if (server.arg("set-visor") != "") {
    setColor(server.arg("set-visor"));
  } else if (server.arg("play-audio") != "") {
    playAudio(server.arg("play-audio"));
  }
  server.send(200, "text/html", HTML_CONTENT);
}

void handleAjax() {
  String data = server.arg("plain");
  Serial.println("Received data via Ajax POST: " + data);

  // Process the received data as needed

  server.send(200, "text/plain", "Data received successfully!");
}

void initializeLeds() {
  // Helmet
  FastLED.addLeds<WS2812, PIN_MOUTH_LED, GRB>(mouth_leds, NUM_MOUTH_LEDS);
  FastLED.addLeds<WS2812, PIN_FOREHEAD_LED, GRB>(forehead_leds, NUM_FOREHEAD_LEDS);
  FastLED.addLeds<WS2812, PIN_EYE_LEFT_LED, GRB>(eyeleft_leds, NUM_EYE_LEDS);
  FastLED.addLeds<WS2812, PIN_EYE_RIGHT_LED, GRB>(eyeright_leds, NUM_EYE_LEDS);
  FastLED.addLeds<NEOPIXEL, PIN_HELMET_HEADBAND>(headband_leds, NUM_HEADBAND_LEDS);
  headband_matrix->begin();
  headband_matrix->setTextWrap(false);
  headband_matrix->setBrightness(BRIGHTNESS_HEADBAND);
  headband_matrix->setTextColor(COLORS_DATETIME[0]);
  FastLED.addLeds<NEOPIXEL, PIN_EAR_LEFT_LED>(earleft_leds, NUM_EAR_LEDS);
  FastLED.addLeds<NEOPIXEL, PIN_EAR_RIGHT_LED>(earright_leds, NUM_EAR_LEDS);
  // Torso
  if (isTorsoAttached()) {
    
  }
  // Hands
  if (isHandsAttached()) {
    
  }
  // Legs
  if (isLegsAttached()) {
    
  }
  // Wing
  if (isWingAttached()) {
    
  }
  // Tail
  if (isTailAttached()) {
    
  }
}

void setup(void) {
  Serial.begin(9600);

  initializeLeds();

  WiFi.begin(SSID, PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi");

  // Initialize the web server
  server.on("/", HTTP_GET, handleRoot);
  server.on("/ajax", HTTP_POST, handleAjax);
  server.begin();
  Serial.println("HTTP server started");

  // Initialize and synchronize time
  timeClient.begin();
  timeClient.update();
}

// Get hours, minutes, and seconds in a clean format, ready to be displayed.
String getUpdatedTimeClientTime() {
  // If there is an internet connection, use that to look for the data.
  if (WiFi.status() == WL_CONNECTED) {
    timeClient.update();
    hours = timeClient.getHours();
    minutes = timeClient.getMinutes();
    seconds = timeClient.getSeconds();
  } else {
    seconds++;
    minutes += (seconds > 59);
    hours += (minutes > 59);
    seconds %= 60;
    minutes %= 60;
    hours %= 24;
  }
  String hoursStr = (hours < 10 ? "0" : "") + String(hours);
  String minutesStr = (minutes < 10 ? "0" : "") + String(minutes);
  String secondsStr = (seconds < 10 ? "0" : "") + String(seconds);
  return (USE_SECONDS) ? (hoursStr + ":" + minutesStr + ":" + secondsStr) : (hoursStr + ":" + minutesStr);
}

// Display the time on the NeoPixel headband_matrix
void headbandDatetimeLoop() {
  headband_matrix->fillScreen(0);
  headband_matrix->setCursor(headband_x, 0);
  headband_text = getUpdatedTimeClientTime();
  headband_matrix->print(headband_text.c_str());
  unsigned long currentMillis = millis();
  // Scroll text from right to left
  if (SCROLL_INTERVAL > 0) {
    if (currentMillis - previousMillis >= SCROLL_INTERVAL) {
      previousMillis = currentMillis;
      if (--headband_x < -(int)headband_text.length() * 6) {
        headband_x = headband_matrix->width();
        headband_matrix->setTextColor(COLORS_DATETIME[hours]); // update every hour
      }
    }
  }
  headband_matrix->show();
}

void loop(void) {
  server.handleClient();
  headbandDatetimeLoop();
  delay(16.66);
}
