// invaders.cpp - Cosmic Invaders implementation
#include "invaders.h"
#include "input.h"
#include "p2.h"

InvadersGame invadersGame;

void InvadersGame::init() {
  // Portrait mode
  M5.Lcd.setRotation(0);
  
  score = 0;
  level = 1;
  gameSpeed = 7000;
  gameOver = false;
  gamePhase = 1;  // Start playing immediately
  
  // Ball position
  ballX = random(30, 100);
  ballY = 70;
  ballSpeedX = 0.5;
  ballSpeedY = 1.0;
  prevBallX = ballX;
  prevBallY = ballY;
  
  // Paddle
  paddleX = 45;
  prevPaddleX = paddleX;
  
  // Initialize invaders
  int startX[16] = {8,33,58,83,108,8,33,58,83,108,22,47,72,97,47,72};
  int startY[16] = {37,37,37,37,37,45,45,45,45,45,53,53,53,53,61,61};
  uint16_t colors[16] = {
    COLOR_RED,COLOR_RED,COLOR_RED,COLOR_RED,COLOR_RED,
    COLOR_GREEN,COLOR_GREEN,COLOR_GREEN,COLOR_GREEN,COLOR_GREEN,
    COLOR_ORANGE,COLOR_ORANGE,COLOR_ORANGE,COLOR_ORANGE,
    COLOR_CYAN,COLOR_CYAN
  };
  
  for (int i = 0; i < 16; i++) {
    invaderX[i] = startX[i];
    invaderY[i] = startY[i];
    invaderColor[i] = colors[i];
  }
  
  // Draw initial screen
  M5.Lcd.fillScreen(COLOR_BLACK);
  M5.Lcd.drawLine(0, 17, 0, 240, 0x5AEB);
  M5.Lcd.drawLine(0, 17, 135, 17, 0x5AEB);
  M5.Lcd.drawLine(134, 17, 134, 240, 0x5AEB);
}

void InvadersGame::update() {
  if (gamePhase != 1) return;
  
  handleInput();
  
  // Move ball
  ballY += ballSpeedY;
  ballX += ballSpeedX;
  
  // Ball hits top
  if (ballY < 21) {
    ballSpeedY = -ballSpeedY;
  }
  
  // Ball hits sides
  if (ballX >= 130) ballSpeedX = -ballSpeedX;
  if (ballX <= 4) ballSpeedX = -ballSpeedX;
  
  // Ball hits paddle
  if (ballY > 232 && ballX > paddleX && ballX < paddleX + 24) {
    ballSpeedY = -ballSpeedY;
    // Random angle
    float angles[4] = {0.25, 0.50, 0.75, 1.0};
    int directions[2] = {1, -1};
    ballSpeedX = angles[random(4)] * directions[random(2)];
  }
  
  // Check invader collisions
  checkCollisions();
  
  // Ball falls off bottom
  if (ballY > 240) {
    gameOver = true;
    gamePhase = 2;
  }
  
  // Level complete
  if (score == 16 || score == 33 || score == 50 || score == 67 || score == 84 || 
      score == 101 || score == 118 || score == 135 || score == 152 || score == 169) {
    newLevel();
  }
}

void InvadersGame::checkCollisions() {
  for (int i = 0; i < 16; i++) {
    if (invaderX[i] < 300 && ballX > invaderX[i] && ballX < invaderX[i] + 20 &&
        ballY > invaderY[i] && ballY < invaderY[i] + 5) {
      // Hit!
      invaderX[i] = 400;  // Move off screen
      ballSpeedY = -ballSpeedY;
      // Random angle
      float angles[4] = {0.25, 0.50, 0.75, 1.0};
      int directions[2] = {1, -1};
      ballSpeedX = angles[random(4)] * directions[random(2)];
      score++;
    }
  }
}

void InvadersGame::newLevel() {
  score++;
  level++;
  gameSpeed = max(2000, gameSpeed - 500);  // Faster each level
  
  ballY = 75;
  ballSpeedY = 1.0;
  ballX = random(30, 100);
  
  // Reset invaders
  int startX[16] = {8,33,58,83,108,8,33,58,83,108,22,47,72,97,47,72};
  for (int i = 0; i < 16; i++) {
    invaderX[i] = startX[i];
  }
  
  delay(1000);  // Brief pause
}

void InvadersGame::draw() {
  // Clear old ball position
  M5.Lcd.fillEllipse(prevBallX, prevBallY, 2, 2, COLOR_BLACK);
  prevBallX = ballX;
  prevBallY = ballY;
  
  // Clear old paddle
  if ((int)paddleX != prevPaddleX) {
    M5.Lcd.fillRect(prevPaddleX, 234, 24, 4, COLOR_BLACK);
    prevPaddleX = paddleX;
  }
  
  // Draw invaders
  for (int i = 0; i < 16; i++) {
    if (invaderX[i] < 300) {
      M5.Lcd.fillRect(invaderX[i], invaderY[i], 20, 4, invaderColor[i]);
    }
  }
  
  // Draw ball
  M5.Lcd.fillEllipse((int)ballX, (int)ballY, 2, 2, COLOR_WHITE);
  
  // Draw paddle
  M5.Lcd.fillRect((int)paddleX, 234, 24, 4, COLOR_WHITE);
  
  // Draw UI
  M5.Lcd.setTextSize(1);
  M5.Lcd.setTextColor(COLOR_WHITE, COLOR_BLACK);
  M5.Lcd.setCursor(0, 0);
  M5.Lcd.print("SCORE ");
  M5.Lcd.print(score);
  M5.Lcd.setCursor(99, 0);
  M5.Lcd.print("LVL");
  M5.Lcd.print(level);
  
  // Small delay based on level speed
  delayMicroseconds(gameSpeed);
}

void InvadersGame::handleInput() {
  // Same controls as other portrait games
  // Physical LEFT (system down) = move paddle left
  // Physical RIGHT (system up) = move paddle right
  
  if (buttons.down && paddleX > 2) {
    paddleX -= 1.5;
  }
  
  if (buttons.up && paddleX < 109) {
    paddleX += 1.5;
  }
  
  // Clamp paddle position
  if (paddleX <= 3) paddleX = 4;
  if (paddleX >= 108) paddleX = 107;
}
