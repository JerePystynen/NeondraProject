// Original: BT_RGB

#include <Adafruit_NeoPixel.h>

char BTdata = 0; //Variable for storing received data

#define PIN 9      // Output pin for led strip
#define NUMPIXELS 35 // Number of LED Diodes
#define SPEED 30

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);


void setup()
{
    Serial.begin(9600); //Sets the baud for serial data transmission                               
    strip.begin();
    strip.setBrightness(30);
    strip.show(); // Initialize all pixels to 'off'
}
void loop(){
   for(int i=0; i<NUMPIXELS; i++)
   if(Serial.available() > 0)  // Send data only when you receive data:
   {
      BTdata = Serial.read();        //Read the  incoming  data and store it into variable data
      Serial.print(BTdata);          //Print Value inside data in Serial monito
      
      if(BTdata == 'r')              // press "r" for RED
      colorWipe(strip.Color(255, 0, 0), SPEED);

      if(BTdata == 'g')              // press "g" for GREEN
      colorWipe(strip.Color(0, 255, 0), SPEED);

      if(BTdata == 'b')              // press "b" for BLUE
      colorWipe(strip.Color(0, 0, 255), SPEED);

      if(BTdata == 'y')              // press "y" for YELLOW
      colorWipe(strip.Color(255, 255, 0), SPEED);
      
      if(BTdata == 'c')              // press "c" for CYAN
      colorWipe(strip.Color(0, 255, 255), SPEED);
      
      if(BTdata == 'p')              // press "p" for PINK
      colorWipe(strip.Color(255, 0, 255), SPEED);

      if(BTdata == 'o')              // press "o" for ORANGE
      colorWipe(strip.Color(255, 100, 0), SPEED);

      if(BTdata == 'v')              // press "v" for VIOLET
      colorWipe(strip.Color(100, 0, 255), SPEED);

     
   }
}
void colorWipe(uint32_t color, int wait) {
  for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
    strip.setPixelColor(i, color);         //  Set pixel's color (in RAM)
    strip.show();                          //  Update strip to match
    delay(wait);                           //  Pause for a moment
  }
}


uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
