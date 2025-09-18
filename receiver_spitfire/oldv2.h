/*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^
  Author: Neondra Studio (by "Jentle")
  Project: Spitfire
  Year: 2025
  Website: https://neondra.com/
  Board: ESP8266
^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*/
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>

/// OWN SERVER
const char* ssid = "Neondra_AP";
const char* password = "neondra123";
ESP8266WebServer server(80);

WiFiClient client;

/// LED CONTROLS
const int PIN_RED = 4;   // D2
const int PIN_GREEN = 0; // D3
const int PIN_BLUE = 2;  // D4

String currentColor = "";

void setColorTheme(String color) {
  currentColor = color;
  digitalWrite(PIN_RED, color == "red" ? HIGH : LOW);
  digitalWrite(PIN_GREEN, color == "green" ? HIGH : LOW);
  digitalWrite(PIN_BLUE, color == "blue" ? HIGH : LOW);
}

void messageNeondra() {
  HTTPClient http;
  http.begin(client, "http://127.0.0.1/receive");
  int httpCode = http.GET();
  if (httpCode > 0) {
    Serial.println("Hello, Neondra! sent successfully.");
    setColorTheme("blue");
  } else {
    Serial.println("Error sending Hello.");
  }
  http.end();
}

void setup() {
  Serial.begin(9600);

  pinMode(PIN_RED, OUTPUT);
  pinMode(PIN_GREEN, OUTPUT);
  pinMode(PIN_BLUE, OUTPUT);

  setColorTheme("red");

  WiFi.begin(ssid, password);
  WiFi.hostname("Neondra Spitfire");
  while (WiFi.status() != WL_CONNECTED) {
    setColorTheme("");
    delay(1000);
    setColorTheme("red");
    delay(1000);
  }
  server.on("/receive", HTTP_GET, []() {
    setColorTheme("green");
  });
  server.begin();

  // We ping Neondra first so that it knows spitfire is online!
  messageNeondra();
}

void loop() {
  server.handleClient();
}