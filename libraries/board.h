#pragma once
#include <curses.h>
#include "drawable.h"

class Board {
public:

    Board();

    Board(int height, int width);

    //descriptive method: add border to the window
    void addBorder();

    //descriptive method: move cursor to a (y, x) point in the window and add a character
    void addAt(int y, int x, char ch);

    //returns the input key or character read from the window and gives a free refresh
    chtype getInput();

    // this function is going to loop until it find a blank character at some random (y, x)
    // and when it's done it will have saved the value in the y and in the x
    void getEmptyCoordinates(int& y, int& x);

    //runs clear() and refresh()
    void initialize();

    //cleans the whole window and runs addBorder()
    void clear();

    // descriptive method: refreshes the window
    void refresh();

    // uses the drawable class to add "icons" to the window. Ex. add(apple) or add(snake)
    void add(Drawable drawable);

    //for the implementation of PAUSE
    void setTimeout(int timeout) {
        wtimeout(board_win, timeout);
    }

    int getHeight() const { return height; }
    int getWidth()  const { return width; }

    chtype getCharAt(int y, int x);

private:
    WINDOW *board_win;
    int height, width;

    //creates a window, needed to create the default constructor
    void construct(int height, int width);
};
