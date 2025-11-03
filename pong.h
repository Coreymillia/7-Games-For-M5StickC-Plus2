// pong.h - Ping Pong game for M5StickC Plus2
#ifndef PONG_H
#define PONG_H

#include "config.h"

struct Ball {
  float x, y;
  float dx, dy;
  float speed;
  int radius;
  float prevX, prevY;
};

struct Paddle {
  int x, y;
  int width, height;
  int dy;
  int prevY;
};

class PongGame {
private:
  Ball ball;
  Paddle player;
  Paddle enemy;
  int playerScore;
  int enemyScore;
  float aiTargetY;
  bool gameOver;
  
  void resetBall(int direction);
  void movePaddle(Paddle* paddle, int y);
  void updateAI();
  void checkCollisions();
  void drawScores();
  
public:
  void init();
  void update();
  void draw();
  void handleInput();
  bool isGameOver() { return gameOver; }
  int getScore() { return playerScore; }
  const char* getName() { return "PONG"; }
};

extern PongGame pongGame;

#endif
