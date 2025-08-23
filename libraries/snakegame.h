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
    bool isOver() const;

private:
    void createApple();
    void destroyApple();
    void updateSnakePosition(); // Nuova funzione helper

    Board board;
    Snake snake;
    Apple* apple;
    bool game_over;
};