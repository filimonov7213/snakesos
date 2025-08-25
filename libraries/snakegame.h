#pragma once
#include "Board.h"
#include "Snake.h"
#include "Apple.h"

class SnakeGame {
public:
    SnakeGame(int height, int width);
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
    void updateSnakePosition();

    Board board;
    Snake snake;
    Apple* apple;
    int currentSpeed;
    bool game_over;
};
