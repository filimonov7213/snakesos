#pragma once
#include "Board.h"
#include "Snake.h"
#include "Apple.h"
#include <ctime>

class SnakeGame {
public:
    SnakeGame(int height, int width, int timeLimit, int appleFactor);
    ~SnakeGame();

    void initialize();
    void processInput();
    void updateState();
    void redraw();
    void setGameSpeed(int speed);
    void forceGameOver();
    bool isOver() const;
    int getScore() const;
    void setScore(int newScore);

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
    int appleFactor;    // moltiplicatore punti per mela
    int score;          // punteggio corrente
    bool game_over;
    time_t startTime;   // tempo dallo start

};
