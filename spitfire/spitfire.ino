/*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^
  Author: Neondra Studio (by "Jentle")
  Project: Spitfire
  Year: 2024
  Website: https://neondra.com/
  Board: ESP8266
^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*/
#include <EEPROM.h>
#include <FastLED.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#define PIN_SHAFT 2
#define NUM_SHAFT_LEDS 80

#define PIN_RINGS 3
#define NUM_BIGRING_LEDS 35
#define NUM_SMALLRING_LEDS 24
#define NUM_RINGS_LEDS (NUM_BIGRING_LEDS * 2 + NUM_SMALLRING_LEDS * 2)

const char* ssid = "Jentle";
const char* password = "jemmu123";
ESP8266WebServer server(80);

int swordModeAddress = 0; // EEPROM address to store data
int swordMode = 0;

CRGB leds_shaft[NUM_SHAFT_LEDS];
CRGB leds_rings[NUM_RINGS_LEDS];

void setSwordMode(int selectedMode) {
  swordMode = selectedMode;
  EEPROM.put(address, valueToSave); // Save the value to EEPROM
}

void setup(void) {
  Serial.begin(115200);
  
  EEPROM.get(swordModeAddress, swordMode); // Read the value from EEPROM
  Serial.println("SwordMode: " + swordMode);

  // Initialize the WiFi for HTTP packets on how to display the colors, if UV meter is enabled.
  WiFi.begin(ssid, password);
  WiFi.hostname("Neondra Spitfire"); // Set the custom hostname
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  server.on("/receive", HTTP_GET, [](){
    Serial.println("Hello World received!");
    server.send(200, "text/plain", "Hello World received!");
  });
  server.begin();
  
  // Initialize the Matrices.
  // Shaft: 80
  leds_shaft = FastLED.addLeds<WS2812B, GRB>(PIN_SHAFT, NUM_SHAFT_LEDS);
  // Rings: 35x2 + 24x2
  leds_rings = FastLED.addLeds<WS2812B, GRB>(PIN_RINGS, NUM_RINGS_LEDS);
}

void loop(void) {
  server.handleClient();
  // Animate the matrices...
}
