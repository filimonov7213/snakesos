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
    clear();
    refresh();
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

void Board::add(Drawable drawable) {
    addAt(drawable.getY(), drawable.getX(), drawable.getIcon());
}

void Board::getEmptyCoordinates(int& y, int& x) {
    //mvwinch return whatever character is at position (y, x) in a specific window
    while ((mvwinch(board_win, y = rand() % height, x = rand() % width)) != ' ');
}

void Board::construct(int height, int width) {
    int xMax, yMax;
    getmaxyx(stdscr, yMax, xMax);
    this->height = height;
    this->width = width;
    start_row = (yMax / 2) - (height / 2);
    start_col = (xMax / 2) - (width / 2);

    board_win = newwin(height, width, start_row, start_col);
    //wtimeout(board_win, 500); //makes the board refresh every ms
    keypad(board_win, true);
}






