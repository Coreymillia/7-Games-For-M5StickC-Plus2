// flappy.cpp - Flappy Bird implementation
#include "flappy.h"
#include "display.h"
#include "input.h"

FlappyGame flappyGame;

void FlappyGame::init() {
  birdY = SCREEN_HEIGHT / 2;
  birdVelocity = 0;
  score = 0;
  gameOver = false;
  lastUpdate = 0;
  initPipes();
}

void FlappyGame::initPipes() {
  for (int i = 0; i < MAX_PIPES; i++) {
    pipes[i].x = SCREEN_WIDTH + i * 100;
    pipes[i].gapY = random(30, SCREEN_HEIGHT - 50);
    pipes[i].gapSize = 40;
    pipes[i].passed = false;
  }
}

void FlappyGame::update() {
  handleInput();
  
  unsigned long now = millis();
  if (now - lastUpdate < 30) return;
  lastUpdate = now;
  
  // Update bird physics
  birdVelocity += 0.5; // gravity
  birdY += birdVelocity;
  
  // Boundary check
  if (birdY < 0) birdY = 0;
  if (birdY > SCREEN_HEIGHT - 10) gameOver = true;
  
  updatePipes();
  checkCollision();
}

void FlappyGame::updatePipes() {
  for (int i = 0; i < MAX_PIPES; i++) {
    pipes[i].x -= 2;
    
    // Reset pipe
    if (pipes[i].x < -20) {
      pipes[i].x = SCREEN_WIDTH;
      pipes[i].gapY = random(30, SCREEN_HEIGHT - 50);
      pipes[i].passed = false;
    }
    
    // Score point
    if (!pipes[i].passed && pipes[i].x < 30) {
      pipes[i].passed = true;
      score++;
    }
  }
}

void FlappyGame::checkCollision() {
  int birdX = 30;
  
  for (int i = 0; i < MAX_PIPES; i++) {
    if (birdX + 8 > pipes[i].x && birdX < pipes[i].x + 20) {
      if (birdY < pipes[i].gapY || birdY + 8 > pipes[i].gapY + pipes[i].gapSize) {
        gameOver = true;
      }
    }
  }
}

void FlappyGame::draw() {
  clearDisplay();
  
  // Draw bird
  M5.Lcd.fillCircle(30, birdY, 4, COLOR_YELLOW);
  
  // Draw pipes
  for (int i = 0; i < MAX_PIPES; i++) {
    M5.Lcd.fillRect(pipes[i].x, 0, 20, pipes[i].gapY, COLOR_GREEN);
    M5.Lcd.fillRect(pipes[i].x, pipes[i].gapY + pipes[i].gapSize, 20, SCREEN_HEIGHT, COLOR_GREEN);
  }
  
  // Draw score
  M5.Lcd.setTextSize(2);
  M5.Lcd.setTextColor(COLOR_WHITE);
  M5.Lcd.setCursor(10, 10);
  M5.Lcd.print(score);
}

void FlappyGame::handleInput() {
  if (buttons.btnAPressed || buttons.upPressed) {
    birdVelocity = -4;
  }
}
