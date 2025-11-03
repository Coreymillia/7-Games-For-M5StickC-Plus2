// display.cpp - Display implementation
#include "display.h"

void initDisplay() {
  M5.Lcd.setRotation(1); // Landscape mode
  M5.Lcd.fillScreen(COLOR_BLACK);
}

void clearDisplay() {
  M5.Lcd.fillScreen(COLOR_BLACK);
}

void updateDisplay() {
  // M5StickC Plus2 LCD updates immediately, no buffering needed
}

void drawCenteredText(const char* text, int y, int textSize, uint16_t color) {
  M5.Lcd.setTextSize(textSize);
  M5.Lcd.setTextColor(color);
  
  int16_t x1, y1;
  uint16_t w, h;
  
  // Estimate text width (6 pixels per char * textSize)
  w = strlen(text) * 6 * textSize;
  
  M5.Lcd.setCursor((SCREEN_WIDTH - w) / 2, y);
  M5.Lcd.print(text);
}

void drawText(const char* text, int x, int y, int textSize, uint16_t color) {
  M5.Lcd.setTextSize(textSize);
  M5.Lcd.setTextColor(color);
  M5.Lcd.setCursor(x, y);
  M5.Lcd.print(text);
}
