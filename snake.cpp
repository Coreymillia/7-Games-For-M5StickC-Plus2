// snake.cpp - Snake game implementation
#include "snake.h"
#include "display.h"
#include "input.h"

SnakeGame snakeGame;

void SnakeGame::init() {
  snakeLength = 3;
  snake[0] = {GRID_WIDTH/2, GRID_HEIGHT/2};
  snake[1] = {GRID_WIDTH/2-1, GRID_HEIGHT/2};
  snake[2] = {GRID_WIDTH/2-2, GRID_HEIGHT/2};
  
  direction = 0; // right
  nextDirection = 0;
  directionChanged = false;
  score = 0;
  gameSpeed = 200;
  gameOver = false;
  lastMoveTime = 0;
  
  generateFood();
}

void SnakeGame::update() {
  handleInput();
  
  if (millis() - lastMoveTime > gameSpeed) {
    direction = nextDirection;
    directionChanged = false;
    
    moveSnake();
    if (checkCollisions()) {
      gameOver = true;
    }
    lastMoveTime = millis();
  }
}

void SnakeGame::draw() {
  clearDisplay();
  
  // Draw snake
  for (int i = 0; i < snakeLength; i++) {
    int x = snake[i].x * GRID_SIZE;
    int y = snake[i].y * GRID_SIZE;
    
    if (i == 0) {
      M5.Lcd.fillRect(x, y, GRID_SIZE, GRID_SIZE, COLOR_GREEN);
    } else {
      M5.Lcd.fillRect(x, y, GRID_SIZE-1, GRID_SIZE-1, COLOR_YELLOW);
    }
  }
  
  // Draw food
  int foodX = food.x * GRID_SIZE;
  int foodY = food.y * GRID_SIZE;
  M5.Lcd.fillCircle(foodX + GRID_SIZE/2, foodY + GRID_SIZE/2, GRID_SIZE/2-1, COLOR_RED);
  
  // Draw score
  M5.Lcd.setTextSize(1);
  M5.Lcd.setTextColor(COLOR_WHITE);
  M5.Lcd.setCursor(0, 0);
  M5.Lcd.print("Score:");
  M5.Lcd.print(score);
}

void SnakeGame::handleInput() {
  if (directionChanged) return;
  
  int newDirection = nextDirection;
  
  if (buttons.upPressed && newDirection != 1) {
    nextDirection = 3;
    directionChanged = true;
  }
  else if (buttons.downPressed && newDirection != 3) {
    nextDirection = 1;
    directionChanged = true;
  }
  else if (buttons.leftPressed && newDirection != 0) {
    nextDirection = 2;
    directionChanged = true;
  }
  else if (buttons.rightPressed && newDirection != 2) {
    nextDirection = 0;
    directionChanged = true;
  }
}

void SnakeGame::generateFood() {
  bool validPosition = false;
  
  while (!validPosition) {
    food.x = random(0, GRID_WIDTH);
    food.y = random(0, GRID_HEIGHT);
    
    validPosition = true;
    for (int i = 0; i < snakeLength; i++) {
      if (snake[i].x == food.x && snake[i].y == food.y) {
        validPosition = false;
        break;
      }
    }
  }
}

void SnakeGame::moveSnake() {
  Point newHead = snake[0];
  
  switch (direction) {
    case 0: newHead.x++; break; // right
    case 1: newHead.y++; break; // down
    case 2: newHead.x--; break; // left
    case 3: newHead.y--; break; // up
  }
  
  for (int i = snakeLength - 1; i > 0; i--) {
    snake[i] = snake[i-1];
  }
  
  snake[0] = newHead;
}

bool SnakeGame::checkCollisions() {
  Point head = snake[0];
  
  // Wall collision
  if (head.x < 0 || head.x >= GRID_WIDTH || 
      head.y < 0 || head.y >= GRID_HEIGHT) {
    return true;
  }
  
  // Self collision
  for (int i = 1; i < snakeLength; i++) {
    if (snake[i].x == head.x && snake[i].y == head.y) {
      return true;
    }
  }
  
  // Food collision
  if (head.x == food.x && head.y == food.y) {
    score++;
    snakeLength++;
    
    if (gameSpeed > 80) {
      gameSpeed -= 10;
    }
    
    generateFood();
  }
  
  return false;
}
