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
    //score = 0;

    // Ottieni le dimensioni del terminale
    int terminalHeight, terminalWidth;
    getmaxyx(stdscr, terminalHeight, terminalWidth);

    // Posiziona HUD a destra del terminale con margine
    int hudWidth = 25;
    int hudHeight = 5;
    int hudX = terminalWidth - hudWidth - 2; // 2 caratteri di margine dal bordo destro
    int hudY = 1; // Inizia dalla riga 1

    hud = newwin(hudHeight, hudWidth, hudY, hudX);

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
            if (!isPaused) {
                // Entra in pausa
                isPaused = true;
                board.setTimeout(-1);
                redraw(); // Ridisegna per mostrare "PAUSA"

                // Aspetta che venga premuto di nuovo 'p' per uscire dalla pausa
                while (board.getInput() != 'p');

                // Esce dalla pausa
                isPaused = false;
                board.setTimeout(currentSpeed);
                redraw(); // Ridisegna per togliere "PAUSA"
            }
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
    if (game_over || isPaused) return; // Non aggiornare se in pausa

    tickCount++;

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

    int terminalHeight, terminalWidth;
    getmaxyx(stdscr, terminalHeight, terminalWidth);
    // Posiziona la scritta sotto l'HUD
    int hudWidth = 25;
    int hudHeight = 5;
    int hudX = terminalWidth - hudWidth - 2;
    int hudY = 1;

    // Posizione per la scritta PAUSA (sotto l'HUD)
    int pauseY = hudY + hudHeight + 1;
    int pauseX = hudX + (hudWidth - 5) / 2; // Centra la scritta "PAUSA"

    // stampa punteggio e tempo rimanente in alto
    mvwprintw(hud, 1, 1, "Score: %d", score);
    mvwprintw(hud, 2, 1, "Time left: %02d", remaining);
    mvwprintw(hud, 3, 1, "Speed: %dms", currentSpeed);
    wrefresh(hud);

    // Se è in pausa, mostra la scritta "PAUSA" sotto l'HUD
    if (isPaused) {
        int terminalHeight, terminalWidth;
        getmaxyx(stdscr, terminalHeight, terminalWidth);

        // Stampa la scritta PAUSA
        attron(A_BOLD | A_BLINK); // Grassetto e lampeggiante per enfasi
        mvprintw(pauseY, pauseX, "PAUSA");
        attroff(A_BOLD | A_BLINK);
        refresh();
    } else {
        // Se non è in pausa, cancella la scritta PAUSA (se c'era)
        int terminalHeight, terminalWidth;
        getmaxyx(stdscr, terminalHeight, terminalWidth);

        // Cancella la scritta PAUSA con spazi
        mvprintw(pauseY, pauseX, "     "); // 5 spazi per "PAUSA"
        refresh();
    }
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
