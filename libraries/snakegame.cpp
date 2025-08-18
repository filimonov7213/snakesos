#include <curses.h>
#include "snakegame.h"
#include "empty.h"

SnakeGame::SnakeGame(int height, int width) {
    board = Board(height, width);
    initialize();
}

SnakeGame::~SnakeGame() {
    delete apple;
}

void SnakeGame::handleNextPiece(SnakePiece next) {
    board.add(next);
    snake.addPiece(next);
}

void SnakeGame::createApple() {
    int y,x;
    board.getEmptyCoordinates(y,x); //fill up (y, x) with an empty coordinate in the window
    apple = new Apple(y,x);
    board.add(*apple);
}

void SnakeGame::destroyApple() {
    delete apple;
    apple = nullptr;
}

void SnakeGame::initialize() {
    apple = nullptr;
    board.initialize();
    game_over = false;
    srand(time(nullptr));

    //initialize snake
    snake.setDirection(down);
    handleNextPiece(SnakePiece(1,1));
    handleNextPiece(snake.nextHead());
    handleNextPiece(snake.nextHead());
    snake.setDirection(right);
    handleNextPiece(snake.nextHead());

    //initialize apple
    if (apple == nullptr)
        createApple();
}

void SnakeGame::processInput() {
    chtype input = board.getInput();

    switch (input) {
        case KEY_UP:
        case 'w':
            snake.setDirection(up);
            break;
        case KEY_DOWN:
        case 's':
            snake.setDirection(down);
            break;
        case KEY_LEFT:
        case 'a':
            snake.setDirection(left);
            break;
        case KEY_RIGHT:
        case 'd':
            snake.setDirection(right);
            break;
        // for the implementation of PAUSE
        case 'p':
            board.setTimeout(-1); // -1 is the value used for blocking input
            while (board.getInput() != 'p');
            board.setTimeout(500); // TODO: create global const variable for the speed of the game
            break;
        default:
            break;
    }
}

void SnakeGame::updateState() {
    SnakePiece next = snake.nextHead();

    if (apple != nullptr)
    {
        switch (board.getCharAt(next.getY(), next.getX()))
        {
            case 'A': {
                destroyApple();
                handleNextPiece(next);
                // return tail coords
                int emptyRow = snake.tail().getY();
                int emptyCol = snake.tail().getX();
                // add to tail coords an empty space
                board.add(Empty(emptyRow, emptyCol));
                snake.removePiece();
                break;
            }
            case ' ': {
                handleNextPiece(next);
                // return tail coords
                int emptyRow = snake.tail().getY();
                int emptyCol = snake.tail().getX();
                // add to tail coords an empty space
                board.add(Empty(emptyRow, emptyCol));
                snake.removePiece();
                break;
            }
            // manage the other cases: when the snake touch himself or when it touches the borders
            // TODO: FARE IN MODO CHE QUANDO SNAKE TOCCA IL BORDO SI TELETRASPORTI DALL'ALTRO LATO
            default:
                game_over = true;
                break;
        }
    }
    if (apple == nullptr) createApple();
}

void SnakeGame::redraw() {
    board.refresh();
}

bool SnakeGame::isOver() {
    return game_over;
}