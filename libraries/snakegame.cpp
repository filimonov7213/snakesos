#include "snakegame.h"
#include "empty.h"
#include <cstdlib>

SnakeGame::SnakeGame(int height, int width, int timeLimit, int appleFactor)
    : board(height, width), apple(nullptr), game_over(false),
      currentSpeed(200), timeLimit(timeLimit), appleFactor(appleFactor), score(0) {
    startTime = time(nullptr);
    initialize();
}

SnakeGame::~SnakeGame() {
    destroyApple();
}

void SnakeGame::initialize() {
    destroyApple();
    board.initialize();
    game_over = false;
    tickCount = 0;
    score = 0;
    hud = newwin(3, 30, 0, board.getWidth() + 2); // 3 righe, 30 colonne a destra del campo
//    appleFactor = 10;

    // inizializza snake al centro
    snake.initialize(board.getHeight() / 2, board.getWidth() / 2,
                 board.getHeight(), board.getWidth());

    // disegna snake iniziale
    board.addAt(snake.getHeadY(), snake.getHeadX(), 'O');
    board.drawBorder();
    for (int i = 0; i < Snake::FIXED_LENGTH - 1; i++) {
        board.addAt(snake.getBodyY(i), snake.getBodyX(i), 'o');
    }

    createApple();
}

void SnakeGame::createApple() {
    int y, x;
    do {
        board.getEmptyCoordinates(y, x);
    } while (snake.isAt(y, x));
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
        case KEY_UP:    case 'w': snake.setDirection(up); break;
        case KEY_DOWN:  case 's': snake.setDirection(down); break;
        case KEY_LEFT:  case 'a': snake.setDirection(left); break;
        case KEY_RIGHT: case 'd': snake.setDirection(right); break;
        case 'p': // pausa
            board.setTimeout(-1);
            while (board.getInput() != 'p');
            board.setTimeout(currentSpeed);
            break;
        default: break;
    }
}

void SnakeGame::updateSnakePosition() {
    int tailY = snake.getBodyY(Snake::FIXED_LENGTH - 1);
    int tailX = snake.getBodyX(Snake::FIXED_LENGTH - 1);

    snake.move();

    int headY = snake.getHeadY();
    int headX = snake.getHeadX();

    chtype nextChar = board.getCharAt(headY, headX);

    if (nextChar == 'A') {
        destroyApple();
        createApple();
        score += appleFactor;
    } else if (snake.isAt(headY, headX)) {
        // Collisione con se stesso
        game_over = true;
        return;
    }

    board.addAt(tailY, tailX, ' ');
    for (int i = Snake::FIXED_LENGTH - 2; i > 0; i--) {
        board.addAt(snake.getBodyY(i), snake.getBodyX(i), 'o');
    }
    board.addAt(snake.getBodyY(0), snake.getBodyX(0), 'o');
    board.addAt(headY, headX, 'O');
}

void SnakeGame::updateState() {
    if (game_over) return;

    tickCount++;

    // tempo in secondi
    //int elapsed = (tickCount * currentSpeed) / 1000;
    int elapsed = static_cast<int>(time(nullptr) - startTime);
    int remaining = timeLimit - elapsed;

    if (remaining <= 0) {
        game_over = true;
        return;
    }

    updateSnakePosition();
}


void SnakeGame::redraw() {
    board.drawBorder();
    board.refresh();
    werase(hud);
    box(hud, 0, 0);

    int elapsed = static_cast<int>(time(nullptr) - startTime);
    int remaining = timeLimit - elapsed;

    // stampa punteggio e tempo rimanente in alto
    mvwprintw(hud, 1, 1, "Score: %d", score);
    mvwprintw(hud, 2, 1, "Time left: %02d", remaining);
    wrefresh(hud);
}

void SnakeGame::setGameSpeed(int speed) {
    currentSpeed = speed;
    board.setTimeout(speed);
}

bool SnakeGame::isOver() const {
    return game_over;
}

void SnakeGame::setScore(int newScore) {
    score = newScore;
}

int SnakeGame::getScore() const {
    return score;
}

void SnakeGame::forceGameOver() {
    game_over = true;
}
