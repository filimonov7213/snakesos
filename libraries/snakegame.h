#pragma once
#include "Board.h"
#include "Snake.h"
#include "Apple.h"

class SnakeGame {
public:
    SnakeGame(int height, int width, int timeLimit);
    ~SnakeGame();

    void initialize();
    void processInput();
    void updateState();
    void redraw();
    void setGameSpeed(int speed);
    void forceGameOver();
    bool isOver() const;

private:
    void createApple();
    void destroyApple();
    void updateSnakePosition();

    Board board;
    Snake snake;
    Apple* apple;

    int currentSpeed;   // ms per tick
    int timeLimit;      // in secondi
    int tickCount;      // numero di cicli passati
    int score;          // punteggio corrente
    bool game_over;
};
