#include "board.h"
#include "snakegame.h"
#include <cstdlib>
#include <ctime>

Board::Board() {
    construct(0,0);
}

Board::Board(int height, int width) {
    construct(height, width);
}

void Board::initialize() {
    // CREA FINESTRA CON CONTROLLO
    window = newwin(height, width, 0, 0);
    if (!window) {
        // Fallback: usa stdscr se newwin fallisce
        window = stdscr;
    }

    keypad(window, TRUE);
    nodelay(window, TRUE);
    wtimeout(window, 0);
}

void Board::addBorder() {
    box(board_win, 0, 0);
}

void Board::addAt(int y, int x, char ch) {
    mvwaddch(board_win, y, x, ch);
}

chtype Board::getCharAt(int y, int x) {
    return mvwinch(board_win, y, x);
}

chtype Board::getInput() {
    return wgetch(board_win);
}

void Board::clear() {
    wclear(board_win);
    addBorder();
}

void Board::refresh() {
    wrefresh(board_win);
}

const int Board::getWidth() {
    return width;
}

const int Board::getHeight() {
    return height;
}

void Board::setTimeout(int timeout) { wtimeout(board_win, timeout); }

void Board::add(Drawable drawable) {
    addAt(drawable.getY(), drawable.getX(), drawable.getIcon());
}

void Board::getEmptyCoordinates(int& y, int& x) {
    //mvwinch return whatever character is at position (y, x) in a specific window
    while ((mvwinch(board_win, y = rand() % height, x = rand() % width)) != ' ');
}

void Board::drawBorder() {
    if (board_win) {
        box(board_win, 0, 0);   // ridisegna riquadro
        wrefresh(board_win);    // aggiorna la finestra
    }
}

void Board::construct(int height, int width) {
    int xMax, yMax;
    getmaxyx(stdscr, yMax, xMax);
    this->height = height;
    this->width = width;

    board_win = newwin(height, width, (yMax / 2) - (height / 2), (xMax / 2) - (width / 2));
    //wtimeout(board_win, 500); //makes the board refresh every ms
    keypad(board_win, true);
}






