// breakout.h - Breakout game for M5StickC Plus2
#ifndef BREAKOUT_H
#define BREAKOUT_H

#include "config.h"

#define PADDLE_WIDTH 30
#define PADDLE_HEIGHT 4
#define BALL_SIZE 4
#define BRICK_WIDTH 22
#define BRICK_HEIGHT 10
#define BRICK_ROWS 10
#define BRICK_COLS 6
#define BRICK_OFFSET_Y 20

class BreakoutGame {
private:
  float paddleX;
  float ballX, ballY;
  float ballSpeedX, ballSpeedY;
  int score;
  int lives;
  bool gameOver;
  bool bricks[BRICK_ROWS][BRICK_COLS];
  int bricksRemaining;
  
  void resetBall();
  void checkCollisions();
  bool checkBrickCollision(int row, int col);
  
public:
  void init();
  void update();
  void draw();
  void handleInput();
  bool isGameOver() { return gameOver; }
  int getScore() { return score; }
  const char* getName() { return "BREAKOUT"; }
};

extern BreakoutGame breakoutGame;

#endif
