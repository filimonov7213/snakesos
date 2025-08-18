#include "Snake.h"

SnakePiece::SnakePiece() {
    this->x = this->y = 0;
    this->icon = '#';
}

SnakePiece::SnakePiece(int y, int x) {
    this->y = y;
    this->x = x;
    this->icon = '#';
}

Snake::Snake() {
    cur_direction = down;
}

void Snake::addPiece(SnakePiece piece) {
    prev_pieces.push(piece);
}

void Snake::removePiece() {
    prev_pieces.pop();
}

SnakePiece Snake::head() {
    return prev_pieces.back();
}

SnakePiece Snake::tail() {
    return prev_pieces.front();
}

Direction Snake::getDirection() {
    return cur_direction;
}

void Snake::setDirection(Direction d) {
    if (cur_direction + d != 0)
        cur_direction = d;
}

SnakePiece Snake::nextHead() {
    // finds the coords of the head of the queue
    int row = head().getY();
    int col = head().getX();

    switch (cur_direction) {
        case down:
            row++;
            break;
        case up:
            row--;
            break;
        case right:
            col++;
            break;
        case left:
            col--;
            break;
    }
    return SnakePiece(row,col);
}






