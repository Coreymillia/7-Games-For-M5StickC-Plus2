// gamemanager.h - Main game manager
#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include "config.h"
#include "snake.h"
#include "tetris.h"
#include "flappy.h"
#include "breakout.h"
#include "spacewars.h"
#include "pong.h"
#include "dodge.h"

class GameManager {
private:
  GameState currentState;
  int currentGame;
  int menuSelection;
  
  void showMenu();
  void handleMenuInput();
  void startGame(int gameId);
  void showGameOver();
  
public:
  void init();
  void update();
  void setState(GameState state);
};

void initGameManager();
void updateGameManager();

extern GameManager gameManager;

#endif
