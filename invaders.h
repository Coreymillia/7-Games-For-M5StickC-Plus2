// invaders.h - Cosmic Invaders game for M5StickC Plus2
#ifndef INVADERS_H
#define INVADERS_H

#include "config.h"

class InvadersGame {
private:
  // Ball
  float ballX, ballY;
  float ballSpeedX, ballSpeedY;
  int prevBallX, prevBallY;
  
  // Paddle
  float paddleX;
  int prevPaddleX;
  
  // Invaders (16 blocks)
  int invaderX[16];
  int invaderY[16];
  uint16_t invaderColor[16];
  
  int score;
  int level;
  int gameSpeed;
  bool gameOver;
  int gamePhase;  // 0=start, 1=playing, 2=game over
  
  void resetLevel();
  void newLevel();
  void drawInvaders();
  void checkCollisions();
  
public:
  void init();
  void update();
  void draw();
  void handleInput();
  bool isGameOver() { return gameOver && gamePhase == 2; }
  int getScore() { return score; }
  const char* getName() { return "INVADERS"; }
};

extern InvadersGame invadersGame;

#endif
