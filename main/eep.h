/*
  // #INTERFACE
  // ssid=Jentle
  // password=jemmu123
  // useInterface=1
  #PLAYBACK
  musicDirectory=Music/Drum and Bass Dancefloor
  testsong=Addicted 2 The Bass (Tantrum Desire D & B Remix).mp3
  #SUIT
  suitMode=1
  suitAnimationState=1
  #DATETIME
  useSeconds=1
  scrollIntervalms=500
  #HELMET
  helmetBrightness=0.5
  helmetInsideVolume=0.5
  helmetOutsideVolume=0.5
  micFilterMode=0
  micSensitivity=1
  helmetFanSpeed=0.5
  mouthBrightness=0.5
  headbandBrightness=0.5
  lefteyeBrightness=0.5
  righteyeBrightness=0.5
  foreheadBrightness=0.5
  leftringBrightness=0.5
  rightringBrightness=0.5
  #TORSO
  torsoVolume=0.5
  torsoSubwooferVolume=0.5
  leftshoulderBrightness=0.5
  rightshoulderBrightness=0.5
  torsoFanSpeed=0.2
  #CLAWS
  clawBrightness=0.5
  #LEGS
  legBrightness=0.5
  #TAIL
  tailBrightness=0.5
  #WING
  wingTipBrightness=0.5
  wingSpikeBrightness=0.5
  wingMotorY=0.2
  wingVolume=0.5
*/

#include <EEPROM.h>

void writeDataToEEPROM(int address, int data) {
  EEPROM.begin(512);          // Initialize EEPROM with size
  EEPROM.put(address, data);  // Commit data to EEPROM
  EEPROM.commit();            // Save the data to EEPROM
  EEPROM.end();               // Release EEPROM
}

int readDataFromEEPROM(int address) {
  int data;
  EEPROM.begin(512);          // Initialize EEPROM with size
  EEPROM.get(address, data);  // Get data from address of size
  EEPROM.end();               // Release EEPROM
  return data;
}