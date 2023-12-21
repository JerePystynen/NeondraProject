/**********************************************************************
  Author: Neondra Studio
  Project: Project Neondra
  Year: 2023
  Source: https://neondra.com/
  
  This script is for ESP8266DEV board which is in the Neondra's stand.
  Controls animation, emotion and color.
**********************************************************************/
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <FastLED.h>
#include <AudioFileSourceICYStream.h>
#include <AudioFileSourceBuffer.h>
#include <AudioGeneratorMP3.h>
#include <AudioOutputI2SNoDAC.h>
#include "html_content.h"

// Define the LEDs
#define NUM_LEDS 64
#define DATA_PIN 4
#define BRIGHTNESS 24
CRGB leds[NUM_LEDS];

// Define the server
const char *ssid = "Jentle";
const char *password = "jemmu123";
ESP8266WebServer server(80);

//Define the audio stream
// Randomly picked URL
const char *URL="http://kvbstreams.dyndns.org:8000/wkvi-am";
//https://cdn.discordapp.com/attachments/1118974619899142264/1187096095944343552/The_only_thing_i_know_for_real_is_japanese_goblin.mp3
AudioGeneratorMP3 *mp3;
AudioFileSourceICYStream *file;
AudioFileSourceBuffer *buff;
AudioOutputI2SNoDAC *out;

// Called when a metadata event occurs (i.e. an ID3 tag, an ICY block, etc.
void MDCallback(void *cbData, const char *type, bool isUnicode, const char *string) {
  const char *ptr = reinterpret_cast<const char *>(cbData);
  (void) isUnicode; // Punt this ball for now
  // Note that the type and string may be in PROGMEM, so copy them to RAM for printf
  char s1[32], s2[64];
  strncpy_P(s1, type, sizeof(s1));
  s1[sizeof(s1)-1]=0;
  strncpy_P(s2, string, sizeof(s2));
  s2[sizeof(s2)-1]=0;
  Serial.printf("METADATA(%s) '%s' = '%s'\n", ptr, s1, s2);
  Serial.flush();
}

// Called when there's a warning or error (like a buffer underflow or decode hiccup)
void StatusCallback(void *cbData, int code, const char *string) {
  const char *ptr = reinterpret_cast<const char *>(cbData);
  // Note that the string may be in PROGMEM, so copy it to RAM for printf
  char s1[64];
  strncpy_P(s1, string, sizeof(s1));
  s1[sizeof(s1)-1]=0;
  Serial.printf("STATUS(%s) '%d' = '%s'\n", ptr, code, s1);
  Serial.flush();
}

void setColor(const String setDisplayParam) {
  uint32_t color = (uint32_t)strtol(setDisplayParam.c_str(), NULL, 16);
  fill_solid(leds, NUM_LEDS, color);
  FastLED.show();
}

void playAudio(const String playAudioParam) {
  uint32_t audioClip = (uint32_t)strtol(playAudioParam.c_str(), NULL, 16);
  Serial.println("\nPlaying audio clip " + playAudioParam);
  static int lastms = 0;
  if (mp3->isRunning()) {
    if (millis()-lastms > 1000) {
      lastms = millis();
      Serial.printf("Running for %d ms...\n", lastms);
      Serial.flush();
    }
    if (!mp3->loop()) mp3->stop();
  } else {
    Serial.printf("MP3 done\n");
    delay(1000);
  }
}

void handleRoot() {
  // If a command has been given
  if (server.arg("set-visor") != "") {
    String setDisplayParam = server.arg("set-visor");
    if (setDisplayParam == "0000FF" || setDisplayParam == "00FF00" || setDisplayParam == "FF0000") {
      setColor(setDisplayParam);
    }
  } else if (server.arg("play-audio") != "") {
    String playAudioParam = server.arg("play-audio");
    playAudio(playAudioParam);
  }
  server.send(200, "text/html", HTML_CONTENT);
}



uint32_t getTime() {
  if (UDP.parsePacket() == 0) { // If there's no response (yet)
    return 0;
  }
  UDP.read(NTPBuffer, NTP_PACKET_SIZE); // read the packet into the buffer
  // Combine the 4 timestamp bytes into one 32-bit number
  uint32_t NTPTime = (NTPBuffer[40] << 24) | (NTPBuffer[41] << 16) | (NTPBuffer[42] << 8) | NTPBuffer[43];
  // Convert NTP time to a UNIX timestamp:
  // Unix time starts on Jan 1 1970. That's 2208988800 seconds in NTP time:
  const uint32_t seventyYears = 2208988800UL;
  // subtract seventy years:
  uint32_t UNIXTime = NTPTime - seventyYears;
  return UNIXTime;
}

void sendNTPpacket(IPAddress& address) {
  memset(NTPBuffer, 0, NTP_PACKET_SIZE);  // set all bytes in the buffer to 0
  // Initialize values needed to form NTP request
  NTPBuffer[0] = 0b11100011;   // LI, Version, Mode
  // send a packet requesting a timestamp:
  UDP.beginPacket(address, 123); // NTP requests are to port 123
  UDP.write(NTPBuffer, NTP_PACKET_SIZE);
  UDP.endPacket();
}

inline int getSeconds(uint32_t UNIXTime) {
  return UNIXTime % 60;
}

inline int getMinutes(uint32_t UNIXTime) {
  return UNIXTime / 60 % 60;
}

inline int getHours(uint32_t UNIXTime) {
  return UNIXTime / 3600 % 24;
}



void setup() {
  Serial.begin(115200);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi");

  // Initialize the web server
  server.on("/", HTTP_GET, handleRoot);
  server.begin();
  Serial.println("HTTP server started");

  // Initialize the LED matrix
  FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
  fill_solid(leds, NUM_LEDS, CRGB(0x11, 0x2F, 0x4B));
  FastLED.show();
  Serial.println("Matrix ready");

  // Initialize the mp3 audio clip using URL
  audioLogger = &Serial;
  file = new AudioFileSourceICYStream(URL);
  file->RegisterMetadataCB(MDCallback, (void*)"ICY");
  buff = new AudioFileSourceBuffer(file, 2048);
  buff->RegisterStatusCB(StatusCallback, (void*)"buffer");
  out = new AudioOutputI2SNoDAC();
  mp3 = new AudioGeneratorMP3();
  mp3->RegisterStatusCB(StatusCallback, (void*)"mp3");
  mp3->begin(buff, out);
}

void loop() {
  server.handleClient();
}
