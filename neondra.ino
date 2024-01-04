/*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^
  Author: Neondra Studio (by "Jentle")
  Project: Project Neondra V1
  Year: 2023
  Website: https://neondra.com/
  Components: ESP8266 + DFPlayer Mini
^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*/
#include <Adafruit_GFX.h>
#include <FastLED.h>
#include <FastLED_NeoMatrix.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <NTPClient.h>

// Reading the SD
#include <SPI.h>
#include <SD.h>
#include "sd_utils.h"
#define PIN_SD 4
// Configuration from the SD
char* ssid;
char* password;
bool use_interface;
uint8_t suitMode;

char BTdata = 0; // Variable for storing received data

// Used by Interface
#include <ESP8266WebServer.h>
#include "interface_html.h"

// Modes (MOUTH, HEADBAND, EYE_LEFT, EYE_RIGHT, FOREHEAD)
#include "mode_default.h"
#include "mode_rave.h"
#include "mode_fury.h"
#include "mode_love.h"
#include "visor_texts.h"

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
#define PART_CHECK_INTERVAL 4000 // if > 0, checks all parts which are connected every X ms

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

// Spitfire
const char* SPITFIRE_IP = "192.168.1.54"; // IP address of the sword so this board can remotely send it messages...

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
// CRGB arm_leds[];
// CRGB leg_leds[];
// CRGB wing_leds[];
// CRGB tail_leds[NUM_TAIL_LEDS];

String headband_text;
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

bool isTailAttached() {
  return false;
}

bool isClawsAttached() {
  return false;
}

bool isLegsAttached() {
  return false;
}

int getBatteryPercentage() {
  int batteryPercentage = 0;
  /*
    currentVoltage = ???;
    maxVoltage = 11.2;
    batteryPercentage = map(currentVoltage, maxVoltage, 0, 100);
  */
  return batteryPercentage;
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

char getFile(const String fileName) {
  File file = SD.open(fileName);
  if (!file) return "";
  Serial.println("Reading from file: " + fileName);
  char output = "";
  while (file.available()) {
    output += file.read();
  }
  file.close();
  return output;
}

void playAudio(const String fileName) {
  getFile(fileName);
}

// Interface
void handleRoot() {
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
  if (isClawsAttached()) {
    
  }
  // Legs
  if (isLegsAttached()) {
    
  }
  // Wing
  if (isWingsAttached()) {
    
  }
  // Tail
  if (isTailAttached()) {
    
  }
}

void initializeInterface() {
  if (!use_interface) return;
  WiFi.begin(ssid, password);
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

void initializeSD() {
  Serial.print("Initializing SD card...");
  if (!SD.begin(PIN_SD)) {
    Serial.println("Error reading SD!");
    return;
  }
  use_interface = ge;
}

void setup(void) {
  Serial.begin(9600);
  Serial.println("Starting...");

  // Initialize to get configs from 'config.txt'
  initializeSD();

  headband_text = USE_SECONDS ? "xx:xx:xx" : "xx:xx";
  initializeLeds();
  initializeInterface();
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

void checkPartsConnectedLoop() {
  if (PART_CHECK_INTERVAL == 0) return;
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis < PART_CHECK_INTERVAL) return;
  previousMillis = currentMillis;
  // CHECK TORSO
  // CHECK LEFT ARM
  // CHECK RIGHT ARM
  // CHECK LEFT LEG
  // CHECK RIGHT LEG
  // CHECK WINGS
  // CHECK TAIL
  // CHECK SPITFIRE
}

void loop(void) {
  server.handleClient();
  headbandDatetimeLoop();
  checkPartsConnectedLoop();
  delay(16.66);
}
