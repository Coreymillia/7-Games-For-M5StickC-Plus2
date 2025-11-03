// pong.cpp - Ping Pong implementation
#include "pong.h"
#include "input.h"

PongGame pongGame;

void PongGame::init() {
  // Landscape mode for Pong
  M5.Lcd.setRotation(1);
  M5.Lcd.fillScreen(COLOR_BLACK);
  
  // Initialize ball
  ball.x = SCREEN_WIDTH / 2;
  ball.y = SCREEN_HEIGHT / 2;
  ball.dx = 3;
  ball.dy = 3;
  ball.speed = 3;
  ball.radius = 3;
  ball.prevX = ball.x;
  ball.prevY = ball.y;
  
  // Initialize player paddle (left)
  player.x = 5;
  player.y = SCREEN_HEIGHT / 2 - 20;
  player.width = 5;
  player.height = 40;
  player.dy = 3;
  player.prevY = player.y;
  
  // Initialize enemy paddle (right)
  enemy.x = SCREEN_WIDTH - 10;
  enemy.y = SCREEN_HEIGHT / 2 - 20;
  enemy.width = 5;
  enemy.height = 40;
  enemy.dy = 2;
  enemy.prevY = enemy.y;
  
  playerScore = 0;
  enemyScore = 0;
  gameOver = false;
  aiTargetY = SCREEN_HEIGHT / 2;
  
  // Draw center line
  for (int i = 0; i < SCREEN_HEIGHT; i += 10) {
    M5.Lcd.drawLine(SCREEN_WIDTH / 2, i, SCREEN_WIDTH / 2, i + 5, COLOR_WHITE);
  }
}

void PongGame::resetBall(int direction) {
  ball.x = SCREEN_WIDTH / 2;
  ball.y = SCREEN_HEIGHT / 2;
  ball.speed = 3;
  ball.dy = (random(2) > 0 ? 1 : -1) * ball.speed;
  ball.dx = direction * ball.speed;
}

void PongGame::movePaddle(Paddle* paddle, int y) {
  // Clear old position
  M5.Lcd.fillRect(paddle->x, paddle->prevY, paddle->width, paddle->height, COLOR_BLACK);
  
  paddle->prevY = paddle->y;
  paddle->y = y;
  
  // Clamp to screen
  if (paddle->y < 0) paddle->y = 0;
  if (paddle->y + paddle->height > SCREEN_HEIGHT) 
    paddle->y = SCREEN_HEIGHT - paddle->height;
  
  // Draw new position
  M5.Lcd.fillRect(paddle->x, paddle->y, paddle->width, paddle->height, COLOR_WHITE);
}

void PongGame::updateAI() {
  // AI follows ball when it's coming towards it
  if (ball.dx > 0) {
    aiTargetY = ball.y + (random(60) - 30);
  }
  
  int enemyCenter = enemy.y + enemy.height / 2;
  
  if (enemyCenter < aiTargetY - 5) {
    movePaddle(&enemy, enemy.y + enemy.dy);
  } else if (enemyCenter > aiTargetY + 5) {
    movePaddle(&enemy, enemy.y - enemy.dy);
  }
}

void PongGame::checkCollisions() {
  // Ball hits top/bottom
  if (ball.y + ball.radius > SCREEN_HEIGHT || ball.y - ball.radius < 0) {
    ball.dy *= -1;
  }
  
  // Ball hits player paddle
  if (ball.x - ball.radius < player.x + player.width &&
      ball.x + ball.radius > player.x &&
      ball.y - ball.radius < player.y + player.height &&
      ball.y + ball.radius > player.y) {
    ball.dx = abs(ball.dx);  // Always bounce right
    ball.speed += 0.5;
    
    // Angle based on where it hits paddle
    float collidePoint = ball.y - (player.y + player.height / 2.0);
    collidePoint = collidePoint / (player.height / 2.0);
    float angleRad = (PI / 4) * collidePoint;
    ball.dy = ball.speed * sin(angleRad);
    aiTargetY = ball.y + (random(60) - 30);
  }
  
  // Ball hits enemy paddle
  if (ball.x + ball.radius > enemy.x &&
      ball.x - ball.radius < enemy.x + enemy.width &&
      ball.y - ball.radius < enemy.y + enemy.height &&
      ball.y + ball.radius > enemy.y) {
    ball.dx = -abs(ball.dx);  // Always bounce left
    ball.speed += 0.5;
    
    float collidePoint = ball.y - (enemy.y + enemy.height / 2.0);
    collidePoint = collidePoint / (enemy.height / 2.0);
    float angleRad = (PI / 4) * collidePoint;
    ball.dy = ball.speed * sin(angleRad);
    aiTargetY = ball.y + (random(60) - 30);
  }
  
  // Ball goes off right side (player scores)
  if (ball.x + ball.radius > SCREEN_WIDTH) {
    playerScore++;
    delay(500);
    resetBall(-1);
  }
  
  // Ball goes off left side (enemy scores)
  if (ball.x - ball.radius < 0) {
    enemyScore++;
    delay(500);
    resetBall(1);
  }
  
  // Game over at 10 points
  if (playerScore >= 10 || enemyScore >= 10) {
    gameOver = true;
  }
}

void PongGame::update() {
  if (gameOver) return;
  
  handleInput();
  updateAI();
  
  // Move ball
  ball.prevX = ball.x;
  ball.prevY = ball.y;
  ball.x += ball.dx;
  ball.y += ball.dy;
  
  checkCollisions();
}

void PongGame::draw() {
  // Clear old ball
  M5.Lcd.fillCircle(ball.prevX, ball.prevY, ball.radius, COLOR_BLACK);
  
  // Draw new ball
  M5.Lcd.fillCircle(ball.x, ball.y, ball.radius, COLOR_WHITE);
  
  // Draw scores
  drawScores();
}

void PongGame::drawScores() {
  M5.Lcd.setTextSize(2);
  M5.Lcd.setTextColor(COLOR_WHITE, COLOR_BLACK);
  M5.Lcd.setCursor(SCREEN_WIDTH / 4 - 10, 10);
  M5.Lcd.print(playerScore);
  M5.Lcd.setCursor(3 * SCREEN_WIDTH / 4 - 10, 10);
  M5.Lcd.print(enemyScore);
}

void PongGame::handleInput() {
  // Use joystick for paddle control
  // Physical UP = paddle moves up
  // Physical DOWN = paddle moves down
  
  if (buttons.up) {
    movePaddle(&player, player.y - player.dy);
  }
  if (buttons.down) {
    movePaddle(&player, player.y + player.dy);
  }
}
