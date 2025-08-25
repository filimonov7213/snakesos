#include "snakegame.h"
#include "empty.h"
#include <cstdlib>
#include <ctime>

SnakeGame::SnakeGame(int height, int width)
    : board(height, width), apple(nullptr), game_over(false), currentSpeed(200) {
    // imposta dimensioni campo alla snake (per la matrice booleana)
    snake.setFieldSize(height, width);
    initialize();
}

SnakeGame::~SnakeGame() {
    destroyApple();
}

void SnakeGame::initialize() {
    destroyApple();
    board.initialize();
    game_over = false;

    srand((unsigned)time(nullptr));

    // centra lo snake
    int startY = 1 + (board.getHeight() - 2) / 2; // dentro il bordo
    int startX = 1 + (board.getWidth()  - 2) / 2;
    snake.initialize(startY, startX);

    // disegna snake iniziale
    for (int i = Snake::FIXED_LENGTH - 1; i >= 1; --i) {
        board.addAt(snake.getBodyY(i), snake.getBodyX(i), 'o');
    }
    board.addAt(snake.getHeadY(), snake.getHeadX(), 'O');

    // prima mela
    createApple();

    // applica velocità corrente come timeout di input (game tick)
    board.setTimeout(currentSpeed);
}

void SnakeGame::createApple() {
    int y, x;
    do {
        board.getEmptyCoordinates(y, x);
    } while (snake.isAt(y, x)); // non sulla snake

    apple = new Apple(y, x);
    board.add(*apple);
}

void SnakeGame::destroyApple() {
    if (apple) { delete apple; apple = nullptr; }
}

void SnakeGame::processInput() {
    chtype input = board.getInput(); // blocca fino a timeout -> gestisce la velocità

    switch (input) {
        case KEY_UP:
        case 'w': snake.setDirection(up);    break;
        case KEY_DOWN:
        case 's': snake.setDirection(down);  break;
        case KEY_LEFT:
        case 'a': snake.setDirection(left);  break;
        case KEY_RIGHT:
        case 'd': snake.setDirection(right); break;

        case 'p': // pausa
            board.setTimeout(-1);
            while (board.getInput() != 'p');
            board.setTimeout(currentSpeed);
            break;
        default:
            // timeout o tasto irrilevante: nessun cambio direzione
            break;
    }
}

void SnakeGame::updateState() {
    if (game_over) return;

    // calcola prossima cella della testa
    int ny = snake.getHeadY();
    int nx = snake.getHeadX();
    switch (snake.getDirection()) {
        case up:    --ny; break;
        case down:  ++ny; break;
        case left:  --nx; break;
        case right: ++nx; break;
    }

    // salva coda attuale (serve per permettere "entrare nella coda")
    int tailY = snake.getTailY();
    int tailX = snake.getTailX();

    // leggi cosa c'è nella prossima cella
    chtype nextC = board.getCharAt(ny, nx);

    // collisione con bordo o corpo (CONSENTI se è la cella della coda)
    if (nextC != ' ' && nextC != 'A') {
        if (!(ny == tailY && nx == tailX)) { // non è la coda
            game_over = true;
            return;
        }
    }

    // se mela, la mangi (non allunghiamo: lunghezza fissa) -> ricrea più tardi
    bool ateApple = (nextC == 'A');

    // pulisci visivamente la coda PRIMA di muovere
    board.addAt(tailY, tailX, ' ');

    // muovi lo snake: aggiorna FIFO e matrice booleana (rimuove coda, aggiunge testa)
    snake.move();

    // ridisegna corpo e testa
    for (int i = Snake::FIXED_LENGTH - 1; i >= 1; --i) {
        board.addAt(snake.getBodyY(i), snake.getBodyX(i), 'o');
    }
    board.addAt(snake.getHeadY(), snake.getHeadX(), 'O');

    // ricrea la mela se mangiata
    if (ateApple) {
        destroyApple();
        createApple();
    }
}

void SnakeGame::redraw() {
    board.refresh();
}

void SnakeGame::setGameSpeed(int speed) {
    currentSpeed = speed;      // ms
    board.setTimeout(currentSpeed);
}

bool SnakeGame::isOver() const {
    return game_over;
}
