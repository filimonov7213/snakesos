#pragma once

#include <curses.h>
#include <queue>
#include "drawable.h"

enum Direction {
    up = -1,
    down = 1,
    left = -2,
    right = 2

    // the illegal moves (like going up and down at the same time) add up to 0.
    // But the non-illegal moves don't add up to 0
    // this allows setDirection() to don't have illegal moves.
};

class SnakePiece : public Drawable {
public:
    SnakePiece();
    SnakePiece(int y, int x);
};

//queue class FIFO
class Snake {
public:
    Snake();

    //add a piece to the queue, "piece" is a "Drawable"
    void addPiece(SnakePiece piece);

    //remove a piece from the queue
    void removePiece();

    SnakePiece tail();

    SnakePiece head();

    Direction getDirection();

    void setDirection(Direction d);

    //takes the current head and decide what the position of the next head is going to be
    SnakePiece nextHead();

private:
    std::queue<SnakePiece> prev_pieces; //prev_pieces is a queue made of SnakePiece
    Direction cur_direction;
};
























