#include "snakegame.h"
#include "empty.h"
#include <cstdlib>
#include <string>
#include <cstring>

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
        case 'P':
            showPauseMenu();
            break;
        default: break;
    }
}

void SnakeGame::showPauseMenu() {
    // Salva lo stato corrente del timeout
    board.setTimeout(-1);
    flushinp();

    // 👇 CREA UN OVERLAY SCURO SU TUTTO LO SCHERMO
    WINDOW* overlay_win = newwin(board.getHeight(), board.getWidth(), 0, 0);
    for (int y = 0; y < board.getHeight(); y++) {
        for (int x = 0; x < board.getWidth(); x++) {
            mvwprintw(overlay_win, y, x, " ");
        }
    }
    wbkgd(overlay_win, A_REVERSE); // 👈 Effetto scuro
    wrefresh(overlay_win);

    // 👇 CREA IL MENU DI PAUSA CENTRATO
    int menu_height = 5;
    int menu_width = 20;
    int start_y = (board.getHeight() - menu_height) / 2;
    int start_x = (board.getWidth() - menu_width) / 2;

    WINDOW* pause_win = newwin(menu_height, menu_width, start_y, start_x);
    keypad(pause_win, TRUE);

    // 👇 SFONDO DEL MENU
    wbkgd(pause_win, A_NORMAL);
    wattron(pause_win, A_BOLD);
    box(pause_win, 0, 0);
    wattroff(pause_win, A_BOLD);

    // Titolo centrato
    const char* title = "PAUSA";
    mvwprintw(pause_win, 1, (menu_width - strlen(title)) / 2, "%s", title);

    // Opzioni del menu
    const char* options[2] = {"Riprendi", "Menu principale"};
    int highlight = 0;

    while (true) {
        // Disegna le opzioni centrate
        for (int i = 0; i < 2; i++) {
            int x_pos = (menu_width - strlen(options[i])) / 2;
            if (i == highlight) {
                wattron(pause_win, A_REVERSE);
            }
            mvwprintw(pause_win, i + 2, x_pos, "%s", options[i]);
            wattroff(pause_win, A_REVERSE);
        }
        wrefresh(pause_win);

        // Input con W/S
        int ch = wgetch(pause_win);
        switch (ch) {
            case 'w': case 'W': case KEY_UP:
                highlight = (highlight - 1 + 2) % 2;
                break;
            case 's': case 'S': case KEY_DOWN:
                highlight = (highlight + 1) % 2;
                break;
            case 10: case ' ': // ENTER o SPACE
                delwin(pause_win);
                delwin(overlay_win);
                touchwin(stdscr);
                refresh();
                flushinp();
                board.setTimeout(currentSpeed);
                if (highlight == 1) forceGameOver();
                return;
            case 'p': case 'P': case 27: // P o ESC per riprendere
                delwin(pause_win);
                delwin(overlay_win);
                touchwin(stdscr);
                refresh();
                flushinp();
                board.setTimeout(currentSpeed);
                return;
        }
    }
}

void SnakeGame::updateSnakePosition() {
    // Salva la posizione della coda prima di muovere
    int oldTailY = snake.getTailY();
    int oldTailX = snake.getTailX();

    // move() ritorna false se c'è collisione col corpo
    if (!snake.move()) {
        game_over = true;
        return;
    }

    int headY = snake.getHeadY();
    int headX = snake.getHeadX();

    // Controllo mela
    bool ateApple = false;
    if (apple && headY == apple->getY() && headX == apple->getX()) {
        destroyApple();
        createApple();
        score += appleFactor;
        ateApple = true;
    }

    // Pulisci solo la vecchia coda se NON ha mangiato la mela
    if (!ateApple) {
        board.addAt(oldTailY, oldTailX, ' ');
    }

    // Prima pulisci tutte le posizioni del serpente
    for (int y = 0; y < board.getHeight(); y++) {
        for (int x = 0; x < board.getWidth(); x++) {
            chtype ch = board.getCharAt(y, x);
            if (ch == 'o' || ch == 'O') {
                board.addAt(y, x, ' ');
            }
        }
    }

    // Ora ridisegna tutto il serpente
    for (int i = 0; i < Snake::FIXED_LENGTH; i++) {
        int bodyY = snake.getBodyY(i);
        int bodyX = snake.getBodyX(i);

        // Controlla che le coordinate siano valide
        if (bodyY >= 0 && bodyY < board.getHeight() &&
            bodyX >= 0 && bodyX < board.getWidth()) {

            if (i == 0) {
                // Testa
                board.addAt(bodyY, bodyX, 'O');
            } else {
                // Corpo
                board.addAt(bodyY, bodyX, 'o');
            }
        }
    }
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
