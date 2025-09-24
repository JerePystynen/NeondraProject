// Source:
// https://www.circuitbasics.com/arduino-ohm-meter/

// Examples of the resistors are in this sheet:
// https://docs.google.com/spreadsheets/d/1W_uhhL5tW4ke7RFwoA-nKQAPrWWVSFHEoSCmMDLBSd4/edit?usp=sharing

int raw = 0;
int Vin = 5;
float Vout = 0;
float r1 = 1000;
float r2 = 0;
float buffer = 0;

int get_total_resistor_value() {
  raw = analogRead(PIN_RESISTOR_NETWORK);
  if (!raw) return 0;
  buffer = raw * Vin;
  Vout = buffer / 1024.0;
  buffer = (Vin / Vout) - 1;
  r2 = r1 * buffer;
  Serial.print("Vout: ");
  Serial.println(Vout);
  Serial.print("r2: ");
  Serial.println(r2);
  delay(1000);
  return 0;
}

const double SUIT_PARTS_RESISTOR_VALUES[] PROGMEM = {
  1,   // 0: Torso
  2,   // 1: Left Claw
  4,   // 2: Right Claw
  8,   // 3: Left Foot
  16,  // 4: Right Foot
  32,  // 5: Tail
  64   // 6: Wings
};

bool connected_pieces[] = {
  false,  // 0: Torso
  false,  // 1: Left Claw
  false,  // 2: Right Claw
  false,  // 3: Left Foot
  false,  // 4: Right Foot
  false,  // 5: Tail
  false   // 6: Wings
};

bool[] getConnectedPieces() {
  bool connections[] = { false, false, false, false, false, false };
  /*
  TOTAL_VALUE - READ_RESISTOR = MISSING_IF_ANY

  [EXAMPLE: Wings missing]
  float value = 127 - 63 = 64;

  float margin = 0.5;

  for (uint8_t i = 0; i < 7; i++) {
    int part_value = SUIT_PARTS_RESISTOR_VALUES[i];
    // check the value with margin
    if (part.value > value - margin && part.value < value + margin) {
      // Aight, wing has a value of 64
      // 64-64 == 0 so only the wings are missing!
    }
  }
  */
  return connections;
}

bool isTorsoAttached() {
  return connected_pieces[0];
}

bool isLeftClawAttached() {
  return connected_pieces[1];
}

bool isRightClawAttached() {
  return connected_pieces[2];
}

bool isLeftFootAttached() {
  return connected_pieces[3];
}

bool isRightFootAttached() {
  return connected_pieces[4];
}

bool isTailAttached() {
  return connected_pieces[5];
}

bool isWingsAttached() {
  return connected_pieces[6];
}