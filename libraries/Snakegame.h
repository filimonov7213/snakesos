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
    void redraw(int totalScore);
    void setGameSpeed(int speed);
    void forceGameOver();
    bool isOver() const;
    int getScore() const;
    void setScore(int newScore);

private:
    void createApple();
    void destroyApple();
    void showPauseMenu();
    void updateSnakePosition();
    void drawGreenBorder();

    Board board;
    Snake snake;
    Apple* apple;
    WINDOW* hud; // hud

    int currentSpeed;   // ms per tick
    int timeLimit;      // in secondi
    int tickCount;      // numero di cicli passati
    int appleFactor;    // moltiplicatore punti per mela
    int score;          // punteggio corrente
    bool game_over;
    int borderFlashCount;  // Contatore per l'effetto flash
    int pausedTime;       // tempo totale passato in pausa
    time_t pauseStart;    // quando la pausa è iniziata
    bool paused;
    bool borderGreen;      // Stato del bordo verde
    time_t startTime;   // tempo dallo start

};
