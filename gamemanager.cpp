// gamemanager.cpp - Game manager implementation
#include "gamemanager.h"
#include "display.h"
#include "input.h"

GameManager gameManager;

const char* gameNames[] = {
  "Snake",
  "Tetris",
  "Flappy",
  "Breakout",
  "SpaceWars",
  "Pong",
  "Dodge"
};

void initGameManager() {
  gameManager.init();
}

void updateGameManager() {
  gameManager.update();
}

void GameManager::init() {
  currentState = STATE_MENU;
  currentGame = 0;
  menuSelection = 0;
  showMenu();
}

void GameManager::update() {
  switch (currentState) {
    case STATE_MENU:
      handleMenuInput();
      break;
      
    case STATE_PLAYING:
      switch (currentGame) {
        case GAME_SNAKE:
          snakeGame.update();
          snakeGame.draw();
          if (snakeGame.isGameOver()) {
            setState(STATE_GAME_OVER);
          }
          break;
          
        case GAME_TETRIS:
          tetrisGame.update();
          tetrisGame.draw();
          if (tetrisGame.isGameOver()) {
            setState(STATE_GAME_OVER);
          }
          break;
          
        case GAME_FLAPPY:
          flappyGame.update();
          flappyGame.draw();
          if (flappyGame.isGameOver()) {
            setState(STATE_GAME_OVER);
          }
          break;
          
        case GAME_BREAKOUT:
          breakoutGame.update();
          breakoutGame.draw();
          if (breakoutGame.isGameOver()) {
            setState(STATE_GAME_OVER);
          }
          break;
          
        case GAME_SPACEWARS:
          spacewarsGame.update();
          spacewarsGame.draw();
          if (spacewarsGame.isGameOver()) {
            setState(STATE_GAME_OVER);
          }
          break;
          
        case GAME_PONG:
          pongGame.update();
          pongGame.draw();
          if (pongGame.isGameOver()) {
            setState(STATE_GAME_OVER);
          }
          break;
          
        case GAME_DODGE:
          dodgeGame.update();
          dodgeGame.draw();
          if (dodgeGame.isGameOver()) {
            setState(STATE_GAME_OVER);
          }
          break;
      }
      
      // Press B to return to menu
      if (buttons.btnBPressed) {
        setState(STATE_MENU);
      }
      break;
      
    case STATE_GAME_OVER:
      showGameOver();
      if (buttons.btnAPressed) {
        setState(STATE_MENU);
      }
      break;
      
    case STATE_GAME_WON:
      // Similar to game over
      showGameOver();
      if (buttons.btnAPressed) {
        setState(STATE_MENU);
      }
      break;
  }
}

void GameManager::showMenu() {
  clearDisplay();
  
  // Draw title on right side
  M5.Lcd.setTextSize(2);
  M5.Lcd.setTextColor(COLOR_CYAN);
  M5.Lcd.setCursor(85, 10);
  M5.Lcd.println("M5STICK");
  M5.Lcd.setCursor(85, 28);
  M5.Lcd.println("GAMES");
  
  int y = 10;
  for (int i = 0; i < 7; i++) { // Show 7 games
    if (i == menuSelection) {
      M5.Lcd.fillRect(5, y - 2, 75, 17, COLOR_BLUE);
      M5.Lcd.setTextSize(2);
      M5.Lcd.setTextColor(COLOR_WHITE);
      M5.Lcd.setCursor(8, y);
      M5.Lcd.println(gameNames[i]);
    } else {
      M5.Lcd.setTextSize(2);
      M5.Lcd.setTextColor(COLOR_WHITE);
      M5.Lcd.setCursor(8, y);
      M5.Lcd.println(gameNames[i]);
    }
    y += 17;
  }
}

void GameManager::handleMenuInput() {
  static unsigned long lastInput = 0;
  if (millis() - lastInput < 200) return;
  
  if (buttons.upPressed) {
    menuSelection--;
    if (menuSelection < 0) menuSelection = 6; // Now 7 games
    showMenu();
    lastInput = millis();
  }
  else if (buttons.downPressed) {
    menuSelection++;
    if (menuSelection > 6) menuSelection = 0; // Now 7 games
    showMenu();
    lastInput = millis();
  }
  else if (buttons.btnAPressed) {
    startGame(menuSelection);
    lastInput = millis();
  }
}

void GameManager::startGame(int gameId) {
  currentGame = gameId;
  
  switch (gameId) {
    case GAME_SNAKE:
      snakeGame.init();
      break;
    case GAME_TETRIS:
      tetrisGame.init();
      break;
    case GAME_FLAPPY:
      flappyGame.init();
      break;
    case GAME_BREAKOUT:
      breakoutGame.init();
      break;
    case GAME_SPACEWARS:
      spacewarsGame.init();
      break;
    case GAME_PONG:
      pongGame.init();
      break;
    case GAME_DODGE:
      dodgeGame.init();
      break;
  }
  
  setState(STATE_PLAYING);
}

void GameManager::showGameOver() {
  clearDisplay();
  
  drawCenteredText("GAME OVER", 40, 3, COLOR_RED);
  
  char scoreText[30];
  int finalScore = 0;
  
  switch (currentGame) {
    case GAME_SNAKE:
      finalScore = snakeGame.getScore();
      break;
    case GAME_TETRIS:
      finalScore = tetrisGame.getScore();
      break;
    case GAME_FLAPPY:
      finalScore = flappyGame.getScore();
      break;
    case GAME_BREAKOUT:
      finalScore = breakoutGame.getScore();
      break;
    case GAME_SPACEWARS:
      finalScore = spacewarsGame.getScore();
      break;
    case GAME_PONG:
      finalScore = pongGame.getScore();
      break;
    case GAME_DODGE:
      finalScore = dodgeGame.getScore();
      break;
  }
  
  sprintf(scoreText, "Score: %d", finalScore);
  drawCenteredText(scoreText, 75, 2, COLOR_YELLOW);
  drawCenteredText("Press A for Menu", 110, 1, COLOR_WHITE);
}

void GameManager::setState(GameState state) {
  currentState = state;
  
  if (state == STATE_MENU) {
    M5.Lcd.setRotation(1); // Reset to landscape for menu
    showMenu();
  }
  
  if (state == STATE_GAME_OVER || state == STATE_GAME_WON) {
    M5.Lcd.setRotation(1); // Reset to landscape for game over screen
  }
}
