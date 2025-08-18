#pragma once
#include <stdlib.h>
#include <time.h>
#include "board.h"
#include "drawable.h"
#include "apple.h"
#include "empty.h"
#include "Snake.h"

class SnakeGame {
public:
    SnakeGame(int height, int width);

    ~SnakeGame();

    // get input from user and process it
    void processInput();

    // updates the current state of the game which allows to redraw()
    void updateState();

    //
    void redraw();

    // returns value of game_over
    bool isOver();

    // initialize the snake on the board
    void initialize();

private:
    Board board;
    bool game_over;
    Apple *apple;
    Snake snake;

    // if there is no apple on the board it adds an apple
    void createApple();

    // handle the pieces of the snake, check if the nextHead is not an apple,
    // add to tail coords EmptySpace to create
    // the illusion of movement
    void handleNextPiece(SnakePiece next);

    // Destroys the apple
    void destroyApple();
};
