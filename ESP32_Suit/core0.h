/*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^
  
[CORE 0]
  Communicates with the suit's parts and their speakers and LEDs.
  Controls a DFPlayer Mini.
  Analyses the audio output to visualize the music accordingly.

Components:
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
      Tail 20 LEDs.

*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*/

/// DFPLAYER MINI ///

#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>

const byte RXD2 = 16; // GPIO16 => Module's RX
const byte TXD2 = 17; // GPIO17 => Module's TX

HardwareSerial dfSD(1);  // Use UART channel 1
DFRobotDFPlayerMini df_player_mini;

uint8_t current_track = 1;
const uint8_t TRACKCOUNT = 20;
bool should_play = true;
int current_volume = 8; // Set your default volume level
const int VOLUME_MAX = 30;
String current_status = "";
const int VOLUME_THRESHOLD = 50; // Adjust as needed

void playNext(void) {
  current_track++;
  if (current_track > TRACKCOUNT) current_track = 1;
  dfPlayerMini.play(current_track);
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

// Setup

void SETUP_CORE_0(void) {

  // Set up communication with the DFPlayer Mini and
  // tell the DFPlayer Mini to start playing "PS1 starting up sound effect"
  // while also setting up the communication with the 128x128 LCD.

  dfSD.begin(9600, SERIAL_8N1, RXD2, TXD2);  // 16, 17
  while (!dfSD) { ; }
  Serial.println(F("Initializing DFPlayer Mini..."));
  if (!dfPlayerMini.begin(dfSD, true, true)) {
    Serial.println(F("Unable to begin Communication with the DFPlayer Mini. Please recheck the connection or the SD card!"));
    while (true) { delay(0); }
  }
  Serial.println(F("DFPlayer Mini online."));
  dfPlayerMini.volume(current_volume);
  Serial.println("Current volume: " + String(current_volume));
  dfPlayerMini.play(current_track);

  // Initialize the LCD MONITOR

  set_lcd_screen();

  
  delay(1000);  // Wait for the BUSY state to change
}

// Loop

void LOOP_CORE_0(void) {

}