#pragma once
#include <curses.h>

// Creare un Drawable è utile per non chiamare tantissime volte metodi quando hai dei modelli da disegnare.
// In questo caso per non richiamare tante volte addAt()

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
