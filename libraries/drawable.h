#pragma once
#include <curses.h>

// Creating a drawable is useful to not spam methods when u have a lot of models.
// In this case in order to not spam the addAt() method.

class Drawable {
public:
    Drawable();
    Drawable(int y, int x, chtype ch);

    int getY();
    int getX();
    chtype getIcon();

protected:
    int y, x;
    chtype icon;
};
