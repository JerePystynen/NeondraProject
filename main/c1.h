// Variables
unsigned long previous_loop_time_1 = 0;

// WEBSERVER

#include <WiFi.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <WebServer.h>
#include <HTTPClient.h>
#include "interface.h"

WebServer server(80);

void handleRoot(void) {
  server.send(200, "text/html", HTML_CONTENT);
}

void handleAjax(void) {
  String command = server.arg("plain");
  Serial.println("Received command via Ajax POST: " + command);

  // Process the received command as needed
  bool is_valid_command = false;
  if (command == "play='toggle'") {
    handle_m_play_toggle();
    is_valid_command = true;
  } else if (command == "play='next'") {
    handle_m_play_next();
    is_valid_command = true;
  } else if (command == "play='previous'") {
    handle_m_play_previous();
    is_valid_command = true;
  } else if (command == "set-volume-m='up'") {
    handle_m_volume_up();
    is_valid_command = true;
  } else if (command == "set-volume-m='down'") {
    handle_m_volume_down();
    is_valid_command = true;
  } else if (command.startsWith("set-volume-m=")) {  // Parse the received command
    // Extract m volume percent
    String percent = command.substring(13);
    Serial.print("Set M volume to: ");
    Serial.println(percent);
    // Handle m volume
    is_valid_command = true;
  } else if (command.startsWith("set-volume-sfx=")) {  // Parse the received command
    // Extract sfx volume percent
    String percent = command.substring(15);
    Serial.print("Set SFX volume to: ");
    Serial.println(percent);
    // Handle m volume
    is_valid_command = true;
  } else if (command.startsWith("playAnim=")) {
    // Extract animation name
    String name = command.substring(9);
    Serial.print("Playing animation: ");
    Serial.println(name);
    // Handle playing animation here
    is_valid_command = true;
  } else if (command.startsWith("play-audio-m=")) {
    // Extract audio name
    String name = command.substring(13);
    Serial.print("Playing music: ");
    Serial.println(name);
    // Handle playing music here
    is_valid_command = true;
  } else if (command.startsWith("play-audio-sfx=")) {
    // Extract sound effect name
    String name = command.substring(16);
    Serial.print("Playing sound effect: ");
    Serial.println(name);
    // Handle playing sound effect here
    is_valid_command = true;
  } else if (command.startsWith("set-fan=")) {
    // Extract fan percent
    int percent = command.substring(8).toInt();
    Serial.print("Setting fan speed to ");
    Serial.println(percent);
    setPWM(percent); // Handle setting fan speed here...
    is_valid_command = true;
  } else if (command.startsWith("set-brightness=")) {
    // Extract brightness percent
    int percent = command.substring(15).toInt();
    Serial.print("Setting brightness to ");
    Serial.println(percent);
    // Handle setting brightness here
    is_valid_command = true;
  } else if (command.startsWith("set-wing-position=")) {
    // Extract wing position percent
    int percent = command.substring(18).toInt();
    Serial.print("Setting wing position to ");
    Serial.println(percent);
    // Handle setting brightness here
    is_valid_command = true;
  }
  if (is_valid_command) server.send(200, "text/string", "Success: Received command via POST: " + command);
  else server.send(404, "text/string", "Error: Invalid command, not found!");
}

void initializeInterface(void) {
  // Connect to the existing Wi-Fi network as a station
  WiFi.hostname("Neondra Suit");
  WiFi.begin(STATION_SSID, STATION_PASSWORD);
  Serial.print("Connecting to Wi-Fi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  is_wifi_connected = WiFi.status() == WL_CONNECTED;
  Serial.print(is_wifi_connected ? " connect success!" : " connect failed!");

  // Define web server routes
  server.on("/", HTTP_GET, handleRoot);
  server.on("/ajax", HTTP_POST, handleAjax);

  // Communication between Neondra and Spitfire
  // server.on("/receive", HTTP_GET, []() {
  //   Serial.println("Spitfire's initial ping received. Let's send something back!");
  //   server.send(200);
  //   //messageSpitfire();
  // });

  server.begin();  // Start the server

  // Initialize and synchronize time
  // timeClient.begin();
  // timeClient.update();
}

void setup_c1(void) {
  initializeInterface();
  Serial.print("main: c1 running!");
}

void loop_c1(void) {
  server.handleClient();
  handle_serial_input();
}