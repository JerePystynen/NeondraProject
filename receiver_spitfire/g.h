#include <BMI160Gen.h>

int previous_x = 0;
int previous_y = 0;
int previous_z = 0;

const int INERTIA_THRESHOLD_X = 8;
const int INERTIA_THRESHOLD_Y = 8;
const int INERTIA_THRESHOLD_Z = 8;

const float INERTIA_FACTOR = 0.00012;

void setup_gyro(void) {
  BMI160.begin(BMI160GenClass::I2C_MODE, 0x68);  // GY-521 MPU-6050 default I2C address '0x69'
}

void get_gyro(int *x, int *y) {
  int ax, ay, az, gx, gy, gz;
  BMI160.readAccelerometer(ax, ay, az);
  BMI160.readGyro(gx, gy, gz);
  gx *= INERTIA_FACTOR;
  gy *= INERTIA_FACTOR;
  gz *= INERTIA_FACTOR;
  
  int current_x = min(INERTIA_THRESHOLD_X, abs(gx)) * (gx > 0 ? 1 : -1);
  int current_y = min(INERTIA_THRESHOLD_Y, abs(gy)) * (gy > 0 ? 1 : -1);
  int current_z = min(INERTIA_THRESHOLD_Z, abs(gz)) * (gz > 0 ? 1 : -1);

  // int current_x = abs(gx) > 1 ? (min(INERTIA_THRESHOLD_X, abs(gx)) * (gx > 0 ? 1 : -1)) : 0;
  // int current_y = abs(gy) > 1 ? (min(INERTIA_THRESHOLD_Y, abs(gy)) * (gy > 0 ? 1 : -1)) : 0;
  // int current_z = abs(gz) > 1 ? (min(INERTIA_THRESHOLD_Z, abs(gz)) * (gz > 0 ? 1 : -1)) : 0;

  // Adjustment for x axis
  int x_adjustment = min(INERTIA_THRESHOLD_X, abs(current_x - previous_x));
  if (current_x < previous_x) {
    current_x = previous_x - x_adjustment;
  } else if (current_x > previous_x) {
    current_x = previous_x + x_adjustment;
  }

  // Adjustment for y axis
  int y_adjustment = min(INERTIA_THRESHOLD_Y, abs(current_y - previous_y));
  if (current_y < previous_y) {
    current_y = previous_y - y_adjustment;
  } else if (current_y > previous_y) {
    current_y = previous_y + y_adjustment;
  }

  // Adjustment for z axis
  int z_adjustment = min(INERTIA_THRESHOLD_Z, abs(current_z - previous_z));
  if (current_z < previous_z) {
    current_z = previous_z - z_adjustment;
  } else if (current_z > previous_z) {
    current_z = previous_z + z_adjustment;
  }

  // Update previous readings
  previous_x = current_x;
  previous_y = current_y;
  previous_z = current_z;

  *x = current_x;
  *y = current_y + current_z;
}