/*
  Modified code of Ultrasonic Sensor HC-SR04 and Arduino Tutorial
  by Dejan Nedelkovski, https://www.howtomechatronics.com
*/
const int trigPin = 0;
const int echoPin = 4;
long duration;
int distance;

void setup(void) {
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  Serial.begin(9600); // Starts the serial communication
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

void loop(void) {
  bool a = readBoopSensor();
}