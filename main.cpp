// main.cpp - M5StickC Plus2 Game System
#include <Arduino.h>
#include "config.h"
#include "display.h"
#include "input.h"
#include "gamemanager.h"

void showSplash() {
  clearDisplay();
  drawCenteredText("M5STICK", 40, 3, COLOR_CYAN);
  drawCenteredText("GAMES", 70, 3, COLOR_YELLOW);
  drawCenteredText("8 Games Collection", 110, 1, COLOR_WHITE);
  delay(2000);
}

void setup() {
  Serial.begin(115200);
  
  initInput();
  initDisplay();
  
  showSplash();
  
  initGameManager();
}

void loop() {
  updateInput();
  updateGameManager();
  delay(16); // ~60 FPS
}
