// input.h - Input handling for M5StickC Plus2 with JoyC Hat
#ifndef INPUT_H
#define INPUT_H

#include "config.h"
#include "UNIT_MiniJoyC.h"

#define JoyC_ADDR 0x54
#define POS_X 0
#define POS_Y 1

struct ButtonState {
  bool up;
  bool down;
  bool left;
  bool right;
  bool btnA;
  bool btnB;
  bool joyBtn;
  bool upPressed;
  bool downPressed;
  bool leftPressed;
  bool rightPressed;
  bool btnAPressed;
  bool btnBPressed;
  bool joyBtnPressed;
};

extern ButtonState buttons;
extern UNIT_JOYC Joystick;

void initInput();
void updateInput();
bool isButtonPressed(int button);
bool wasButtonJustPressed(int button);

#endif
