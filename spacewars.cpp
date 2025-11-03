// spacewars.cpp - Space Wars implementation
#include "spacewars.h"
#include "input.h"

// Include graphics
#include "rover.h"
#include "brod.h"
#include "brod1.h"

SpaceWarsGame spacewarsGame;

void SpaceWarsGame::init() {
  // Portrait mode like Tetris/Breakout
  M5.Lcd.setRotation(0);
  
  playerX = 50;
  playerY = 190;  // Player at BOTTOM
  playerSpeed = 2.0;
  lives = 3;
  score = 0;
  level = 1;
  
  enemyX = 40;
  enemyY = 40;    // Enemy at TOP
  enemySpeed = 0.5;
  enemyHealth = 50;
  maxEnemyHealth = 50;
  
  rockets = 999; // Unlimited rockets
  bulletCounter = 0;
  enemyBulletCounter = 0;
  rocketCounter = 0;
  
  gameOver = false;
  gamePhase = 1; // Start playing
  
  // Initialize bullet arrays
  for (int i = 0; i < 10; i++) {
    bulletX[i] = -20;
    bulletY[i] = -20;
    enemyBulletX[i] = -20;
    enemyBulletY[i] = -20;
    rocketX[i] = -20;
    rocketY[i] = -20;
  }
}

void SpaceWarsGame::update() {
  if (gamePhase != 1) return;
  
  handleInput();
  
  // Move enemy (left/right at top) - gets more erratic each level
  enemyX += enemySpeed;
  
  // Random direction changes make it erratic at higher levels
  if (random(100) < level) {  // More erratic as level increases
    enemySpeed = -enemySpeed;
  }
  
  if (enemyX > 90 || enemyX < 10) {
    enemySpeed = -enemySpeed;
  }
  
  // Update bullets
  updateBullets();
  
  // Enemy fires MUCH more frequently as level increases
  // Level 1: 5%, Level 2: 8%, Level 5: 17%, Level 10: 32%
  int fireChance = 5 + (level * 3);
  if (random(100) < fireChance) {
    enemyBulletX[enemyBulletCounter % 10] = enemyX + 16;  // Center of enemy
    enemyBulletY[enemyBulletCounter % 10] = enemyY + 32;  // Bottom of enemy
    enemyBulletCounter++;
  }
  
  // Check collisions
  checkCollisions();
  
  // Check game over
  if (lives <= 0) {
    gameOver = true;
    gamePhase = 2;
  }
  
  // Check level complete
  if (enemyHealth <= 0) {
    nextLevel();
  }
}

void SpaceWarsGame::updateBullets() {
  // Update player bullets (shoot UP)
  for (int i = 0; i < 10; i++) {
    if (bulletY[i] > 0) {
      bulletY[i] -= 3;
      if (bulletY[i] < 0) {
        bulletX[i] = -20;
        bulletY[i] = -20;
      }
    }
  }
  
  // Update enemy bullets (shoot DOWN)
  for (int i = 0; i < 10; i++) {
    if (enemyBulletY[i] > 0) {
      enemyBulletY[i] += 2;
      if (enemyBulletY[i] > 240) {
        enemyBulletX[i] = -20;
        enemyBulletY[i] = -20;
      }
    }
  }
  
  // Update rockets (shoot UP)
  for (int i = 0; i < 10; i++) {
    if (rocketY[i] > 0) {
      rocketY[i] -= 4;
      if (rocketY[i] < 0) {
        rocketX[i] = -20;
        rocketY[i] = -20;
      }
    }
  }
}

void SpaceWarsGame::checkCollisions() {
  // Player bullets hit enemy (rover at top - 32x32)
  for (int i = 0; i < 10; i++) {
    if (bulletX[i] > enemyX && bulletX[i] < enemyX + 32 &&
        bulletY[i] > enemyY && bulletY[i] < enemyY + 32) {
      enemyHealth -= 2;
      score += 10;
      bulletX[i] = -20;
      bulletY[i] = -20;
    }
  }
  
  // Rockets hit enemy (rover at top - 32x32)
  for (int i = 0; i < 10; i++) {
    if (rocketX[i] > enemyX && rocketX[i] < enemyX + 32 &&
        rocketY[i] > enemyY && rocketY[i] < enemyY + 32) {
      enemyHealth -= 10;
      score += 50;
      rocketX[i] = -20;
      rocketY[i] = -20;
    }
  }
  
  // Enemy bullets hit player (brod ships at bottom - ~48x48)
  for (int i = 0; i < 10; i++) {
    if (enemyBulletX[i] > playerX && enemyBulletX[i] < playerX + 49 &&
        enemyBulletY[i] > playerY && enemyBulletY[i] < playerY + 48) {
      lives--;
      enemyBulletX[i] = -20;
      enemyBulletY[i] = -20;
    }
  }
}

void SpaceWarsGame::nextLevel() {
  level++;
  
  // Progressive difficulty scaling - MORE AGGRESSIVE
  enemyHealth = 40 + (level * 20);  // Level 1=60, Level 5=140, Level 10=240
  maxEnemyHealth = enemyHealth;
  
  // Speed increases significantly
  if (enemySpeed < 3.5) {
    enemySpeed *= 1.25;  // 25% faster each level
  }
  
  score += 100 * level;  // Bonus points scale with level
  rockets = 999; // Always unlimited
  
  // Bonus life every 3 levels (more forgiving since it's harder)
  if (level % 3 == 0) {
    lives++;
  }
  
  // Reset positions
  enemyX = 40;
  enemyY = 40;
}

void SpaceWarsGame::draw() {
  static bool firstDraw = true;
  if (firstDraw) {
    M5.Lcd.fillScreen(0x0000);
    firstDraw = false;
  }
  
  // Clear old positions
  M5.Lcd.fillRect(0, 20, 135, 220, 0x0000);
  
  // Draw enemy sprite at TOP (rover - 32x32) - rotated CLOCKWISE 90 degrees
  M5.Lcd.setSwapBytes(true);
  M5.Lcd.pushImageRotateZoom(enemyX + 16, enemyY + 16, 16, 16, 90, 1.0, 1.0, 32, 32, rover);
  
  // Draw player sprite at BOTTOM - rotated COUNTER-CLOCKWISE 90 degrees (changes each level)
  int shipType = (level - 1) % 2;
  if (shipType == 0) {
    M5.Lcd.pushImageRotateZoom(playerX + 24, playerY + 20, 24, 20, -90, 1.0, 1.0, 49, 40, brod1);
  } else {
    M5.Lcd.pushImageRotateZoom(playerX + 24, playerY + 24, 24, 24, -90, 1.0, 1.0, 48, 48, brod);
  }
  
  // Debug: show positions
  M5.Lcd.setTextSize(1);
  M5.Lcd.setTextColor(COLOR_WHITE);
  M5.Lcd.fillRect(5, 220, 130, 15, COLOR_BLACK);
  M5.Lcd.setCursor(5, 220);
  M5.Lcd.print("P:");
  M5.Lcd.print((int)playerY);
  M5.Lcd.print(" E:");
  M5.Lcd.print((int)enemyY);
  
  // Draw bullets
  for (int i = 0; i < 10; i++) {
    if (bulletY[i] > 0) {
      M5.Lcd.fillRect(bulletX[i], bulletY[i], 2, 4, COLOR_YELLOW);
    }
    if (enemyBulletY[i] > 0) {
      M5.Lcd.fillRect(enemyBulletX[i], enemyBulletY[i], 2, 4, COLOR_ORANGE);
    }
    if (rocketY[i] > 0) {
      M5.Lcd.fillRect(rocketX[i], rocketY[i], 3, 6, COLOR_CYAN);
    }
  }
  
  // Draw UI
  M5.Lcd.setTextSize(1);
  M5.Lcd.setTextColor(COLOR_WHITE);
  M5.Lcd.setCursor(5, 5);
  M5.Lcd.print("L:");
  M5.Lcd.print(level);
  M5.Lcd.print(" S:");
  M5.Lcd.print(score);
  
  M5.Lcd.setCursor(5, 15);
  M5.Lcd.print("Lives:");
  M5.Lcd.print(lives);
  M5.Lcd.print(" R:");
  M5.Lcd.print(rockets);
  
  // Enemy health bar
  int healthBarWidth = (enemyHealth * 60) / maxEnemyHealth;
  M5.Lcd.fillRect(70, 5, 60, 5, 0x0000);
  M5.Lcd.fillRect(70, 5, healthBarWidth, 5, COLOR_RED);
  M5.Lcd.drawRect(69, 4, 62, 7, COLOR_WHITE);
}

void SpaceWarsGame::handleInput() {
  // Same controls as Tetris (portrait mode)
  // Physical UP joystick (system "left") = move UP
  // Physical DOWN joystick (system "right") = move DOWN
  // Physical LEFT joystick (system "down") = move LEFT  
  // Physical RIGHT joystick (system "up") = move RIGHT
  
  if (buttons.left) { // Physical UP = move UP
    playerY -= playerSpeed;
    if (playerY < 100) playerY = 100;
  }
  
  if (buttons.right) { // Physical DOWN = move DOWN
    playerY += playerSpeed;
    if (playerY > 205) playerY = 205;
  }
  
  if (buttons.down) { // Physical LEFT = move LEFT
    playerX -= playerSpeed;
    if (playerX < 0) playerX = 0;
  }
  
  if (buttons.up) { // Physical RIGHT = move RIGHT
    playerX += playerSpeed;
    if (playerX > 100) playerX = 100;
  }
  
  // Fire button - Joystick button
  static bool fireHeld = false;
  if (buttons.joyBtnPressed && !fireHeld) {
    bulletX[bulletCounter % 10] = playerX + 15;
    bulletY[bulletCounter % 10] = playerY;
    bulletCounter++;
    fireHeld = true;
  } else if (!buttons.joyBtn) {
    fireHeld = false;
  }
  
  // Rocket button - Button A
  static bool rocketHeld = false;
  if (buttons.btnAPressed && !rocketHeld && rockets > 0) {
    rocketX[rocketCounter % 10] = playerX + 15;
    rocketY[rocketCounter % 10] = playerY;
    rocketCounter++;
    rockets--;
    rocketHeld = true;
  } else if (!buttons.btnA) {
    rocketHeld = false;
  }
}
