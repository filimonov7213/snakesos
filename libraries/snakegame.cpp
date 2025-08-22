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
    if (game_over) {
        return; // ⬅️ nessun input processato dopo la morte
    }

    chtype input = board.getInput();
    if (input == ERR) return; // timeout scaduto, nessun input

    switch (input) {
        case KEY_UP:
        case 'w': snake.setDirection(up); break;
        case KEY_DOWN:
        case 's': snake.setDirection(down); break;
        case KEY_LEFT:
        case 'a': snake.setDirection(left); break;
        case KEY_RIGHT:
        case 'd': snake.setDirection(right); break;

        case 'p': // pausa
            board.setTimeout(-1);
        while (board.getInput() != 'p');
        board.setTimeout(500);
        break;
    }
}

void SnakeGame::updateState() {
    if (game_over) return;  // ⬅ se già finito, non aggiornare

    SnakePiece next = snake.nextHead();
    int ny = next.getY();
    int nx = next.getX();

    // ontrollo bounds “duro” per evitare letture fuori finestra
    if (ny <= 0 || ny >= board.getHeight()-1 || nx <= 0 || nx >= board.getWidth()-1) {
        game_over = true;
        return;
    }

    // Maschera gli attributi del carattere
    chtype cell = board.getCharAt(ny, nx) & A_CHARTEXT;

    switch (cell) {
        case 'A': {
            destroyApple();
            handleNextPiece(next);
            int emptyRow = snake.tail().getY();
            int emptyCol = snake.tail().getX();
            board.add(Empty(emptyRow, emptyCol));
            snake.removePiece();
            break;
        }
        case ' ': {
            handleNextPiece(next);
            int emptyRow = snake.tail().getY();
            int emptyCol = snake.tail().getX();
            board.add(Empty(emptyRow, emptyCol));
            snake.removePiece();
            break;
        }
        default:
            game_over = true;   // ⬅ muore su bordo o su sé stesso
        return;             // ⬅ interrompi subito l’update
    }

    if (apple == nullptr) createApple();
}


void SnakeGame::redraw() {
    board.refresh();
}

bool SnakeGame::isOver() {
    return game_over;
}