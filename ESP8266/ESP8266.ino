/*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^
Author: Neondra Studio
Project: Project Neondra
Year: 2024
Source: https://neondra.com/

Components:
  + ESP8266
  + DFPlayer Mini

Communication baud rates:
 - DFPlayer Mini 9600
 - Serial 9600

LCD Monitor Wiring definitions:
  ESP8266   LCD
  ===================
  3V3     VCC
  GND     GND
  D1      A0 (D/C)
  D2      CS (CS)
  RX      RESET (RES)
  D7      SDA (DIN)
  D5      SCK (CLK)
          LED (Open in my test)

^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*/
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266mDNS.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"
#include <EEPROM.h>

// CONFIGURATION AND STATE DATA SAVING

void writeDataToEEPROM(int address, int data) {
  EEPROM.begin(512);  // Initialize EEPROM with size
  EEPROM.put(address, data);
  EEPROM.commit();     // Save the data to EEPROM
  EEPROM.end();        // Release EEPROM
}

int readDataFromEEPROM(int address) {
  int data;
  EEPROM.begin(512);
  EEPROM.get(address, data);
  EEPROM.end();
  return data;
}

/// WEBSERVER

// Wi-Fi Credentials
const char* stationSSID = "Jentle";
const char* stationPassword = "jemmu123";

// Access Point Credentials
const char* apSSID = "Neondra_AP";
const char* apPassword = "neondra123";

#include "interface_html.h"
const bool USE_INTERFACE = true;

IPAddress IPaddr (127, 0, 0, 1);
IPAddress IPmask (255, 255, 255, 0);

const bool USE_SECONDS = true;
int hours, minutes, seconds;

const char* ntpServerName = "pool.ntp.org";
WiFiUDP udp; // Create an instance of WiFiUDP to send and receive NTP packets
const long utcOffsetInSeconds = 2 * 60 * 60; // UTC offset in seconds (UTC+2)
NTPClient timeClient(udp, ntpServerName, utcOffsetInSeconds); // Create an instance of the NTPClient to get time

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
  return USE_SECONDS ? (hoursStr + ":" + minutesStr + ":" + secondsStr) : (hoursStr + ":" + minutesStr);
}

ESP8266WebServer server(80); // Create an instance of the server

/// END WEBSERVER

/// AUDIO PLAYER

//const int DFplayerIN = 12;      // 'D6'
//const int DFPlayerOUT = 14;     // 'D5'
//const int DFPlayerBUSYPin = 15; // 'D8'

const int DFplayerIN = 15;     // 'D8'
const int DFPlayerOUT = 12;    // 'D6'
const int DFPlayerBUSYPin = 2; // 'D4'

SoftwareSerial mySoftwareSerial(DFPlayerOUT, DFplayerIN); // RX, TX
DFRobotDFPlayerMini myDFPlayer;

uint8_t currentTrack = 1;
const uint8_t TRACKCOUNT = 20;
bool shouldPlay = true;
int currentVolume = 8; // Set your default volume level
const int VOLUME_MAX = 30;
String currentStatus = "";
const int VOLUME_THRESHOLD = 50; // Adjust as needed

const int numReadings = 10;
int readings[numReadings];
int indexX = 0;

uint8_t currentAnimationFrameIndex = 0;
unsigned long previousMillis = 0; // will store last time AnimationFrame was updated
const long interval = 1000;

/// END AUDIO PLAYER

// AUDIO ANALYSIS //

const uint8_t MEDIUM_THRESHOLD = 20;
const uint8_t LOUD_THRESHOLD = 52;

// END AUDIO ANALYSIS //

/// LCD MONITOR ///
#include "lcd_icons.h"
#define TFT_CS 4
#define TFT_RST 16
#define TFT_DC 5

const uint16_t* ICONS[7] = {
  ICON_BATTERY_CHARGING,
  ICON_BATTERY_CRITICAL,
  ICON_BATTERY_20,
  ICON_BATTERY_40,
  ICON_BATTERY_60,
  ICON_BATTERY_80,
  ICON_BATTERY_100
};

// Create an instance of the display
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

int tempSeconds = 0;
int currentBattery = 100;
int currentIndex = 0; // TEMP

/// END LCD MONITOR ///

/// SPITFIRE
WiFiClient client;

void messageSpitfire(void) {
  // Wait until the client sends some data
  Serial.println("New client connected");
  while(!server.client()){
    delay(1);
  }
  HTTPClient http;
  String clientIP = server.client().remoteIP().toString();
  Serial.println(clientIP);
  String url = "http://" + clientIP + "/activate?params=lorem+ipsum+dolor+sit+amet";
  http.begin(client, url);
  int httpCode = http.GET();
  if (httpCode > 0) {
    Serial.println("Hello, Spitfire! sent successfully.");
  } else {
    Serial.println("Error sending Hello.");
  }
  http.end();
}

void initializeInterface() {
  if (!USE_INTERFACE) return;
  // Connect to the existing Wi-Fi network as a station
  WiFi.hostname("Neondra Suit");
  WiFi.begin(stationSSID, stationPassword);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to Wi-Fi");
  // Define web server routes
  server.on("/", HTTP_GET, handleRoot);
  server.on("/play", HTTP_GET, handlePlay);
  server.on("/stop", HTTP_GET, handleStop);
  server.on("/volumeup", HTTP_GET, handleVolumeUp);
  server.on("/volumedown", HTTP_GET, handleVolumeDown);
  server.on("/playnext", HTTP_GET, handlePlayNext);
  server.on("/playprevious", HTTP_GET, handlePlayPrevious);

  // Communication between Neondra and Spitfire
  server.on("/receive", HTTP_GET, []() {
    Serial.println("Spitfire's initial ping received. Let's send something back!");
    server.send(200);
    messageSpitfire();
  });

  // Start the server
  server.begin();
  MDNS.addService("http", "tcp", 80);
  
  // Initialize and synchronize time
  timeClient.begin();
  timeClient.update();
}

void setup(void) {
  pinMode(DFPlayerBUSYPin, INPUT);
    
  Serial.begin(9600);
  while (!Serial) { ; }

  /*
  mySoftwareSerial.begin(9600);
  while (!mySoftwareSerial) { ; }
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
  myDFPlayer.volume(currentVolume);
  Serial.println("Current volume: " + String(currentVolume));
  myDFPlayer.play(currentTrack);
  */
  
  // LCD MONITOR
  // Initialize the display
  tft.initR(INITR_BLACKTAB);
  tft.setRotation(-45);
  // Fill the background
  tft.fillScreen(ST7735_BLACK);
  tft.setTextColor(ST7735_WHITE);
  tft.setTextSize(1);
  tft.setTextWrap(false);
  tft.drawRGBBitmap(8, 8, ICON_NEONDRA, 112, 112); // xpos, ypos, array, width, height
  // Write the text
  tft.setCursor(24, 122);
  tft.print("NEONDRA ONLINE");

  initializeInterface();
  
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

  

  // Initialize the array with 0 values
  for (int i = 0; i < numReadings; i++) {
    readings[i] = 0;
  }

  delay(1000);
  tft.fillScreen(ST7735_BLACK);
  refreshDisplay();
  
  delay(1000); // Wait for the BUSY state to change
}

// Color the WS2812B strips:
void checkAudioIntensity(void) {
  // - Red: if the music is below 20 = Music is low-intensity and quiet
  // - Green: if the music is above 20 and below 52 = not too intense
  // - Yellow: if the music is above 52 = music is loud and intense
  uint8_t musicIntensity = digitalRead(A0);
  if (musicIntensity < MEDIUM_THRESHOLD) {
    // Low intensity; Red
    setColorTheme(1);
  } else if (musicIntensity < LOUD_THRESHOLD) {
    // Medium intensity; Green
    setColorTheme(2);
  } else {
    // High intensity; Yellow
    setColorTheme(3);
  }
}

void checkSuitPartConnectionsLoop() {
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

void refreshDisplay(void) {
  String timeclock = "23:21:" + (tempSeconds < 10 ? "0" + String(tempSeconds) : String(tempSeconds));
  String batteryPercentage = String(currentBattery);
  String volumePercentage = String(currentVolume);
  String playState = currentBattery % 2 == 0 ? "Playing" : "Paused";
  String microphoneEnabled = currentBattery % 2 == 0 ? "On" : "Off";
  String spitfireConnected = currentVolume % 2 == 0 ? "Connected" : "Disconnected";
  // Fill the background
  tft.fillRect(0, 56, 128, 128, ST7735_BLACK);
  tft.setCursor(8, 64);
  tft.print(timeclock + "\n\n ");
  tft.print("Bat:" + batteryPercentage + "%\n ");
  tft.print("Vol:" + volumePercentage + "%\n ");
  tft.print("Sword:" + spitfireConnected + "\n ");
  tft.print("Music:" + playState + "\n " + "Rick Astley - Never Gonna Give You Up");
}

void setMicIcon(const uint16_t ICON[]) {
  tft.drawRGBBitmap(86, 8, ICON, 48, 48); // xpos, ypos, array, width, height
}
void setBatteryIcon(const uint16_t ICON[]) {
  tft.drawRGBBitmap(8, 8, ICON, 80, 48); // xpos, ypos, array, width, height
}

void playNext(void) {
  currentTrack++;
  if (currentTrack > TRACKCOUNT) currentTrack = 1;
  myDFPlayer.play(currentTrack);
  shouldPlay = true;
  currentStatus = "Next";
  Serial.println("DFPlayer: Playing track" + String(currentTrack));
}

// Interface
void handleRoot(void) {
  server.send(200, "text/html", HTML_CONTENT);
}

void handleAjax(void) {
  String data = server.arg("plain");
  Serial.println("Received data via Ajax POST: " + data);

  // Process the received data as needed

  server.send(200, "text/plain", "Data received successfully!");
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

void handlePlayNext(void) {
  playNext();
  handleRoot();
}

void handlePlayPrevious(void) {
  myDFPlayer.play(--currentTrack > 0 ? currentTrack : TRACKCOUNT);
  shouldPlay = true;
  currentStatus = "Previous";
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
  
  // A 1-second timer without delay
  // if the difference between the current time and last time
  // is bigger than the interval at which you want to update.
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    // save the last time you updated
    previousMillis = currentMillis;

    /*
    // Audio Player
    // Read the state of the BUSY pin
    int busyState = digitalRead(DFPlayerBUSYPin);
    if (shouldPlay && busyState == HIGH) {
      Serial.println("DFPlayer Mini has finished playing.");
      playNext();
      busyState = 0; // LOW
    }
    */

    // LCD MONITOR
    setMicIcon(currentIndex % 2 == 0 ? ICON_MIC_ON : ICON_MIC_OFF);
    currentIndex = ++currentIndex == 7 ? 0 : currentIndex;
    setBatteryIcon(ICONS[currentIndex]);
    refreshDisplay();

    // Transmit data over to Arduino Nano for Matrix controlling
    //int sensorValue = getA0Reading();
    //Serial.println("DAC_Right Value: " + String(sensorValue));
  }
  delay(100);
}
