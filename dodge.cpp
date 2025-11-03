// dodge.cpp - Tetris Dodge implementation
#include "dodge.h"
#include "input.h"

DodgeGame dodgeGame;

// Tetromino shapes (same as Tetris)
static const int SHAPES[7][4][2] = {
  {{0,0}, {1,0}, {2,0}, {3,0}},  // I
  {{0,0}, {1,0}, {0,1}, {1,1}},  // O
  {{1,0}, {0,1}, {1,1}, {2,1}},  // T
  {{0,1}, {1,1}, {1,0}, {2,0}},  // S
  {{0,0}, {1,0}, {1,1}, {2,1}},  // Z
  {{0,0}, {1,0}, {2,0}, {2,1}},  // L
  {{0,0}, {1,0}, {2,0}, {0,1}}   // J
};

static const uint16_t PIECE_COLORS[7] = {
  COLOR_CYAN, COLOR_YELLOW, 0xF81F,  // Magenta = 0xF81F
  COLOR_GREEN, COLOR_RED, COLOR_ORANGE, COLOR_BLUE
};

void DodgeGame::init() {
  // Portrait mode
  M5.Lcd.setRotation(0);
  M5.Lcd.fillScreen(COLOR_BLACK);
  
  // Player starts at bottom center
  playerX = 50;
  playerY = 200;
  playerSize = 8;
  playerRotation = 0;
  
  // Initialize pieces
  for (int i = 0; i < MAX_PIECES; i++) {
    pieces[i].active = false;
  }
  
  score = 0;
  level = 1;
  gameOver = false;
  lastSpawnTime = millis();
  spawnInterval = 1500;  // Spawn every 1.5 seconds initially
}

void DodgeGame::spawnPiece() {
  // Find inactive piece slot
  for (int i = 0; i < MAX_PIECES; i++) {
    if (!pieces[i].active) {
      pieces[i].x = random(10, 110);
      pieces[i].y = -20;
      pieces[i].type = random(7);
      pieces[i].rotation = random(4);
      pieces[i].speed = 1.0 + (level * 0.2);
      pieces[i].active = true;
      break;
    }
  }
}

void DodgeGame::updatePieces() {
  for (int i = 0; i < MAX_PIECES; i++) {
    if (pieces[i].active) {
      // Clear old position
      clearPiece(pieces[i].x, pieces[i].y, pieces[i].type, pieces[i].rotation);
      
      // Move down
      pieces[i].y += pieces[i].speed;
      
      // Remove if off screen
      if (pieces[i].y > 240) {
        pieces[i].active = false;
        score += 10;  // Points for surviving
        
        // Level up every 100 points
        if (score % 100 == 0 && score > 0) {
          level++;
          spawnInterval = max(500, (int)(spawnInterval * 0.8));  // Spawn faster
        }
      }
      
      // Check collision with player
      if (checkCollision(playerX, playerY, i)) {
        gameOver = true;
      }
    }
  }
  
  // Spawn new pieces
  if (millis() - lastSpawnTime > spawnInterval) {
    spawnPiece();
    lastSpawnTime = millis();
  }
}

bool DodgeGame::checkCollision(int px, int py, int pieceIdx) {
  if (!pieces[pieceIdx].active) return false;
  
  FallingPiece* p = &pieces[pieceIdx];
  
  // Precise block-to-block collision
  // Check each block of the falling piece against each block of the player
  for (int i = 0; i < 4; i++) {
    // Get falling piece block position with rotation
    int fpx = SHAPES[p->type][i][0] - 1;
    int fpy = SHAPES[p->type][i][1] - 1;
    int frx, fry;
    switch(p->rotation % 4) {
      case 0: frx = fpx; fry = fpy; break;
      case 1: frx = -fpy; fry = fpx; break;
      case 2: frx = -fpx; fry = -fpy; break;
      case 3: frx = fpy; fry = -fpx; break;
    }
    int fbx = p->x + (frx + 1) * 8;
    int fby = p->y + (fry + 1) * 8;
    
    // Check against each player block
    for (int j = 0; j < 4; j++) {
      int ppx = SHAPES[2][j][0] - 1;  // Player is type 2 (T piece)
      int ppy = SHAPES[2][j][1] - 1;
      int prx, pry;
      switch(playerRotation % 4) {
        case 0: prx = ppx; pry = ppy; break;
        case 1: prx = -ppy; pry = ppx; break;
        case 2: prx = -ppx; pry = -ppy; break;
        case 3: prx = ppy; pry = -ppx; break;
      }
      int pbx = px + (prx + 1) * 8;
      int pby = py + (pry + 1) * 8;
      
      // Check if blocks overlap (within 7 pixels for block size)
      if (abs(fbx - pbx) < 7 && abs(fby - pby) < 7) {
        return true;
      }
    }
  }
  
  return false;
}

void DodgeGame::clearPiece(int x, int y, int type, int rotation) {
  // Draw black squares over piece with rotation around center block (1,1)
  for (int i = 0; i < 4; i++) {
    int px = SHAPES[type][i][0] - 1;  // Shift to rotate around (1,1)
    int py = SHAPES[type][i][1] - 1;
    
    // Apply rotation
    int rx, ry;
    switch(rotation % 4) {
      case 0: rx = px; ry = py; break;
      case 1: rx = -py; ry = px; break;
      case 2: rx = -px; ry = -py; break;
      case 3: rx = py; ry = -px; break;
    }
    
    int bx = x + (rx + 1) * 8;  // Shift back
    int by = y + (ry + 1) * 8;
    M5.Lcd.fillRect(bx, by, 8, 8, COLOR_BLACK);
  }
}

void DodgeGame::drawPiece(int x, int y, int type, int rotation, uint16_t color) {
  // Draw the tetromino with rotation around center block (1,1)
  for (int i = 0; i < 4; i++) {
    int px = SHAPES[type][i][0] - 1;  // Shift to rotate around (1,1)
    int py = SHAPES[type][i][1] - 1;
    
    // Apply rotation
    int rx, ry;
    switch(rotation % 4) {
      case 0: rx = px; ry = py; break;
      case 1: rx = -py; ry = px; break;  // 90 degrees
      case 2: rx = -px; ry = -py; break; // 180 degrees
      case 3: rx = py; ry = -px; break;  // 270 degrees
    }
    
    int bx = x + (rx + 1) * 8;  // Shift back
    int by = y + (ry + 1) * 8;
    M5.Lcd.fillRect(bx, by, 7, 7, color);
    M5.Lcd.drawRect(bx, by, 7, 7, COLOR_WHITE);
  }
}

void DodgeGame::drawPlayer() {
  // Draw player as T piece (type 2) with rotation in bright color
  drawPiece(playerX, playerY, 2, playerRotation, COLOR_YELLOW);
}

void DodgeGame::update() {
  if (gameOver) return;
  
  handleInput();
  updatePieces();
}

void DodgeGame::draw() {
  // Draw active pieces
  for (int i = 0; i < MAX_PIECES; i++) {
    if (pieces[i].active) {
      drawPiece(pieces[i].x, pieces[i].y, pieces[i].type, 
                pieces[i].rotation, PIECE_COLORS[pieces[i].type]);
    }
  }
  
  // Draw player
  drawPlayer();
  
  // Draw UI
  M5.Lcd.setTextSize(1);
  M5.Lcd.setTextColor(COLOR_WHITE, COLOR_BLACK);
  M5.Lcd.setCursor(5, 5);
  M5.Lcd.print("Score:");
  M5.Lcd.print(score);
  M5.Lcd.setCursor(85, 5);
  M5.Lcd.print("Lv:");
  M5.Lcd.print(level);
}

void DodgeGame::handleInput() {
  // Portrait controls (same as Tetris/Breakout)
  if (buttons.down && playerX > 0) {
    // Clear old position
    clearPiece(playerX, playerY, 2, playerRotation);
    playerX -= 3;
  }
  
  if (buttons.up && playerX < 100) {
    clearPiece(playerX, playerY, 2, playerRotation);
    playerX += 3;
  }
  
  if (buttons.left && playerY > 20) {
    clearPiece(playerX, playerY, 2, playerRotation);
    playerY -= 3;
  }
  
  if (buttons.right && playerY < 210) {
    clearPiece(playerX, playerY, 2, playerRotation);
    playerY += 3;
  }
  
  // Rotate with joystick button press
  static bool rotateHeld = false;
  if (buttons.joyBtnPressed && !rotateHeld) {
    clearPiece(playerX, playerY, 2, playerRotation);
    playerRotation = (playerRotation + 1) % 4;
    rotateHeld = true;
  } else if (!buttons.joyBtn) {
    rotateHeld = false;
  }
}
