#include "snakegame.h"
#include "empty.h"
#include <cstdlib>
#include <ctime>

SnakeGame::SnakeGame(int height, int width)
    : board(height, width), apple(nullptr), game_over(false) {
    initialize();
}

SnakeGame::~SnakeGame() {
    destroyApple();
}

void SnakeGame::initialize() {
    destroyApple();
    board.initialize();
    game_over = false;

    srand(time(nullptr));

    // Inizializza snake
    snake.initialize(board.getHeight() / 2, board.getWidth() / 2);

    // Disegna tutto lo snake iniziale
    board.addAt(snake.getHeadY(), snake.getHeadX(), 'O'); // Testa
    for (int i = 0; i < Snake::FIXED_LENGTH - 1; i++) {
        board.addAt(snake.getBodyY(i), snake.getBodyX(i), 'o'); // Corpo
    }

    // Prima mela
    createApple();
}

void SnakeGame::createApple() {
    int y, x;
    do {
        board.getEmptyCoordinates(y, x);
    } while (snake.isAt(y, x)); // ass. che la mela non sia sullo snake

    apple = new Apple(y, x);
    board.add(*apple);
}

void SnakeGame::destroyApple() {
    if (apple != nullptr) {
        delete apple;
        apple = nullptr;
    }
}

void SnakeGame::processInput() {
    chtype input = board.getInput();

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
            board.setTimeout(200);
            break;
        default: break;
    }
}

void SnakeGame::updateSnakePosition() {
    // Salva la posizione della coda (ultimo segmento) per cancellarla dopo
    int tailY = snake.getBodyY(Snake::FIXED_LENGTH - 2);
    int tailX = snake.getBodyX(Snake::FIXED_LENGTH - 2);

    // Muovi lo snake
    snake.move();

    // Ottieni le nuove posizioni
    int headY = snake.getHeadY();
    int headX = snake.getHeadX();

    // Controlla collisioni
    chtype nextChar = board.getCharAt(headY, headX);

    if (nextChar == 'A') { // Mela
        destroyApple();
        createApple();
        // Non cresce, quindi non cambia la lunghezza
    } else if (nextChar != ' ') { // Collisione con muro o sé stesso
        game_over = true;
        return;
    }

    // Aggiorna la board
    board.addAt(tailY, tailX, ' '); // Cancella la coda vecchia

    // Aggiorna tutto il corpo
    for (int i = Snake::FIXED_LENGTH - 2; i > 0; i--) {
        board.addAt(snake.getBodyY(i), snake.getBodyX(i), 'o');
    }
    board.addAt(snake.getBodyY(0), snake.getBodyX(0), 'o'); // Primo segmento del corpo
    board.addAt(headY, headX, 'O'); // Nuova testa
}

void SnakeGame::updateState() {
    if (game_over) return;

    updateSnakePosition();
}

void SnakeGame::redraw() {
    board.refresh();
}

bool SnakeGame::isOver() const {
    return game_over;
}