#include "Adafruit_GFX.h"
#include "MCUFRIEND_kbv.h"
#include "volume.h"
#define BLACK 0x0000

MCUFRIEND_kbv tft;

int a = 0, b = 4000, c = 1000, d = 3000;
int s = 2000;
int j, j2;
int i, i2;
int white;

void setup() {
  Serial.begin(9600);
  uint16_t ID = tft.readID();
  tft.begin(ID);
  tft.invertDisplay(true);
  tft.setRotation(1);
}

void loop() {
  tft.invertDisplay(true);
  tft.fillScreen(BLACK);
  tft.drawRGBBitmap(0, 0, test, 480, 320);
  white = tft.color565(255, 255, 255);
  while (1) {
    if (a < s) {
      j = 14 * (sin(PI * a / 2000));
      i = 14 * (cos(PI * a / 2000));
      j2 = 1 * (sin(PI * a / 2000));
      i2 = 1 * (cos(PI * a / 2000));
      tft.drawLine(i2 + 62, j2 + 240, i + 62, j + 240, white);
      j = 14 * (sin(PI * (a - 300) / 2000));
      i = 14 * (cos(PI * (a - 300) / 2000));
      j2 = 1 * (sin(PI * (a - 300) / 2000));
      i2 = 1 * (cos(PI * (a - 300) / 2000));
      tft.drawLine(i2 + 62, j2 + 240, i + 62, j + 240, 0x0000);
      tft.fillRect(50, 285, 30, 30, 0x0000);
      tft.setTextSize(2);
      tft.setTextColor(0xffff);
      tft.setCursor(50, 285);
      tft.print(a / 40); tft.print("%");
      a++;
    }
    if (b < s) {
      j = 14 * (sin(PI * b / 2000));
      i = 14 * (cos(PI * b / 2000));
      j2 = 1 * (sin(PI * b / 2000));
      i2 = 1 * (cos(PI * b / 2000));
      tft.drawLine(i2 + 180, j2 + 240, i + 180, j + 240, white);
      j = 14 * (sin(PI * (b - 300) / 2000));
      i = 14 * (cos(PI * (b - 300) / 2000));
      j2 = 1 * (sin(PI * (b - 300) / 2000));
      i2 = 1 * (cos(PI * (b - 300) / 2000));
      tft.drawLine(i2 + 180, j2 + 240, i + 180, j + 240, 0x0000);
      tft.fillRect(168, 285, 30, 30, 0x0000);
      tft.setTextSize(2);
      tft.setTextColor(0xffff);
      tft.setCursor(168, 285);
      tft.print(b / 40); tft.print("%");
      b++;
    }
    if (c < s) {
      j = 14 * (sin(PI * c / 2000));
      i = 14 * (cos(PI * c / 2000));
      j2 = 1 * (sin(PI * c / 2000));
      i2 = 1 * (cos(PI * c / 2000));
      tft.drawLine(i2 + 297, j2 + 240, i + 297, j + 240, white);
      j = 14 * (sin(PI * (c - 300) / 2000));
      i = 14 * (cos(PI * (c - 300) / 2000));
      j2 = 1 * (sin(PI * (c - 300) / 2000));
      i2 = 1 * (cos(PI * (c - 300) / 2000));
      tft.drawLine(i2 + 297, j2 + 240, i + 297, j + 240, 0x0000);
      tft.fillRect(286, 285, 30, 30, 0x0000);
      tft.setTextSize(2);
      tft.setTextColor(0xffff);
      tft.setCursor(286, 285);
      tft.print(c / 40); tft.print("%");
      c++;
    }
    if (d < s) {
      j = 14 * (sin(PI * d / 2000));
      i = 14 * (cos(PI * d / 2000));
      j2 = 1 * (sin(PI * d / 2000));
      i2 = 1 * (cos(PI * d / 2000));
      tft.drawLine(i2 + 414, j2 + 240, i + 414, j + 240, white);
      j = 14 * (sin(PI * (d - 300) / 2000));
      i = 14 * (cos(PI * (d - 300) / 2000));
      j2 = 1 * (sin(PI * (d - 300) / 2000));
      i2 = 1 * (cos(PI * (d - 300) / 2000));
      tft.drawLine(i2 + 414, j2 + 240, i + 414, j + 240, 0x0000);
      tft.fillRect(402, 285, 30, 30, 0x0000);
      tft.setTextSize(2);
      tft.setTextColor(0xffff);
      tft.setCursor(402, 285);
      tft.print(d / 40);
      tft.print("%");
      d++;
    } if (a > s) {
      j = 14 * (sin(PI * a / 2000));
      i = 14 * (cos(PI * a / 2000));
      j2 = 1 * (sin(PI * a / 2000));
      i2 = 1 * (cos(PI * a / 2000));
      tft.drawLine(i2 + 62, j2 + 240, i + 62, j + 240, white);
      j = 14 * (sin(PI * (a + 300) / 2000));
      i = 14 * (cos(PI * (a + 300) / 2000));
      j2 = 1 * (sin(PI * (a + 300) / 2000));
      i2 = 1 * (cos(PI * (a + 300) / 2000));
      tft.drawLine(i2 + 62, j2 + 240, i + 62, j + 240, 0x0000);
      tft.fillRect(50, 285, 30, 30, 0x0000);
      tft.setTextSize(2);
      tft.setTextColor(0xffff);
      tft.setCursor(50, 285);
      tft.print(a / 40); tft.print("%");
      a--;
    }
    if (b > s) {
      j = 14 * (sin(PI * b / 2000));
      i = 14 * (cos(PI * b / 2000));
      j2 = 1 * (sin(PI * b / 2000));
      i2 = 1 * (cos(PI * b / 2000));
      tft.drawLine(i2 + 180, j2 + 240, i + 180, j + 240, white);
      j = 14 * (sin(PI * (b + 300) / 2000));
      i = 14 * (cos(PI * (b + 300) / 2000));
      j2 = 1 * (sin(PI * (b + 300) / 2000));
      i2 = 1 * (cos(PI * (b + 300) / 2000));
      tft.drawLine(i2 + 180, j2 + 240, i + 180, j + 240, 0x0000);
      tft.fillRect(168, 285, 30, 30, 0x0000);
      tft.setTextSize(2);
      tft.setTextColor(0xffff);
      tft.setCursor(168, 285);
      tft.print(b / 40); tft.print("%");
      b--;
    }
    if (c > s) {
      j = 14 * (sin(PI * c / 2000));
      i = 14 * (cos(PI * c / 2000));
      j2 = 1 * (sin(PI * c / 2000));
      i2 = 1 * (cos(PI * c / 2000));
      tft.drawLine(i2 + 297, j2 + 240, i + 297, j + 240, white);
      j = 14 * (sin(PI * (c + 300) / 2000));
      i = 14 * (cos(PI * (c + 300) / 2000));
      j2 = 1 * (sin(PI * (c + 300) / 2000));
      i2 = 1 * (cos(PI * (c + 300) / 2000));
      tft.drawLine(i2 + 297, j2 + 240, i + 297, j + 240, 0x0000);
      tft.fillRect(286, 285, 30, 30, 0x0000);
      tft.setTextSize(2);
      tft.setTextColor(0xffff);
      tft.setCursor(286, 285);
      tft.print(c / 40); tft.print("%");
      c--;
    }
    if (d > s) {
      j = 14 * (sin(PI * d / 2000));
      i = 14 * (cos(PI * d / 2000));
      j2 = 1 * (sin(PI * d / 2000));
      i2 = 1 * (cos(PI * d / 2000));
      tft.drawLine(i2 + 414, j2 + 240, i + 414, j + 240, white);
      j = 14 * (sin(PI * (d + 300) / 2000));
      i = 14 * (cos(PI * (d + 300) / 2000));
      j2 = 1 * (sin(PI * (d + 300) / 2000));
      i2 = 1 * (cos(PI * (d + 300) / 2000));
      tft.drawLine(i2 + 414, j2 + 240, i + 414, j + 240, 0x0000);
      tft.fillRect(402, 285, 30, 30, 0x0000);
      tft.setTextSize(2);
      tft.setTextColor(0xffff);
      tft.setCursor(402, 285);
      tft.print(d / 40); tft.print("%");
      d--;
    }
  }
}
