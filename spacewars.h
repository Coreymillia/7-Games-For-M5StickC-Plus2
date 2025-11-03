// spacewars.h - Space Wars game for M5StickC Plus2
#ifndef SPACEWARS_H
#define SPACEWARS_H

#include "config.h"

class SpaceWarsGame {
private:
  // Player variables
  float playerX, playerY;
  float playerSpeed;
  int lives;
  int score;
  
  // Enemy variables
  float enemyX, enemyY;
  float enemySpeed;
  int enemyHealth;
  int maxEnemyHealth;
  
  // Bullet arrays
  float bulletX[10];
  float bulletY[10];
  float enemyBulletX[10];
  float enemyBulletY[10];
  
  // Rocket arrays
  float rocketX[10];
  float rocketY[10];
  int rockets;
  
  int bulletCounter;
  int enemyBulletCounter;
  int rocketCounter;
  int level;
  bool gameOver;
  int gamePhase; // 0=start, 1=playing, 2=game over
  
  void drawPlayer();
  void drawEnemy();
  void drawBullets();
  void drawUI();
  void updateBullets();
  void checkCollisions();
  void resetLevel();
  void nextLevel();
  
public:
  void init();
  void update();
  void draw();
  void handleInput();
  bool isGameOver() { return gameOver && gamePhase == 2; }
  int getScore() { return score; }
  const char* getName() { return "SPACE WARS"; }
};

extern SpaceWarsGame spacewarsGame;

#endif
