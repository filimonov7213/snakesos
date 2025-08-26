#include "snakegame.h"
#include "Scoreboard.h"
#include "empty.h"
#include "livello.h"
#include <cstdlib>
#include <ctime>

SnakeGame::SnakeGame(int height, int width,Livello *current)
    : board(height, width), apple(nullptr), game_over(false), currentSpeed(500), currentLevel(current) {
  int sb_row =board.getStartRow() + height ;
  int sb_col= board.getStartCol();
  scoreboard = Scoreboard(width,sb_row,sb_col);
  initialize();
}

SnakeGame::~SnakeGame() {
    destroyApple();
}

void SnakeGame::initialize() {
    destroyApple();
    board.initialize();

    //inizializazza score
    score = 0;
    scoreboard.initialize(score);

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
        score += currentLevel->getId();  // Punteggio in base al livello
        scoreboard.updateScore(score);
        // Elimina la mela
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
            board.setTimeout(-1); // Input bloccante
        while (board.getInput() != 'p'); // Aspetta che premi 'p' again
        board.setTimeout(currentSpeed); // Torna alla velocità CORRETTA
        break;
        default: break;
    }
}

void SnakeGame::setGameSpeed(int speed) {
    currentSpeed = speed;
    board.setTimeout(speed);
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

    int maxY = board.getHeight() - 2; // Esclude bordo inferiore
    int maxX = board.getWidth() - 2;  // Esclude bordo destro
    int minY = 1; // Esclude bordo superiore
    int minX = 1; // Esclude bordo sinistro

    // Wrapping effetto Pac-Man
    if (headX < minX) {
        headX = maxX;
    } else if (headX > maxX) {
        headX = minX;
    }

    if (headY < minY) {
        headY = maxY;
    } else if (headY > maxY) {
        headY = minY;
    }

    // Aggiorna la testa con le nuove coordinate
    snake.setHeadPosition(headY, headX);

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

int SnakeGame::getScore() const {
  return(score);
  }

void SnakeGame::updateState() {
    if (game_over) return;

    updateSnakePosition();
}

void SnakeGame::redraw() {
    board.refresh();
    scoreboard.refresh();
}

bool SnakeGame::isOver() const {
    return game_over;
}