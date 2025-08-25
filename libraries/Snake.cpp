#include "Snake.h"
#include <cstring>

Snake::Snake() : headY(0), headX(0), cur_dir(right), next_dir(right) {
    for (int i = 0; i < FIXED_LENGTH - 1; i++) {
        bodyY[i] = -1;
        bodyX[i] = -1;
    }
}

void Snake::initialize(int startY, int startX) {
    headY = startY;
    headX = startX;
    cur_dir = right;
    next_dir = right;

    // Inizializza il corpo in posizioni consecutive a sinistra della testa
    for (int i = 0; i < FIXED_LENGTH - 1; i++) {
        bodyY[i] = startY;
        bodyX[i] = startX - (i + 1);
    }
}


/* Non puoi andare su se stai andando giù

Non puoi andare destra se stai andando
sinistra

Usa next_dir per applicare il cambio al prossimo movimento*/

void Snake::setDirection(Direction d) {
    // Previene cambi di direzione opposti
    if ((cur_dir == up && d != down) ||
        (cur_dir == down && d != up) ||
        (cur_dir == left && d != right) ||
        (cur_dir == right && d != left)) {
        next_dir = d;
    }
}

Direction Snake::getDirection() const {
    return cur_dir;
}

void Snake::move() {
    // Applica il cambio di direzione
    cur_dir = next_dir;

    // Salva la posizione corrente della testa
    int prevHeadY = headY;
    int prevHeadX = headX;

    // Muovi la testa nella nuova direzione
    switch (cur_dir) {
        case up:    headY--; break;
        case down:  headY++; break;
        case left:  headX--; break;
        case right: headX++; break;
    }

    // Muovi il corpo: ogni segmento prende la posizione del precedente
    for (int i = FIXED_LENGTH - 2; i > 0; i--) {
        bodyY[i] = bodyY[i - 1];
        bodyX[i] = bodyX[i - 1];
    }

    // Il primo segmento del corpo prende la posizione precedente della testa
    bodyY[0] = prevHeadY;
    bodyX[0] = prevHeadX;
}

bool Snake::isAt(int y, int x) const {
    // Controlla la testa
    if (y == headY && x == headX) {
        return true;
    }

    // Controlla il corpo
    for (int i = 0; i < FIXED_LENGTH - 1; i++) {
        if (bodyY[i] == y && bodyX[i] == x) {
            return true;
        }
    }

    return false;
}

void Snake::setHeadPosition(int y, int x) {
    headY = y;
    headX = x;
}

int Snake::getHeadY() const { return headY; }
int Snake::getHeadX() const { return headX; }
int Snake::getBodyY(int index) const { return bodyY[index]; }
int Snake::getBodyX(int index) const { return bodyX[index]; }

