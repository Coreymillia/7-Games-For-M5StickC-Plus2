// breakout.cpp - Breakout game implementation
#include "breakout.h"
#include "input.h"

BreakoutGame breakoutGame;

void BreakoutGame::init() {
  // Set portrait orientation like Tetris
  M5.Lcd.setRotation(0);
  
  // Portrait mode: width=135, height=240
  paddleX = 135 / 2 - PADDLE_WIDTH / 2;
  score = 0;
  lives = 3;
  gameOver = false;
  bricksRemaining = 0;
  
  // Initialize bricks with random pattern from start
  for (int row = 0; row < BRICK_ROWS; row++) {
    for (int col = 0; col < BRICK_COLS; col++) {
      bricks[row][col] = (random(100) < 85); // 85% spawn chance
      if (bricks[row][col]) bricksRemaining++;
    }
  }
  
  resetBall();
}

void BreakoutGame::resetBall() {
  ballX = 60;
  ballY = 200;
  ballSpeedX = 2;
  ballSpeedY = -2;
}

void BreakoutGame::update() {
  if (gameOver) return;
  
  handleInput();
  
  // Move ball
  ballX += ballSpeedX;
  ballY += ballSpeedY;
  
  // Ball collision with walls (portrait: 135 wide)
  if (ballX <= 0 || ballX >= 135 - BALL_SIZE) {
    ballSpeedX = -ballSpeedX;
    ballX = constrain(ballX, 0, 135 - BALL_SIZE);
  }
  
  if (ballY <= 0) {
    ballSpeedY = -ballSpeedY;
    ballY = 0;
  }
  
  // Ball collision with paddle (portrait: 240 tall)
  if (ballY + BALL_SIZE >= 240 - 15 && 
      ballY + BALL_SIZE <= 240 - 10 &&
      ballX + BALL_SIZE >= paddleX && 
      ballX <= paddleX + PADDLE_WIDTH) {
    ballSpeedY = -abs(ballSpeedY);
    // Add spin based on where ball hits paddle
    float hitPos = (ballX + BALL_SIZE/2 - paddleX) / PADDLE_WIDTH;
    ballSpeedX = (hitPos - 0.5) * 3;
  }
  
  // Ball falls off bottom (portrait: 240 tall)
  if (ballY > 240) {
    lives--;
    if (lives <= 0) {
      gameOver = true;
    } else {
      resetBall();
    }
  }
  
  // Check brick collisions
  checkCollisions();
  
  // Win condition - regenerate level
  if (bricksRemaining == 0) {
    // Regenerate bricks with random pattern filling top half
    for (int row = 0; row < BRICK_ROWS; row++) {
      for (int col = 0; col < BRICK_COLS; col++) {
        // 85% chance of brick appearing - fills more of the screen
        bricks[row][col] = (random(100) < 85);
        if (bricks[row][col]) bricksRemaining++;
      }
    }
    resetBall();
    ballSpeedX *= 1.05; // Slightly faster each level (5% instead of 10%)
    ballSpeedY *= 1.05;
  }
  
  // Free life every 600 points
  static int lastLifeScore = 0;
  if (score >= lastLifeScore + 600) {
    lives++;
    lastLifeScore = score;
  }
}

void BreakoutGame::checkCollisions() {
  for (int row = 0; row < BRICK_ROWS; row++) {
    for (int col = 0; col < BRICK_COLS; col++) {
      if (bricks[row][col] && checkBrickCollision(row, col)) {
        bricks[row][col] = false;
        bricksRemaining--;
        score += 10;
        ballSpeedY = -ballSpeedY;
        return; // Only one brick per frame
      }
    }
  }
}

bool BreakoutGame::checkBrickCollision(int row, int col) {
  int brickX = col * BRICK_WIDTH + 10;
  int brickY = row * BRICK_HEIGHT + BRICK_OFFSET_Y;
  
  return (ballX + BALL_SIZE >= brickX && 
          ballX <= brickX + BRICK_WIDTH &&
          ballY + BALL_SIZE >= brickY && 
          ballY <= brickY + BRICK_HEIGHT);
}

void BreakoutGame::draw() {
  static bool firstDraw = true;
  static float lastBallX = 0;
  static float lastBallY = 0;
  static float lastPaddleX = 0;
  
  if (firstDraw) {
    M5.Lcd.fillScreen(0x0000);
    firstDraw = false;
  }
  
  // Clear old ball position
  M5.Lcd.fillRect(lastBallX, lastBallY, BALL_SIZE, BALL_SIZE, 0x0000);
  
  // Clear old paddle position
  M5.Lcd.fillRect(lastPaddleX, 240 - 15, PADDLE_WIDTH, PADDLE_HEIGHT, 0x0000);
  
  // Draw bricks with colors based on row (10 rows now)
  for (int row = 0; row < BRICK_ROWS; row++) {
    uint16_t color;
    switch(row % 6) { // Cycle through 6 colors
      case 0: color = COLOR_RED; break;
      case 1: color = COLOR_ORANGE; break;
      case 2: color = COLOR_YELLOW; break;
      case 3: color = COLOR_GREEN; break;
      case 4: color = COLOR_CYAN; break;
      case 5: color = COLOR_BLUE; break;
      default: color = COLOR_WHITE; break;
    }
    
    for (int col = 0; col < BRICK_COLS; col++) {
      int x = col * BRICK_WIDTH + 10;
      int y = row * BRICK_HEIGHT + BRICK_OFFSET_Y;
      
      if (bricks[row][col]) {
        M5.Lcd.fillRect(x, y, BRICK_WIDTH - 2, BRICK_HEIGHT - 2, color);
      } else {
        // Clear broken bricks
        M5.Lcd.fillRect(x, y, BRICK_WIDTH - 2, BRICK_HEIGHT - 2, 0x0000);
      }
    }
  }
  
  // Draw paddle (portrait: 240 tall)
  M5.Lcd.fillRect(paddleX, 240 - 15, PADDLE_WIDTH, PADDLE_HEIGHT, COLOR_WHITE);
  
  // Draw ball
  M5.Lcd.fillRect(ballX, ballY, BALL_SIZE, BALL_SIZE, COLOR_WHITE);
  
  // Store positions for next frame
  lastBallX = ballX;
  lastBallY = ballY;
  lastPaddleX = paddleX;
  
  // Draw score and lives
  M5.Lcd.setTextSize(1);
  M5.Lcd.setTextColor(COLOR_WHITE);
  M5.Lcd.setCursor(5, 5);
  M5.Lcd.print("Score:");
  M5.Lcd.print(score);
  M5.Lcd.setCursor(5, 15);
  M5.Lcd.print("Lives:");
  M5.Lcd.print(lives);
}

void BreakoutGame::handleInput() {
  // Same controls as Tetris (portrait mode)
  // Physical LEFT joystick (system "down") = move paddle LEFT
  // Physical RIGHT joystick (system "up") = move paddle RIGHT
  
  if (buttons.down) { // Move paddle left
    paddleX -= 3;
    if (paddleX < 0) paddleX = 0;
  }
  
  if (buttons.up) { // Move paddle right
    paddleX += 3;
    if (paddleX > 135 - PADDLE_WIDTH)  // Portrait width
      paddleX = 135 - PADDLE_WIDTH;
  }
}
