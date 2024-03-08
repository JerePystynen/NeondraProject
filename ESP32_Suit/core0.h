/*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^
  
[CORE 0]
  Communicates with the suit's parts and their speakers and LEDs.
  Controls a DFPlayer Mini.
  Analyses the audio output to visualize the music accordingly.

Components, all handled by core 0:
  Ultra-Sonic Boop Sensor.
  DFPlayer Mini.
  LCD Monitor.
  Helmet Matrices:
    "Mouth" Matrix 1x 32x8 + 2x 8x8.
    "Eye" Matrices 2x 8x8 + "Forehead" Matrix 1x 8x8.
    Headband Matrix 32x8 + 8x8.
    Left and Right Ear rings.
  Resistor network to read which components are connected..
  Optional (the suit can function normally without them being attached):
    Suit WS2812B LEDs:
      Left and Right Shoulder Matrix 35x LEDs.
      Chest middle 8x LEDs.
      Wings.
      Left and Right Ankles.
      Tail 20x LEDs.

DFPlayer Mini Tracks:
  0001 PS1 Startup Sound
  0002 Käärijä - Cha Cha Cha (Zardonic Remix)
  0003 Martin Solveig - +1 (Delta Heavy Remix)
  0004 Medic danced like a butterfly
  0005 Accelerate (Sauniks Remix)
  0006 Aktive - Nobody Like You
  0007 Aktive - Obey Or Die
  0008 Aktive - Sunchaser
  0009 AKVA - Muse (Mizo Remix)
  0010 Alex Metric - Gold Guns Girls (RIOT 87 Remix)
  0011 Andromedik - First To Go ft. Ayah Marar
  0012 Andy C & Fiora ‘Heartbeat Loud'
  0013 Coppa - Anything feat. High Maintenance
  0014 Getsix - Sky Fracture
  0015 High Maintenance &amp; Mediks - Watching Me (Ft. Georgina Upton)
  0016 How do you do (Nightcore remix)
  0017 Kasger - Runnit
  0018 Kayzo x Delta Heavy - Tormenta
  0019 Lets Be Friends - FTW
  0020 Neurachem (Voicians Remix)
  0021 Touhou Rumia's theme
  0022 Spider Dance (Shirobon Remix)
  0023 USAO - Boss Rush (Extended Mix)
  0024 USAO - Wildfire
  0025 VOICIANS - Wolves 49
  0026 Gambare Gambare Senpai (Bemax Remix)

*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*/

/// ULTRA-SONIC BOOP SENSOR ///

/*
  Modified code of Ultrasonic Sensor HC-SR04 and Arduino Tutorial
  by Dejan Nedelkovski, https://www.howtomechatronics.com
*/
const uint8_t trigPin = 0;
const uint8_t echoPin = 4;
long duration;
int distance;

void setupBoopSensor(void) {
  pinMode(trigPin, OUTPUT);  // Sets the trigPin as an Output
  pinMode(echoPin, INPUT);   // Sets the echoPin as an Input
  Serial.begin(9600);        // Starts the serial communication
}

bool readBoopSensor(void) {
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2;
  // Prints the distance on the Serial Monitor
  Serial.print("Distance: ");
  Serial.println(distance);
  if (distance < 10) {
    Serial.println("Boop!");
    delay(2000);
    return true;
  }
  return false;
}

void loopBoopSensor(void) {
  bool isBoopDetected = readBoopSensor();
}

/// DFPLAYER MINI ///

#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>

const byte RXD2 = 16;  // GPIO16 => Module's RX
const byte TXD2 = 17;  // GPIO17 => Module's TX

HardwareSerial df_sd(1);  // Use UART channel 1
DFRobotDFPlayerMini df_player_mini;

uint8_t current_track = 1;
const uint8_t TRACKCOUNT = 20;
bool should_play = true;
int current_volume = 8;  // Set your default volume level
const int VOLUME_MAX = 30;
String current_status = "";
const int VOLUME_THRESHOLD = 50;  // Adjust as needed

void playNext(void) {
  current_track++;
  if (current_track > TRACKCOUNT) current_track = 1;
  df_player_mini.play(current_track);
  should_play = true;
  current_status = "Next";
  Serial.println("DFPlayer: Playing track" + String(current_track));
}

/// END DFPLAYER MINI ///

/// LCD MONITOR ///

#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>

#include "icons.h"
#define TFT_CS 5
#define TFT_RST 4
#define TFT_DC 2

// Create an instance of the display
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

void set_lcd_screen(void) {
  tft.initR(INITR_BLACKTAB);
  tft.setRotation(-45);
  // Fill the background
  tft.fillScreen(ST7735_BLACK);
  tft.setTextColor(ST7735_WHITE);
  tft.setTextSize(1);
  tft.setTextWrap(false);
  tft.drawRGBBitmap(8, 8, ICON_NEONDRA, 112, 112);  // xpos, ypos, array, width, height
  // Write the text
  tft.setCursor(24, 122);
  tft.print("NEONDRA ONLINE");
}

/// END LCD MONITOR ///

/// HELMET AND OTHER (TORSO) LEDS ///

#include <FastLED.h>
#include <FastLED_NeoMatrix.h>

/// Color Theme

// Define a macro to convert RGB & GRB values to a uint16_t color value
#define RGB_TO_COLOR(r, g, b) (((uint16_t)r & 0xF8) << 8 | ((uint16_t)g & 0xFC) << 3 | ((uint16_t)b) >> 3)
#define GRB_TO_COLOR(g, r, b) (((uint16_t)r & 0xF8) << 8 | ((uint16_t)g & 0xFC) << 3 | ((uint16_t)b) >> 3)

const uint16_t COLORS_DATETIME[] PROGMEM = {
  GRB_TO_COLOR(200, 10, 0),    // 00:00 - 01:00 (Night Deep Red)
  GRB_TO_COLOR(180, 10, 0),    // 01:00 - 02:00 (Night Deep Red)
  GRB_TO_COLOR(160, 10, 0),    // 02:00 - 03:00 (Night Deep Red)
  GRB_TO_COLOR(140, 10, 0),    // 03:00 - 04:00 (Night Deep Red)
  GRB_TO_COLOR(150, 10, 0),    // 04:00 - 05:00 (Dawn Dark Red)
  GRB_TO_COLOR(180, 30, 0),    // 05:00 - 06:00 (Morning Red)
  GRB_TO_COLOR(200, 100, 0),   // 06:00 - 07:00 (Orange)
  GRB_TO_COLOR(200, 140, 10),  // 07:00 - 08:00 (Orange)
  GRB_TO_COLOR(200, 180, 20),  // 08:00 - 09:00 (Orange)
  GRB_TO_COLOR(200, 220, 30),  // 09:00 - 10:00 (Orange)
  GRB_TO_COLOR(0, 0, 255),     // 10:00 - 11:00 (Blue)
  GRB_TO_COLOR(0, 0, 255),     // 11:00 - 12:00 (Blue)
  GRB_TO_COLOR(0, 0, 255),     // 12:00 - 13:00 (Blue)
  GRB_TO_COLOR(0, 0, 255),     // 13:00 - 14:00 (Blue)
  GRB_TO_COLOR(0, 0, 255),     // 14:00 - 15:00 (Blue)
  GRB_TO_COLOR(0, 0, 255),     // 15:00 - 16:00 (Blue)
  GRB_TO_COLOR(0, 0, 255),     // 16:00 - 17:00 (Blue)
  GRB_TO_COLOR(0, 0, 255),     // 17:00 - 18:00 (Blue)
  GRB_TO_COLOR(200, 220, 30),  // 18:00 - 19:00 (Orange)
  GRB_TO_COLOR(200, 180, 20),  // 19:00 - 20:00 (Orange)
  GRB_TO_COLOR(200, 140, 10),  // 20:00 - 21:00 (Orange)
  GRB_TO_COLOR(200, 100, 0),   // 21:00 - 22:00 (Orange)
  GRB_TO_COLOR(250, 60, 0),    // 22:00 - 23:00 (Night Dark Red)
  GRB_TO_COLOR(230, 10, 0)     // 23:00 - 00:00 (Night Deep Red)
};

uint16_t COLORS_PIXEL[10] = {
  GRB_TO_COLOR(17, 47, 75),     // DBLUE  0x112F4B
  RGB_TO_COLOR(228, 224, 60),   // YELLO  0xE4E03C
  GRB_TO_COLOR(152, 10, 155),   // PINK   0x980A9B
  GRB_TO_COLOR(211, 0, 0),      // RED    0x6F2610
  GRB_TO_COLOR(10, 9, 4),       // BLACK  0x000000
  GRB_TO_COLOR(225, 215, 225),  // WHITE
  GRB_TO_COLOR(255, 165, 10),   // ORANGE
  GRB_TO_COLOR(20, 168, 30),    // GREEN
  GRB_TO_COLOR(20, 215, 215),   // CYAN
  GRB_TO_COLOR(40, 40, 215)     // BLUE
};

const uint16_t *COLORS_PIXEL_ORIG[10] = { COLORS_PIXEL };

void set_matrix(CRGB leds[], const uint8_t matrix[], uint8_t numColumns) {
  for (uint8_t col = 0; col < numColumns; col++) {
    for (uint8_t row = 0; row < 8; row++) {
      uint8_t i = pgm_read_byte(&(matrix[col * 8 + row]));
      leds[col * 8 + row] = COLORS_PIXEL[i];
    }
  }
  FastLED.show();
}

#define PIXEL_COUNT 896
//#include "anim.h"
#include "normal.h"

// 0: Helmet Matrices
CRGB leds_helmet[PIXEL_COUNT];

// Shoulder Ring Strips 2x 24, Insignia 4, Claws, Legs, Wings, Tail, and other CRGB Arrays.
// 1: Other LEDs
CRGB leds_other[0];

/// END HELMET AND OTHER (TORSO) LEDS ///

// Setup

void SETUP_CORE_0(void) {

  // Set up communication with the DFPlayer Mini and
  // tell the DFPlayer Mini to start playing "PS1 starting up sound effect"
  // while also setting up the communication with the 128x128 LCD.

  df_sd.begin(9600, SERIAL_8N1, RXD2, TXD2);  // 16, 17
  while (!df_sd) { ; }
  Serial.println(F("Initializing DFPlayer Mini..."));
  if (!df_player_mini.begin(df_sd, true, true)) {
    Serial.println(F("Unable to begin Communication with the DFPlayer Mini. Please recheck the connection or the SD card!"));
    while (true) { delay(0); }
  }
  Serial.println(F("DFPlayer Mini online."));
  df_player_mini.volume(current_volume);
  Serial.println("Current volume: " + String(current_volume));
  df_player_mini.play(current_track);

  // Initialize the LCD MONITOR

  set_lcd_screen();

  FastLED.setBrightness(14);
  FastLED.addLeds<WS2812B, 0, GRB>(leds_helmet, PIXEL_COUNT);

  delay(1000);  // Wait for the BUSY state to change
}

// Loop

void LOOP_CORE_0(void) {
  set_matrix(leds_helmet, neondra_normal_1, 112);  // 48 + 24 + 40
  delay(2000);
  set_matrix(leds_helmet, neondra_normal_2, 112);  // 48 + 24 + 40
  delay(2000);
  set_matrix(leds_helmet, neondra_normal_3, 112);  // 48 + 24 + 40
  delay(2000);
}