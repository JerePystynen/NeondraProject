/*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^
  Author: Neondra Studio
  Project: Project Neondra
  Year: 2023
  Source: https://neondra.com/
  
  This script is for ESP8266 board for displaying the date on the headband matrix,
  and the time on the mouth matrix of the helmet.
^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*/
#include <Adafruit_GFX.h>
#include <FastLED.h>
#include <FastLED_NeoMatrix.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <NTPClient.h>

// NeoPixel settings
#define PIN 4
#define BRIGHTNESS 14
#define MW 8
#define MH 40
#define NUMMATRIX (MW*MH)

#define USE_SECONDS true
#define SCROLL_INTERVAL 500 // if > 0, scrolls every X ms

const char* ssid = "Jentle";
const char* password = "jemmu123";

const char* ntpServerName = "pool.ntp.org";
WiFiUDP udp; // Create an instance of WiFiUDP to send and receive NTP packets
const long utcOffsetInSeconds = 2 * 60 * 60; // UTC offset in seconds (UTC+2)

// Create an instance of the NTPClient to get time
NTPClient timeClient(udp, ntpServerName, utcOffsetInSeconds);

CRGB leds[NUMMATRIX];
String matrixDisplayText = USE_SECONDS ? "xx:xx:xx" : "xx:xx"; // Placeholder
int x = 0;
int pass = 0;
unsigned long previousMillis = 0;

FastLED_NeoMatrix *matrix = new FastLED_NeoMatrix(leds, MH, MW, MW / 8, 1,
  NEO_MATRIX_TOP  + NEO_MATRIX_LEFT + NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG);

const uint16_t colors[] = {
  matrix->Color(200, 10, 0),   // 00:00 - 01:00 (Night Deep Red)
  matrix->Color(180, 10, 0),   // 01:00 - 02:00 (Night Deep Red)
  matrix->Color(160, 10, 0),   // 02:00 - 03:00 (Night Deep Red)
  matrix->Color(140, 10, 0),   // 03:00 - 04:00 (Night Deep Red)
  matrix->Color(150, 10, 0),   // 04:00 - 05:00 (Dawn Dark Red)
  matrix->Color(180, 30, 0),   // 05:00 - 06:00 (Morning Red)
  matrix->Color(200, 100, 0),  // 06:00 - 07:00 (Orange)
  matrix->Color(200, 140, 10), // 07:00 - 08:00 (Orange)
  matrix->Color(200, 180, 20), // 08:00 - 09:00 (Orange)
  matrix->Color(200, 220, 30), // 09:00 - 10:00 (Orange)
  matrix->Color(0, 0, 255),    // 10:00 - 11:00 (Blue)
  matrix->Color(0, 0, 255),    // 11:00 - 12:00 (Blue)
  matrix->Color(0, 0, 255),    // 12:00 - 13:00 (Blue)
  matrix->Color(0, 0, 255),    // 13:00 - 14:00 (Blue)
  matrix->Color(0, 0, 255),    // 14:00 - 15:00 (Blue)
  matrix->Color(0, 0, 255),    // 15:00 - 16:00 (Blue)
  matrix->Color(0, 0, 255),    // 16:00 - 17:00 (Blue)
  matrix->Color(0, 0, 255),    // 17:00 - 18:00 (Blue)
  matrix->Color(200, 220, 30), // 18:00 - 19:00 (Orange)
  matrix->Color(200, 180, 20), // 19:00 - 20:00 (Orange)
  matrix->Color(200, 140, 10), // 20:00 - 21:00 (Orange)
  matrix->Color(200, 100, 0),  // 21:00 - 22:00 (Orange)
  matrix->Color(250, 60, 0),   // 22:00 - 23:00 (Night Dark Red)
  matrix->Color(230, 10, 0)    // 23:00 - 00:00 (Night Deep Red)
};

void setup() {
  FastLED.addLeds<NEOPIXEL, PIN>(leds, NUMMATRIX);
  matrix->begin();
  matrix->setTextWrap(false);
  matrix->setBrightness(BRIGHTNESS);
  matrix->setTextColor(colors[0]);

  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(250);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("Connected to WiFi");
  // Initialize and synchronize time
  timeClient.begin();
  timeClient.update();
}

void loop() {
  timeClient.update();
  // Get hours, minutes, and seconds
  int hours = timeClient.getHours();
  int minutes = timeClient.getMinutes();
  int seconds = timeClient.getSeconds();
  // Display the time on the NeoPixel matrix
  //displayTime(hours, minutes, seconds);
  matrix->fillScreen(0);
  matrix->setCursor(x, 0);
  
  String hoursStr = (hours < 10 ? "0" : "") + String(hours);
  String minutesStr = (minutes < 10 ? "0" : "") + String(minutes);
  String secondsStr = (seconds < 10 ? "0" : "") + String(seconds);
  matrixDisplayText = (USE_SECONDS) ? (hoursStr + ":" + minutesStr + ":" + secondsStr) : (hoursStr + ":" + minutesStr);
  matrix->print(matrixDisplayText.c_str());

  unsigned long currentMillis = millis();
  
  // Scroll text from right to left
  if (SCROLL_INTERVAL > 0) {
    if (currentMillis - previousMillis >= SCROLL_INTERVAL) {
      previousMillis = currentMillis;
      if (--x < -(int)matrixDisplayText.length() * 6) {
        x = matrix->width();
        matrix->setTextColor(colors[hours]); // update every hour
      }
    }
  }
  matrix->show();
}
