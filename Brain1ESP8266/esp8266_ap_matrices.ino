/**********************************
Author: Neondra Studio
Project: Project Neondra
Year: 2024
Source: https://neondra.com/

Communication baud rates:
 - DFPlayer Mini 9600
 - Arduino Nano 19200
 - Serial 115200

**********************************/
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"
#include <ESP8266mDNS.h>
#include <FastLED.h>

/// WEBSERVER

const char* stationSSID = "Jentle";
const char* stationPassword = "jemmu123";

const char* apSSID = "Neondra_AP";
const char* apPassword = "neondra";

IPAddress IPaddr (127, 1, 1, 40);
IPAddress IPmask (255, 255, 255, 0);

ESP8266WebServer server(80); // Create an instance of the server

/// END WEBSERVER

/// AUDIO PLAYER

const int DFplayerIN = 12;      // 'D6'
const int DFPlayerOUT = 14;     // 'D5'
const int DFPlayerBUSYPin = 15; // 'D8'

SoftwareSerial mySoftwareSerial(DFPlayerOUT, DFplayerIN); // RX, TX
DFRobotDFPlayerMini myDFPlayer;

uint8_t currentTrack = 1;
bool shouldPlay = false;
int currentVolume = 8; // Set your default volume level
const int VOLUME_MAX = 30;
String currentStatus = "";
const int VOLUME_THRESHOLD = 50; // Adjust as needed

const int numReadings = 10;
int readings[numReadings];
int indexX = 0;

/// END AUDIO PLAYER

/// LED MATRICES
/*
// MATRIX DATA OUTPUTS
*/
const int PIN_MOUTH_LED = 4; // 'D2'
/*
//#define PIN_HEADBAND_LED 3
//#define PIN_FOREHEAD_LED 4
//#define PIN_EYE_LEFT_LED 5
//#define PIN_EYE_RIGHT_LED 6

//#define NUM_MATRICES 5
*/
#define NUM_MOUTH_LEDS 320 //384
/*
//#define NUM_HEADBAND_LEDS 320
//#define NUM_FOREHEAD_LEDS 64
//#define NUM_EYE_LEDS 64
*/
// LED SETTINGS
const uint8_t BRIGHTNESS = 20;
/*
// EXPRESSIONS (MOUTH, HEADBAND, FOREHEAD, EYE_LEFT, EYE_RIGHT)
#include "Matrix_Default.h"

const CRGB customColors[] = {
  CRGB(17, 47, 75),   // DBLUE 0x112F4B
  CRGB(228, 224, 60), // YELLO 0xE4E03C
  CRGB(152, 10, 155), // PINK  0x980A9B
  CRGB(211, 0, 0),    // RED   0x6F2610
  CRGB(10, 9, 4)      // BLACK 0x000000
};
*/
CRGB mouth_leds[NUM_MOUTH_LEDS];
/*
//CRGB forehead_leds[NUM_FOREHEAD_LEDS];
//CRGB eyeleft_leds[NUM_EYE_LEDS];
//CRGB eyeright_leds[NUM_EYE_LEDS];

uint8_t currentAnimationFrameIndex = 0;
unsigned long previousMillis = 0; // will store last time AnimationFrame was updated
const long interval = 1000;
*/
/// END LED MATRICES

void setup(void) {

  FastLED.addLeds<WS2812, PIN_MOUTH_LED, GRB>(mouth_leds, NUM_MOUTH_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
  //setMatrix320(mouth_leds, neondra_default_mouth_1);
  
  pinMode(DFPlayerBUSYPin, INPUT);
  
  mySoftwareSerial.begin(9600);
  while (!mySoftwareSerial) { ; }
  Serial.begin(9600);
  while (!Serial) { ; }

  Serial.println(F("Initializing DFPlayer Mini... (May take 3~5 seconds)"));
  if (!myDFPlayer.begin(mySoftwareSerial, true, true)) {
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while (true) {
      delay(0); // Code to compatible with ESP8266 watch dog.
    }
  }
  Serial.println(F("DFPlayer Mini online."));

  myDFPlayer.volume(currentVolume);  //Set volume value. From 0 to 30
  Serial.println(F("volume = 8"));
  myDFPlayer.play(currentTrack);

  // Connect to the existing Wi-Fi network as a station
  WiFi.hostname("Neondra Suit");
  WiFi.begin(stationSSID, stationPassword);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to Wi-Fi");
  
  // Set up the access point
  Serial.println("Initializing Wi-Fi access point...");
  WiFi.softAP(apSSID, apPassword);
  WiFi.softAPConfig(IPaddr, IPaddr, IPmask);
  IPAddress myIP = WiFi.softAPIP();
  Serial.println("AP IP address: ");
  Serial.println(WiFi.softAPIP());

  // Enable mDNS to create a local domain called "neondra.local"
  if (!MDNS.begin("neondra")) {
    Serial.println("Error setting up MDNS responder!");
    while (1) {
      delay(1000);
    }
  }
  Serial.println("mDNS responder started");

  // Define web server routes
  server.on("/", HTTP_GET, handleRoot);
  server.on("/play", HTTP_GET, handlePlay);
  server.on("/stop", HTTP_GET, handleStop);
  server.on("/volumeup", HTTP_GET, handleVolumeUp);
  server.on("/volumedown", HTTP_GET, handleVolumeDown);

  // Start the server
  server.begin();
  MDNS.addService("http", "tcp", 80);

  // Initialize the array with 0 values
  for (int i = 0; i < numReadings; i++) {
    readings[i] = 0;
  }
  
  delay(2000); // Wait for the BUSY state to change
}

/*
// Clears all Matrixes and sets their color to 0x112F4B
void clearMatrix(void) {
  FastLED.setBrightness(BRIGHTNESS);
  fill_solid(mouth_leds, NUM_MOUTH_LEDS, CRGB(0x11, 0x2F, 0x4B));
  //fill_solid(forehead_leds, NUM_FOREHEAD_LEDS, CRGB(0x11, 0x2F, 0x4B));
  //fill_solid(eyeleft_leds, NUM_EYE_LEDS, CRGB(0x11, 0x2F, 0x4B));
  //fill_solid(eyeright_leds, NUM_EYE_LEDS, CRGB(0x11, 0x2F, 0x4B));
  FastLED.show();
}

void setMatrix64(CRGB leds[64], const uint8_t matrix[64]) {
  for (uint8_t col = 0; col < 8; col++) {
    for (uint8_t row = 0; row < 8; row++) {
      uint8_t i = pgm_read_byte(&(matrix[col * 8 + row]));
      leds[col * 8 + row] = customColors[i];
    }
  }
  FastLED.show();
}

void setMatrix320(CRGB leds[320], const uint8_t matrix[320]) {
  for (uint8_t col = 0; col < 40; col++) {
    for (uint8_t row = 0; row < 8; row++) {
      uint8_t i = pgm_read_byte(&(matrix[col * 8 + row]));
      leds[col * 8 + row] = customColors[i];
    }
  }
  FastLED.show();
}

void animateMatrices(void) {
  currentAnimationFrameIndex++;
  if (currentAnimationFrameIndex > 2) currentAnimationFrameIndex = 0;
  Serial.println("animating matrices: " + String(currentAnimationFrameIndex));
  if (currentAnimationFrameIndex == 0) {
    setMatrix320(mouth_leds, neondra_default_mouth_1);
    //setMatrix64(eyeleft_leds, neondra_default_eyeleft_wink);
    //setMatrix64(eyeright_leds, neondra_default_eyeright_closed);
    //setMatrix64(forehead_leds, neondra_love_forehead_1);
    currentAnimationFrameIndex = 1;
    return;
  }
  if (currentAnimationFrameIndex == 2) {
    setMatrix320(mouth_leds, neondra_default_mouth_2);
    //setMatrix64(eyeleft_leds, neondra_default_eyeleft_open);
    //setMatrix64(eyeright_leds, neondra_default_eyeright_open);
    //setMatrix64(forehead_leds, neondra_default_forehead_1);
    return;
  }
}
*/

int getA0Reading(void) {
  if (!shouldPlay) return 0;
  
  int analogValue = analogRead(A0);

  // Apply volume factor to the current reading with a threshold, Inverse relationship
  float volumeFactor = (currentVolume > VOLUME_THRESHOLD) ? (1.0 - float(currentVolume - VOLUME_THRESHOLD) / (VOLUME_MAX - VOLUME_THRESHOLD)) : 1.0;

  // 0-200
  
  int weightedReading = int(float(analogValue) * volumeFactor);

  readings[indexX] = weightedReading;
  indexX = (indexX + 1) % numReadings; // circular buffer

  int total = 0;
  for (int i = 0; i < numReadings; i++) {
    total += readings[i];
  }
  // Calculate the moving average
  int average = total / numReadings;
  return average;
}

void loop(void) {
  // Handle ongoing mDNS operations
  MDNS.update();
  server.handleClient();
  
  // Read the state of the BUSY pin
  int busyState = digitalRead(DFPlayerBUSYPin);
  if (shouldPlay && busyState == HIGH) {
    Serial.println("DFPlayer Mini has finished playing.");
    currentTrack++;
    if (currentTrack > 20) currentTrack = 0;
    myDFPlayer.play(currentTrack);
    char s[24] = "DFPlayer: Playing track";
    //char e[15] = "EMPTY_SONG_NAME";
    sprintf(s, "%s %d", s, currentTrack);
    Serial.println(s);
  }
  
  /*
  // if the difference between the current time and last time
  // is bigger than the interval at which you want to update.
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    // save the last time you updated
    previousMillis = currentMillis;
    animateMatrices();
  }
  */

  // TEST CODE
  int sensorValue = getA0Reading();  // Assuming you have a sensor connected to A0 for range 1-10
  Serial.println("DAC_Right Value: " + String(sensorValue));
  // Map the analog range (0-1023) to LED range (0-63)
  int ledIndex = map(sensorValue, 0, 200, 0, 320 - 1);
  // Clear all LEDs
  fill_solid(mouth_leds, NUM_MOUTH_LEDS, CRGB(0x11, 0x2F, 0x4B));
  // Light up the selected LED
  mouth_leds[ledIndex] = CRGB(228, 224, 60);
  FastLED.show();
  /*
  Index = 0;
  if () {
    // Transmit data over to Arduino NANO for Matrix controlling
    Serial.println("DAC_Right Value: " + String(getA0Reading()));
    Index = 0;
  }
  */
  delay(100);
}

void handleRoot(void) {
  String html = "<html><body><h1>DFPlayer Mini Control</h1>"
                "<p><a href='/play'>Play</a></p>"
                "<p><a href='/stop'>Stop</a></p>"
                "<p><a href='/volumeup'>Volume Up</a></p>"
                "<p><a href='/volumedown'>Volume Down</a></p>"
                "<p>Current Status: " + currentStatus + "</p>"
                "<p>Current Volume: " + String(currentVolume) + "</p></body></html>";
  server.send(200, "text/html", html);
}

void handlePlay(void) {
  myDFPlayer.start();
  shouldPlay = true;
  currentStatus = "Playing";
  handleRoot();
}

void handleStop(void) {
  myDFPlayer.stop();
  shouldPlay = false;
  currentStatus = "Stopped";
  handleRoot();
}

void handleVolumeUp(void) {
  if (currentVolume < VOLUME_MAX) {
    currentVolume += 5;
    if (currentVolume > VOLUME_MAX) currentVolume = VOLUME_MAX;
    myDFPlayer.volume(currentVolume);
  }
  currentStatus = "VolumeUp";
  handleRoot();
}

void handleVolumeDown(void) {
  if (currentVolume > 5) {
    currentVolume -= 5;
    myDFPlayer.volume(currentVolume);
  }
  currentStatus = "VolumeDown";
  handleRoot();
}
