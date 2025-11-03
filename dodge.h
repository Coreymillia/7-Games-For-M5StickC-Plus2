// dodge.h - Tetris Dodge game for M5StickC Plus2
#ifndef DODGE_H
#define DODGE_H

#include "config.h"

struct FallingPiece {
  int x, y;
  int type;  // 0-6 for different tetromino types
  int rotation;
  bool active;
  float speed;
};

class DodgeGame {
private:
  // Player (T piece)
  int playerX, playerY;
  int playerSize;
  int playerRotation;
  
  // Falling pieces
  static const int MAX_PIECES = 8;
  FallingPiece pieces[MAX_PIECES];
  
  int score;
  int level;
  unsigned long lastSpawnTime;
  unsigned long spawnInterval;
  bool gameOver;
  
  void spawnPiece();
  void updatePieces();
  void drawPiece(int x, int y, int type, int rotation, uint16_t color);
  void drawPlayer();
  bool checkCollision(int px, int py, int pieceIdx);
  void clearPiece(int x, int y, int type, int rotation);
  
public:
  void init();
  void update();
  void draw();
  void handleInput();
  bool isGameOver() { return gameOver; }
  int getScore() { return score; }
  const char* getName() { return "DODGE"; }
};

extern DodgeGame dodgeGame;

#endif
