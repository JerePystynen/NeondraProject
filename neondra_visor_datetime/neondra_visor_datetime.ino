#include <Adafruit_GFX.h>
#include <FastLED.h>
#include <FastLED_NeoMatrix.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <NTPClient.h>

const char* ssid = "Jentle";
const char* password = "jemmu123";

const char* ntpServerName = "pool.ntp.org";
WiFiUDP udp; // Create an instance of WiFiUDP to send and receive NTP packets
const long utcOffsetInSeconds = 2 * 60 * 60; // UTC offset in seconds (UTC+2)

// Create an instance of the NTPClient to get time
NTPClient timeClient(udp, ntpServerName, utcOffsetInSeconds);

// NeoPixel settings
#define PIN 4
#define BRIGHTNESS 20
#define MW 8
#define MH 40
#define NUMMATRIX (MW*MH)

CRGB leds[NUMMATRIX];
String DISPLAY_TEXT = "xx:xx:xx"; // Placeholder

FastLED_NeoMatrix *matrix = new FastLED_NeoMatrix(leds, MH, MW, MW / 8, 1,
  NEO_MATRIX_TOP  + NEO_MATRIX_LEFT + NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG);

const uint16_t colors[] = {
  matrix->Color(255, 0, 0), matrix->Color(0, 255, 0), matrix->Color(0, 0, 255)
};

int x = (MH - DISPLAY_TEXT.length() * 6) / 2; // Centered
int pass = 0;

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
  DISPLAY_TEXT = hoursStr + ":" + minutesStr + ":" + secondsStr;
  matrix->print(DISPLAY_TEXT.c_str());

  /*if (--x < -(int)DISPLAY_TEXT.length() * 6) {
    x = matrix->width();
    if (++pass >= 3) pass = 0;
    matrix->setTextColor(colors[pass]);
  }*/
  
  matrix->show();
}
