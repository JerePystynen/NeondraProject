// Variables

uint8_t current_battery = 100;
uint8_t current_volume_m = 4;
uint8_t current_volume_sfx_percent = 60;  // SFXs are louder by default.
uint16_t current_track_m = 1;             // Starts with "001.mp3".
volatile bool m_should_play = false;
bool is_wifi_connected = false;
unsigned long current_helmet_fan_rpm = 0;
uint8_t fan_pwm_percent = 20;
bool is_fan_enabled = false;  // If 'fan_pwm_percent' is <20, disable the fan completely.

bool is_charger_connected = true;
bool is_microphone_enabled = false;

const uint8_t VOLUME_M_MAX = 8;
const uint8_t COUNT_M_TRACKS = 27;
const uint8_t VOLUME_SFX_MAX = 24;
const uint8_t COUNT_SFX_TRACKS = 24;

volatile unsigned long previous_loop_time_0_1 = 0;
volatile unsigned long previous_loop_time_0_2 = 0;

const String M_SONGS[] = {
  "PS1 Startup Sound",
  "Käärijä - Cha Cha Cha (Zardonic Remix)",
  "Martin Solveig - +1 (Delta Heavy Remix)",
  "Medic danced like a butterfly",
  "Accelerate (Sauniks Remix)",
  "Aktive - Nobody Like You",
  "Aktive - Obey Or Die",
  "Aktive - Sunchaser",
  "AKVA - Muse (Mizo Remix)",
  "Alex Metric - Gold Guns Girls (RIOT 87 Remix)",
  "Andromedik - First To Go ft. Ayah Marar",
  "Andy C & Fiora ‘Heartbeat Loud'",
  "Coppa - Anything feat. High Maintenance",
  "Getsix - Sky Fracture",
  "High Maintenance &amp; Mediks - Watching Me (Ft. Georgina Upton)",
  "How do you do (Nightcore remix)",
  "Kasger - Runnit",
  "Kayzo x Delta Heavy - Tormenta",
  "Lets Be Friends - FTW",
  "Neurachem (Voicians Remix)",
  "Touhou Rumia's theme",
  "Spider Dance (Shirobon Remix)",
  "USAO - Boss Rush (Extended Mix)",
  "USAO - Wildfire",
  "VOICIANS - Wolves 49",
  "Gambare Gambare Senpai (Bemax Remix)",
  "Toneshifterz - Can't Fail"
};

String SFX_TRACKS[] = {
  "PS1 Startup Sound",
  "Anime BOOM Soyaa",
  "Anime Wow",
  "Boo-womp",
  "R2-D2 scream",
  "Senko says poggers",
  "Yoimiya Tamaya",
  "Dialup"
};

/// GYROSCOPE ///

#include "g.h"

/// HELMET MATRIX ///

#include "leds.h"

// DFPLAYER MINIs

#include <HardwareSerial.h>
#include <DFPlayerMini_Fast.h>

HardwareSerial serial_dfplayer_m(2);
DFPlayerMini_Fast dfplayer_m;

HardwareSerial serial_dfplayer_sfx(3);
DFPlayerMini_Fast dfplayer_sfx;

bool is_dfplayer_m_playing() {
  return digitalRead(PIN_DFPLAYER_ISPLAYING) == 0;
}

void setup_dfplayer(DFPlayerMini_Fast player, HardwareSerial serial) {
  if (player.begin(serial, /* debug */ false, /* timeout */ 120)) {
    Serial.println("Connection success! Player address: " + String(reinterpret_cast<uintptr_t>(&player)));
    player.flush();
    player.reset();
    player.volume(current_volume_m);
    // player.stopDAC();
    // player.normalMode();
    // player.EQSelect(0);  // 0: Normal, 5: Base
    // player.playbackSource(0);
    // player.volumeAdjustSet(8);
    return;
  }
  Serial.println("Connection fail! Player address: " + String(reinterpret_cast<uintptr_t>(&player)));
}

void handle_m_play_toggle(void) {
  if (!is_dfplayer_m_playing()) {
    m_should_play = true;
    dfplayer_m.resume();
  } else {
    m_should_play = false;
    dfplayer_m.pause();
  }
}

void handle_m_play_next(void) {
  m_should_play = true;
  dfplayer_m.play(++current_track_m < 20 ? current_track_m : 1);
}

void handle_m_play_previous(void) {
  m_should_play = true;
  dfplayer_m.play(--current_track_m > 0 ? current_track_m : COUNT_M_TRACKS);
}

void handle_m_play_track(uint8_t track) {
  if (is_dfplayer_m_playing()) dfplayer_m.stop();
  if (track > COUNT_M_TRACKS) track = 0;
  current_track_m = track;
  dfplayer_m.play(current_track_m);
}

void handle_m_volume_up(void) {
  current_volume_m += 12;
  if (current_volume_m > 100) current_volume_m = 100;
  dfplayer_m.volume(map(current_volume_m, 0, 100, 0, VOLUME_M_MAX));
  //dfplayer_m.incVolume();
  //Serial.println("M: Increased volume");
}

void handle_m_volume_down(void) {
  current_volume_m -= 12;
  if (current_volume_m > 0) current_volume_m = 0;
  dfplayer_m.volume(map(current_volume_m, 0, 100, 0, VOLUME_M_MAX));
  //dfplayer_m.decVolume();
  //Serial.println("M: Decreased volume");
}

// HELMET FAN

volatile unsigned int revolutions;
volatile unsigned long last_interrupt_time = 0;
uint16_t fan_rpm;
bool is_fan_pwm_override;

void setPWM(int percent) {
  if (percent > 100) percent = 100;
  is_fan_pwm_override = percent > 20;
  fan_pwm_percent = is_fan_pwm_override ? percent : 20;

  if (percent < 20) {
  }
}

void controlFanPWM(void) {
  int pwm = fan_pwm_percent * 255 / 100;
  ledcWrite(4, pwm);
  current_helmet_fan_rpm = revolutions * 30;
  revolutions = 0;
}

ICACHE_RAM_ATTR void count_revolutions(void) {
  unsigned long current_time = millis();
  if (current_time - last_interrupt_time > 5) {
    revolutions++;  // Increment revolutions counter
    last_interrupt_time = current_time;
  }
}

// HELMET GYROSCOPE

#include <BMI160Gen.h>

const int select_pin = 10;
const int i2c_addr = 0x69;

/// ULTRA-SONIC BOOP SENSOR ///

void read_boop_sensor(void) {
  //if (is_playing_matrix_anim) return;
  digitalWrite(PIN_ULTRASONIC_TRIGGER, LOW);  // Clears the PIN_ULTRASONIC_TRIGGER
  delayMicroseconds(2);
  digitalWrite(PIN_ULTRASONIC_TRIGGER, HIGH);  // Sets the trigger pin HIGH for 10 micro seconds
  delayMicroseconds(10);
  digitalWrite(PIN_ULTRASONIC_TRIGGER, LOW);
  uint8_t distance = pulseIn(PIN_ULTRASONIC_ECHO, HIGH) * 0.034 / 2;  // Reads echo pin, returns sound wave travel time in microseconds, calculating distance
  if (distance > 0 && distance < 14) {
    Serial.print("\nBoop! Playing 'Snoot Boop Animation'!");
    // play_matrix_anim();
  }
}

/// LCD MONITOR ///

#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include "icons.h"

Adafruit_ST7735 tft = Adafruit_ST7735(PIN_LCD_CS, PIN_LCD_DC, PIN_LCD_RST);

void set_lcd_screen(void) {
  tft.initR(INITR_BLACKTAB);
  tft.setRotation(0);
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

void set_mic_icon(const uint16_t ICON[]) {
  tft.drawRGBBitmap(86, 8, ICON, 48, 48);  // xpos, ypos, array, width, height
}

void set_battery_icon(const uint16_t ICON[]) {
  tft.drawRGBBitmap(8, 8, ICON, 80, 48);  // xpos, ypos, array, width, height
}

void refresh_display(void) {
  // If the USB/Charging cable is connected, show as charging cable.
  uint8_t x = 0;
  if (!is_charger_connected) {
    x = 6;                                 // 100 %
    if (current_battery > 80) x = 5;       // 80 %
    else if (current_battery > 60) x = 4;  // 60 %
    else if (current_battery > 40) x = 3;  // 40 %
    else if (current_battery > 20) x = 2;  // 20 %
    else x = 1;                            // Critical
  }
  set_battery_icon(ICONS_BATTERY[x]);
  set_mic_icon(is_microphone_enabled ? ICON_MIC_OFF : ICON_MIC_OFF);

  tft.fillRect(0, 56, 128, 128, ST7735_BLACK);  // Fill the background
  tft.setCursor(8, 64);

  String timeclock = "23:21:20";
  tft.print(timeclock + "  ");
  tft.print("RPM:" + String(current_helmet_fan_rpm) + "\n\n ");
  tft.print("M Vol:" + String(map(current_volume_m, 0, 100, 0, VOLUME_M_MAX)) + "% SFX Vol:" + String(current_volume_sfx_percent) + "%\n ");

  String battery_percentage = String(current_battery);
  tft.print("Bat:" + battery_percentage + "%\n ");

  tft.print("M:" + String(is_dfplayer_m_playing() ? "Playing" : "Paused") + " SFX:" + "Offline" + "\n ");
  tft.print(M_SONGS[current_track_m - 1] + "\n ");

  String spitfire_connected = false ? "Online" : "Offline";
  tft.print("Spitfire: " + spitfire_connected);
}

// SETUP ///

void setup_c0(void) {
  set_lcd_screen();   // LCD
  setup_gyro();       // HELMET GYRO
  setup_suit_leds();  // HELMET MATRICES

  // Ultrasonic Boop-the-snoot Sensor
  pinMode(PIN_ULTRASONIC_TRIGGER, OUTPUT);  // Sets the PIN_ULTRASONIC_TRIGGER as an Output
  pinMode(PIN_ULTRASONIC_ECHO, INPUT);      // Sets the PIN_ULTRASONIC_ECHO as an Input

  // DFPlayer Minis
  //pinMode(PIN_DFPLAYER_ISPLAYING, INPUT_PULLUP);
  //serial_dfplayer_m.begin(9600, SERIAL_8N1, PIN_DFPLAYER_M_RX, PIN_DFPLAYER_M_TX);
  //setup_dfplayer(dfplayer_m, serial_dfplayer_m);
  //serial_dfplayer_sfx.begin(9600, SERIAL_8N1, PIN_DFPLAYER_SFX_RX, PIN_DFPLAYER_SFX_TX);
  //setup_dfplayer(dfplayer_sfx, serial_dfplayer_sfx);

  // Helmet Fan
  pinMode(PIN_HELMET_FAN_PWM, OUTPUT);
  pinMode(PIN_HELMET_FAN_SENSE, INPUT_PULLUP);
  pinMode(PIN_HELMET_FAN_ENABLE, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(PIN_HELMET_FAN_SENSE), count_revolutions, RISING);
  ledcSetup(4, 25000, 8);                // Configure PWM channel 4, 8-bit resolution
  ledcAttachPin(PIN_HELMET_FAN_PWM, 4);  // Attach PWM channel to pin
  digitalWrite(PIN_HELMET_FAN_ENABLE, HIGH);

  Serial.print("main: c0 running!");
}

void loop_c0(void) {
  // Gyros are read every 60ms.
  if (millis() - previous_loop_time_0_1 >= 33) {
    update_gyro();

    // if (m_should_play) { // If is not playing, play a new track.
    //   if (!is_dfplayer_m_playing()) {
    //     dfplayer_m.play(++current_track_m < COUNT_M_TRACKS ? current_track_m : 1);
    //   }
    // } else if (is_dfplayer_m_playing()) {
    //   dfplayer_m.stop();
    // }
    previous_loop_time_0_1 = millis();
  }
  // Matrix, Fan, LCD are updated every 1000ms.
  if (millis() - previous_loop_time_0_2 >= 1000) {
    update_matrix_framedata();

    //read_boop_sensor();
    //controlFanPWM();
    //refresh_display();
    //   // Serial.print(" | F1 RPM: " + String(current_helmet_fan_rpm));
    //   // Serial.print(" | M play?: " + String(m_should_play));
    //   // Serial.print(" | M State: " + String(is_dfplayer_m_playing()));
    //   // Serial.print(" | M Vol: " + String(current_volume_m));
    previous_loop_time_0_2 = millis();
  }
}