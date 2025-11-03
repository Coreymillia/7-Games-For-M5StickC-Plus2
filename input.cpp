// input.cpp - Input implementation for M5StickC Plus2
#include "input.h"

ButtonState buttons;
ButtonState lastButtons;
UNIT_JOYC Joystick;

void initInput() {
  M5.begin();
  
  // Initialize JoyC Hat
  while (!Joystick.begin(&Wire, JoyC_ADDR, 0, 26, 100000L)) {
    Serial.println("JoyC Hat not found! Retrying...");
    delay(500);
  }
  Serial.println("JoyC Hat initialized!");
  
  // Initialize button states
  buttons = {false, false, false, false, false, false, false, false, false, false, false, false, false, false};
  lastButtons = buttons;
}

void updateInput() {
  M5.update();
  
  // Store previous state
  lastButtons = buttons;
  
  // Read JoyC joystick (12-bit ADC: 0-4095, center ~2048)
  uint16_t joyX = Joystick.getADCValue(POS_X);
  uint16_t joyY = Joystick.getADCValue(POS_Y);
  bool joyBtn = Joystick.getButtonStatus();
  
  // Convert joystick to directional buttons (axes swapped for M5Stick orientation)
  buttons.down = (joyX < 1200);  // Left on joystick = DOWN in game
  buttons.up = (joyX > 3000);    // Right on joystick = UP in game
  buttons.right = (joyY < 1600); // Down on joystick = RIGHT in game
  buttons.left = (joyY > 3000);  // Up on joystick = LEFT in game
  
  // Read M5StickC buttons
  buttons.btnA = M5.BtnA.isPressed();
  buttons.btnB = M5.BtnB.isPressed();
  buttons.joyBtn = (joyBtn == 0); // Joystick button (pressed when 0)
  
  // Detect button press events (just pressed)
  buttons.upPressed = buttons.up && !lastButtons.up;
  buttons.downPressed = buttons.down && !lastButtons.down;
  buttons.leftPressed = buttons.left && !lastButtons.left;
  buttons.rightPressed = buttons.right && !lastButtons.right;
  buttons.btnAPressed = buttons.btnA && !lastButtons.btnA;
  buttons.btnBPressed = buttons.btnB && !lastButtons.btnB;
  buttons.joyBtnPressed = buttons.joyBtn && !lastButtons.joyBtn;
}

bool isButtonPressed(int button) {
  return false;
}

bool wasButtonJustPressed(int button) {
  return false;
}
