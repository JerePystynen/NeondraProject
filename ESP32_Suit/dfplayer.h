/*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*
TRACKS:
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

Credits:
  This ESP32 code is created by esp32io.com
  This ESP32 code is released in the public domain
  For more detail (instruction and wiring diagram), visit https://esp32io.com/tutorials/esp32-mp3-player
^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*/
#define CMD_PLAY_NEXT 0x01
#define CMD_PLAY_PREV 0x02
#define CMD_PLAY_W_INDEX 0x03
#define CMD_SET_VOLUME 0x06
#define CMD_SEL_DEV 0x09
#define CMD_PLAY_W_VOL 0x22
#define CMD_PLAY 0x0D
#define CMD_PAUSE 0x0E
#define CMD_SINGLE_CYCLE 0x19

#define DEV_TF 0x02
#define SINGLE_CYCLE_ON 0x00
#define SINGLE_CYCLE_OFF 0x01

void SETUP_DFPLAYER() {
  Serial.begin(9600);
  Serial2.begin(9600);
  delay(500);  // wait chip initialization is complete

  mp3_command(CMD_SEL_DEV, DEV_TF);  // select the TF card
  delay(200);                        // wait for 200ms

  mp3_command(CMD_PLAY, 0x0000);  // Play mp3
  //mp3_command(CMD_PAUSE, 0x0000);      // Pause mp3
  //mp3_command(CMD_PLAY_NEXT, 0x0000);  // Play next mp3
  //mp3_command(CMD_PLAY_PREV, 0x0000);  // Play previous mp3
  //mp3_command(CMD_SET_VOLUME, 30);     // Change volume to 30
}

void mp3_command(int8_t command, int16_t dat) {
  int8_t frame[8] = { 0 };
  frame[0] = 0x7e;                // starting byte
  frame[1] = 0xff;                // version
  frame[2] = 0x06;                // the number of bytes of the command without starting byte and ending byte
  frame[3] = command;             //
  frame[4] = 0x00;                // 0x00 = no feedback, 0x01 = feedback
  frame[5] = (int8_t)(dat >> 8);  // data high byte
  frame[6] = (int8_t)(dat);       // data low byte
  frame[7] = 0xef;                // ending byte
  for (uint8_t i = 0; i < 8; i++) {
    Serial2.write(frame[i]);
  }
}