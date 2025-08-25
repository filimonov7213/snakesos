#pragma once
#include "Board.h"
#include "Snake.h"
#include "Apple.h"
#include "Scoreboard.h"
#include "livello.h"

class SnakeGame {
public:
    SnakeGame(int height, int width, Livello* current);
    ~SnakeGame();

    void initialize();
    void processInput();
    void updateState();
    void redraw();
    void setGameSpeed(int speed);
    bool isOver() const;

private:
    void createApple();
    void destroyApple();
    void updateSnakePosition(); // Nuova funzione helper

    int score;
    Scoreboard scoreboard;

    Board board;
    Snake snake;
    Apple* apple;
    int currentSpeed;
    Livello* currentLevel;
    bool game_over;

};