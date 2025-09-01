#include "snakegame.h"
#include "empty.h"
#include <cstdlib>
#include <string>
#include <cstring>

SnakeGame::SnakeGame(int height, int width, int timeLimit, int appleFactor)
    : board(height, width), apple(nullptr), game_over(false),
      currentSpeed(200), timeLimit(timeLimit), appleFactor(appleFactor),
      score(0), borderFlashCount(0), borderGreen(false) {

    startTime = time(nullptr);

    // INIZIALIZZA COLORI SE SUPPORTATI
    if (has_colors()) {
        init_pair(1, COLOR_WHITE, COLOR_BLACK); // Normale
        init_pair(2, COLOR_GREEN, COLOR_BLACK); // Verde
    }

    initialize();
}

SnakeGame::~SnakeGame() {
    destroyApple();
}

void SnakeGame::initialize() {
    destroyApple();

    // 👇 INIZIALIZZA BOARD CON CONTROLLO
    board.initialize();

    game_over = false;
    tickCount = 0;

    // CREA HUD CON CONTROLLO
    hud = newwin(4, 30, 0, board.getWidth() + 2);
    if (!hud) {
        game_over = true;
        return;
    }

    // INIZIALIZZA SNAKE CON CONTROLLO
    snake.initialize(board.getHeight() / 2, board.getWidth() / 2,
                    board.getHeight(), board.getWidth());

    // DISEGNA SNAKE INIZIALE
    board.addAt(snake.getHeadY(), snake.getHeadX(), 'O');
    board.drawBorder();
    for (int i = 0; i < Snake::FIXED_LENGTH - 1; i++) {
        board.addAt(snake.getBodyY(i), snake.getBodyX(i), 'o');
    }

    createApple();
}

void SnakeGame::createApple() {
    int y, x;
    int attempts = 0;
    const int max_attempts = 100;

    // LIMITA TENTATIVI PER EVITARE LOOP INFINITI
    do {
        board.getEmptyCoordinates(y, x);
        attempts++;
        if (attempts > max_attempts) {
            // Fallback: usa posizione fissa
            y = board.getHeight() / 3;
            x = board.getWidth() / 3;
            break;
        }
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

    int headY = snake.getHeadY();
    int headX = snake.getHeadX();

    bool is_head_on_border = (headY == 0 || headY == board.getHeight() - 1 ||
                             headX == 0 || headX == board.getWidth() - 1);

    if (is_head_on_border) {
        return; // BLOCCA TUTTI GLI INPUT QUANDO SUL BORDO
    }

    chtype input = board.getInput();

    // DISABILITA L'ECHO AUTOMATICO DEI TASTI
    noecho(); // Importante: previene la stampa dei caratteri

    switch (input) {
        case KEY_UP:    case 'w': snake.setDirection(up); break;
        case KEY_DOWN:  case 's': snake.setDirection(down); break;
        case KEY_LEFT:  case 'a': snake.setDirection(left); break;
        case KEY_RIGHT: case 'd': snake.setDirection(right); break;
        case 'p': case 'P':
            showPauseMenu();
        break;
        default: break;
    }

    // RIPRISTINA L'ECHO SOLO SE NECESSARIO
    // noecho() rimane disattivato per il gioco
}

void SnakeGame::showPauseMenu() {
    // Salva lo stato corrente del timeout
    board.setTimeout(-1);
    //flushinp();

    // OTTIENI LE DIMENSIONI REALI DEL TERMINALE
    int screen_height, screen_width;
    getmaxyx(stdscr, screen_height, screen_width);

    // CREA UN OVERLAY SU TUTTO LO SCHERMO
    WINDOW* overlay_win = newwin(screen_height, screen_width, 0, 0);
    wbkgd(overlay_win, COLOR_PAIR(2)); // Usa un colore scuro
    wattron(overlay_win, A_DIM);
    for (int y = 0; y < screen_height; y++) {
        for (int x = 0; x < screen_width; x++) {
            mvwprintw(overlay_win, y, x, " ");
        }
    }
    wrefresh(overlay_win);

    // CREA IL MENU DI PAUSA CENTRATO SULLO SCHERMO
    int menu_height = 7;
    int menu_width = 30;
    int start_y = (screen_height - menu_height) / 2;
    int start_x = (screen_width - menu_width) / 2;

    WINDOW* pause_win = newwin(menu_height, menu_width, start_y, start_x);
    keypad(pause_win, TRUE);

    // SFONDO DEL MENU
    wbkgd(pause_win, COLOR_PAIR(1));
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
            mvwprintw(pause_win, i + 3, x_pos, "%s", options[i]);
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
                //flushinp();
                board.setTimeout(currentSpeed);
                if (highlight == 1) forceGameOver();
                return;
            case 'p': case 'P': case 27: // P o ESC per riprendere
                delwin(pause_win);
                delwin(overlay_win);
                touchwin(stdscr);
                refresh();
                //flushinp();
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

        // ATTIVA EFFETTO BORDO VERDE
        borderFlashCount = 10;
        borderGreen = true;
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

    // GESTISCI EFFETTO BORDO VERDE
    if (borderFlashCount > 0) {
        borderFlashCount--;
        if (borderFlashCount == 0) {
            borderGreen = false;
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
    //werase(hud);
    //box(hud, 0, 0);

    if (borderGreen && borderFlashCount > 0) {
        drawGreenBorder(); // Bordo verde
    } else {
        board.drawBorder(); // Bordo normale
    }

    int elapsed = static_cast<int>(time(nullptr) - startTime);
    int remaining = timeLimit - elapsed;

    // stampa punteggio e tempo rimanente in alto
    //mvwprintw(hud, 1, 1, "Score: %d", score);
    //mvwprintw(hud, 2, 1, "Time left: %02d", remaining);
    //wrefresh(hud);

    //prova printing sotto la board principale
    int xMax, yMax;
    getmaxyx(stdscr, yMax, xMax);
    int startY = (yMax - board.getHeight()) / 2;
    int startX = (xMax - board.getWidth()) / 2;
    int infoY = startY + board.getHeight();

    mvwprintw(stdscr, infoY, startX, "Score: %d", score);
    mvwprintw(stdscr, infoY, startX + board.getWidth() - 14, "Time left: %d", remaining);
    wrefresh(stdscr);
}

void SnakeGame::drawGreenBorder() {
    WINDOW* win = board.getWindow(); // Usa il nuovo metodo
    if (!win) return;

    int height = board.getHeight();
    int width = board.getWidth();

    // USA I COLORI SE SUPPORTATI
    if (has_colors()) {
        wattron(win, COLOR_PAIR(2)); // Verde
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                if (y == 0 || y == height - 1 || x == 0 || x == width - 1) {
                    chtype current = mvwinch(win, y, x);
                    if (current != 'O' && current != 'o') {
                        mvwaddch(win, y, x, '*');
                    }
                }
            }
        }
        wattroff(win, COLOR_PAIR(2));
    } else {
        // FALLBACK: carattere speciale per bordo verde
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                if (y == 0 || y == height - 1 || x == 0 || x == width - 1) {
                    chtype current = mvwinch(win, y, x);
                    if (current != 'O' && current != 'o') {
                        mvwaddch(win, y, x, '#'); // Carattere speciale
                    }
                }
            }
        }
    }
    wrefresh(win);
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
