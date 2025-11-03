// flappy.h - Flappy Bird style game
#ifndef FLAPPY_H
#define FLAPPY_H

#include "config.h"

#define MAX_PIPES 3

struct Pipe {
  int x;
  int gapY;
  int gapSize;
  bool passed;
};

class FlappyGame {
private:
  int birdY;
  float birdVelocity;
  int score;
  bool gameOver;
  unsigned long lastUpdate;
  Pipe pipes[MAX_PIPES];
  
  void initPipes();
  void updatePipes();
  void checkCollision();
  
public:
  void init();
  void update();
  void draw();
  void handleInput();
  bool isGameOver() { return gameOver; }
  int getScore() { return score; }
  const char* getName() { return "FLAPPY"; }
};

extern FlappyGame flappyGame;

#endif
