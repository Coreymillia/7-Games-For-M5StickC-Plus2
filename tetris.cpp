// tetris.cpp - Enhanced Tetris with modern features
#include "tetris.h"
#include "display.h"
#include "input.h"

TetrisGame tetrisGame;

void TetrisGame::init() {
  // Set landscape mode for better gameplay
  M5.Lcd.setRotation(0); // Rotated right once from 3
  
  // Initialize field
  for (int y = 0; y < FIELD_HEIGHT; y++) {
    for (int x = 0; x < FIELD_WIDTH; x++) {
      field[y][x] = 0;
    }
  }
  
  // CORRECTED piece definitions from PERFECT tetris
  int tempPieces[7][4][2][4] = {
    // O piece - square (doesn't rotate)
    {{{0,1,0,1},{0,0,1,1}},{{0,1,0,1},{0,0,1,1}},{{0,1,0,1},{0,0,1,1}},{{0,1,0,1},{0,0,1,1}}},
    // I piece - line
    {{{0,0,0,0},{-1,0,1,2}},{{-1,0,1,2},{0,0,0,0}},{{0,0,0,0},{-1,0,1,2}},{{-1,0,1,2},{0,0,0,0}}},
    // T piece - CORRECTED rotation
    {{{0,0,0,1},{-1,0,1,0}},{{1,0,-1,0},{0,0,0,-1}},{{0,0,0,-1},{-1,0,1,0}},{{1,0,-1,0},{0,0,0,1}}},
    // S piece
    {{{0,-1,0,1},{0,0,1,1}},{{0,1,1,0},{0,0,-1,1}},{{0,-1,0,1},{0,0,1,1}},{{0,1,1,0},{0,0,-1,1}}},
    // Z piece
    {{{0,-1,0,1},{0,0,-1,-1}},{{0,0,1,1},{0,-1,0,1}},{{0,-1,0,1},{0,0,-1,-1}},{{0,0,1,1},{0,-1,0,1}}},
    // J piece
    {{{1,0,-1,1},{0,0,0,-1}},{{0,-1,0,0},{0,0,1,2}},{{0,1,2,0},{0,0,0,1}},{{1,0,0,0},{1,1,0,-1}}},
    // L piece - CORRECTED rotation
    {{{0,0,1,2},{-1,0,0,0}},{{-1,0,0,0},{1,1,0,-1}},{{1,1,0,-1},{1,0,0,0}},{{1,0,0,0},{-1,-1,0,1}}}
  };
  memcpy(pieces, tempPieces, sizeof(pieces));
  
  // Colors for pieces
  pieceColors[0] = COLOR_YELLOW;
  pieceColors[1] = COLOR_CYAN;
  pieceColors[2] = COLOR_PURPLE;
  pieceColors[3] = COLOR_GREEN;
  pieceColors[4] = COLOR_RED;
  pieceColors[5] = COLOR_BLUE;
  pieceColors[6] = COLOR_ORANGE;
  
  score = 0;
  level = 1;
  linesCleared = 0;
  dropSpeed = 500;
  gameOver = false;
  lastDropTime = 0;
  lockDelayActive = false;
  
  // Modern features
  heldPiece = -1;
  nextPiece = random(0, 7);
  canHold = true;
  
  newPiece(false);
}

void TetrisGame::update() {
  handleInput();
  
  if (millis() - lastDropTime > dropSpeed) {
    posY++;
    if (test(posY, posX, currentPiece, currentRot)) {
      posY--;
      // Start lock delay when piece hits bottom
      if (!lockDelayActive) {
        lockDelayActive = true;
        lockDelayStart = millis();
      }
      
      // Check if lock delay has expired (500ms)
      if (millis() - lockDelayStart >= 500) {
        placePiece();
        clearLines();
        newPiece(true);
        if (test(posY, posX, currentPiece, currentRot)) {
          gameOver = true;
        }
      }
    } else {
      lockDelayActive = false;
    }
    lastDropTime = millis();
  }
}

void TetrisGame::draw() {
  static int lastScore = -1;
  static bool firstDraw = true;
  
  // Only clear screen on first draw
  if (firstDraw) {
    clearDisplay();
    // Draw border around field (only once)
    M5.Lcd.drawRect(OFFSET_X-1, OFFSET_Y-1, FIELD_WIDTH*BLOCK_SIZE+1, FIELD_HEIGHT*BLOCK_SIZE+1, COLOR_WHITE);
    firstDraw = false;
  }
  
  // Draw field blocks (only draw empty spaces as black to reduce flicker)
  for (int y = 0; y < FIELD_HEIGHT; y++) {
    for (int x = 0; x < FIELD_WIDTH; x++) {
      int px = OFFSET_X + x * BLOCK_SIZE;
      int py = OFFSET_Y + y * BLOCK_SIZE;
      if (field[y][x] > 0) {
        M5.Lcd.fillRect(px, py, BLOCK_SIZE-1, BLOCK_SIZE-1, pieceColors[field[y][x]-1]);
        M5.Lcd.drawRect(px, py, BLOCK_SIZE-1, BLOCK_SIZE-1, COLOR_WHITE);
      } else {
        M5.Lcd.fillRect(px, py, BLOCK_SIZE-1, BLOCK_SIZE-1, COLOR_BLACK);
      }
    }
  }
  
  // Draw ghost piece
  drawGhostPiece();
  
  // Draw current piece
  for (int i = 0; i < 4; i++) {
    int x = posX + pieces[currentPiece][currentRot][1][i];
    int y = posY + pieces[currentPiece][currentRot][0][i];
    if (y >= 0 && y < FIELD_HEIGHT && x >= 0 && x < FIELD_WIDTH) {
      int px = OFFSET_X + x * BLOCK_SIZE;
      int py = OFFSET_Y + y * BLOCK_SIZE;
      M5.Lcd.fillRect(px, py, BLOCK_SIZE-1, BLOCK_SIZE-1, pieceColors[currentPiece]);
      M5.Lcd.drawRect(px, py, BLOCK_SIZE-1, BLOCK_SIZE-1, COLOR_WHITE);
    }
  }
  
  // Draw hold and next pieces (these don't change often)
  drawHoldPiece();
  drawNextPiece();
  
  // Only update score when it changes
  if (score != lastScore) {
    M5.Lcd.fillRect(110, 5, 50, 10, COLOR_BLACK); // Clear old score
    M5.Lcd.setTextSize(1);
    M5.Lcd.setTextColor(COLOR_WHITE);
    M5.Lcd.setCursor(110, 5);
    M5.Lcd.print("S:");
    M5.Lcd.print(score);
    lastScore = score;
  }
}

void TetrisGame::handleInput() {
  static unsigned long lastMove = 0;
  static unsigned long lastLeftRight = 0;
  static bool buttonHeld = false;
  
  // Left movement - rotate counter-clockwise: use system "down"
  if (buttons.down && millis() - lastLeftRight > 80) {
    posX--;
    if (test(posY, posX, currentPiece, currentRot)) posX++;
    else if (lockDelayActive) lockDelayStart = millis(); // Reset lock delay
    lastLeftRight = millis();
  }
  // Right movement - rotate counter-clockwise: use system "up"
  else if (buttons.up && millis() - lastLeftRight > 80) {
    posX++;
    if (test(posY, posX, currentPiece, currentRot)) posX--;
    else if (lockDelayActive) lockDelayStart = millis();
    lastLeftRight = millis();
  }
  
  if (millis() - lastMove < 100) return;
  
  // Hard drop - use system "left" 
  if (buttons.leftPressed && !buttonHeld) {
    while(!test(posY + 1, posX, currentPiece, currentRot)) {
      posY++;
      score += 2; // Award more points for hard drop
    }
    lockDelayActive = false;
    lastDropTime = 0; // Force immediate lock
    lastMove = millis();
    buttonHeld = true;
  }
  // Soft drop - use system "right"
  else if (buttons.right) {
    posY++;
    if (test(posY, posX, currentPiece, currentRot)) {
      posY--;
    } else {
      score++; // Award points for soft drop
      lockDelayActive = false;
    }
    lastMove = millis();
    buttonHeld = true;
  }
  // Rotate - Joystick BUTTON = rotate
  else if (buttons.joyBtnPressed && !buttonHeld) {
    int newRot = (currentRot + 1) % 4;
    if (!test(posY, posX, currentPiece, newRot)) {
      currentRot = newRot;
      if (lockDelayActive) lockDelayStart = millis();
    }
    lastMove = millis();
    buttonHeld = true;
  }
  // Hold piece - M5 Button A (side button) = hold
  else if (buttons.btnAPressed && !buttonHeld) {
    holdPiece();
    lastMove = millis();
    buttonHeld = true;
  }
  else {
    buttonHeld = false;
  }
}

bool TetrisGame::test(int y, int x, int piece, int rot) {
  for (int i = 0; i < 4; i++) {
    int px = x + pieces[piece][rot][1][i];
    int py = y + pieces[piece][rot][0][i];
    
    if (px < 0 || px >= FIELD_WIDTH || py >= FIELD_HEIGHT) return true;
    if (py >= 0 && field[py][px] > 0) return true;
  }
  return false;
}

void TetrisGame::placePiece() {
  for (int i = 0; i < 4; i++) {
    int x = posX + pieces[currentPiece][currentRot][1][i];
    int y = posY + pieces[currentPiece][currentRot][0][i];
    if (y >= 0 && y < FIELD_HEIGHT && x >= 0 && x < FIELD_WIDTH) {
      field[y][x] = currentPiece + 1;
    }
  }
}

void TetrisGame::clearLines() {
  int linesThisClear = 0;
  
  for (int y = FIELD_HEIGHT - 1; y >= 0; y--) {
    bool fullLine = true;
    for (int x = 0; x < FIELD_WIDTH; x++) {
      if (field[y][x] == 0) {
        fullLine = false;
        break;
      }
    }
    
    if (fullLine) {
      linesThisClear++;
      score += 100;
      for (int yy = y; yy > 0; yy--) {
        for (int x = 0; x < FIELD_WIDTH; x++) {
          field[yy][x] = field[yy-1][x];
        }
      }
      for (int x = 0; x < FIELD_WIDTH; x++) {
        field[0][x] = 0;
      }
      y++;
    }
  }
  
  // Update lines cleared and check for level up
  if (linesThisClear > 0) {
    linesCleared += linesThisClear;
    
    // Level up every 10 lines
    int newLevel = 1 + (linesCleared / 10);
    if (newLevel > level) {
      level = newLevel;
      // Speed up (reduce dropSpeed by 10% per level)
      dropSpeed = max(100, 500 - (level - 1) * 40);
    }
  }
}

void TetrisGame::newPiece(bool setPiece) {
  if (setPiece) {
    canHold = true; // Reset hold ability
  }
  
  // Use next piece, generate new next
  if (nextPiece >= 0) {
    currentPiece = nextPiece;
    nextPiece = random(0, 7);
  } else {
    currentPiece = random(0, 7);
    nextPiece = random(0, 7);
  }
  
  currentRot = 0;
  posX = FIELD_WIDTH / 2 - 1;
  posY = 0;
  
  lockDelayActive = false;
}

void TetrisGame::holdPiece() {
  if (!canHold) return;
  
  if (heldPiece == -1) {
    heldPiece = currentPiece;
    newPiece(false);
  } else {
    int temp = currentPiece;
    currentPiece = heldPiece;
    heldPiece = temp;
    currentRot = 0;
    posX = FIELD_WIDTH / 2 - 1;
    posY = 0;
  }
  
  canHold = false;
  lockDelayActive = false;
}

int TetrisGame::calculateDropDistance() {
  int dropDist = 0;
  for (int testY = posY + 1; testY < FIELD_HEIGHT; testY++) {
    if (!test(testY, posX, currentPiece, currentRot)) {
      dropDist = testY - posY;
    } else {
      break;
    }
  }
  return dropDist;
}

void TetrisGame::drawGhostPiece() {
  int dropDist = calculateDropDistance();
  if (dropDist > 0) {
    int ghostY = posY + dropDist;
    for (int i = 0; i < 4; i++) {
      int x = posX + pieces[currentPiece][currentRot][1][i];
      int y = ghostY + pieces[currentPiece][currentRot][0][i];
      if (y >= 0 && y < FIELD_HEIGHT && x >= 0 && x < FIELD_WIDTH) {
        int px = OFFSET_X + x * BLOCK_SIZE;
        int py = OFFSET_Y + y * BLOCK_SIZE;
        M5.Lcd.drawRect(px, py, BLOCK_SIZE-1, BLOCK_SIZE-1, 0x4208); // Gray
      }
    }
  }
}

void TetrisGame::drawMiniPiece(int pieceType, int x, int y, int scale) {
  if (pieceType < 0 || pieceType > 6) return;
  for (int i = 0; i < 4; i++) {
    int px = x + (pieces[pieceType][0][1][i] * scale);
    int py = y + (pieces[pieceType][0][0][i] * scale);
    M5.Lcd.fillRect(px, py, scale-1, scale-1, pieceColors[pieceType]);
  }
}

void TetrisGame::drawHoldPiece() {
  static int lastLines = -1;
  
  M5.Lcd.fillRect(115, 15, 30, 30, COLOR_BLACK);
  M5.Lcd.drawRect(114, 14, 32, 32, COLOR_WHITE);
  M5.Lcd.setTextSize(1);
  M5.Lcd.setTextColor(COLOR_WHITE);
  M5.Lcd.setCursor(115, 48);
  M5.Lcd.print("HOLD");
  
  if (heldPiece >= 0) {
    drawMiniPiece(heldPiece, 122, 22, 5);
  }
  
  // Draw line counter below hold piece (only when changed)
  if (linesCleared != lastLines) {
    M5.Lcd.fillRect(115, 60, 40, 10, COLOR_BLACK);
    M5.Lcd.setTextSize(1);
    M5.Lcd.setTextColor(COLOR_CYAN);
    M5.Lcd.setCursor(115, 60);
    M5.Lcd.print(linesCleared);
    lastLines = linesCleared;
  }
}

void TetrisGame::drawNextPiece() {
  // Next piece display removed - just clear the area
  M5.Lcd.fillRect(155, 15, 30, 30, COLOR_BLACK);
}
