// display.h - Display functions for M5StickC Plus2
#ifndef DISPLAY_H
#define DISPLAY_H

#include "config.h"

void initDisplay();
void clearDisplay();
void updateDisplay();
void drawCenteredText(const char* text, int y, int textSize, uint16_t color = COLOR_WHITE);
void drawText(const char* text, int x, int y, int textSize, uint16_t color = COLOR_WHITE);

#endif
