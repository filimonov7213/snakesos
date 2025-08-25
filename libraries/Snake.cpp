#include "Snake.h"

Snake::Snake()
    : fieldH(0), fieldW(0), occ(nullptr),
      headY(0), headX(0),
      cur_dir(right), next_dir(right) {
    for (int i = 0; i < FIXED_LENGTH; ++i) { segY[i] = -1; segX[i] = -1; }
}

Snake::~Snake() {
    freeOcc();
}

void Snake::freeOcc() {
    if (!occ) return;
    for (int r = 0; r < fieldH; ++r) delete[] occ[r];
    delete[] occ;
    occ = nullptr;
    fieldH = fieldW = 0;
}

void Snake::setFieldSize(int h, int w) {
    // re-alloc
    freeOcc();
    fieldH = h; fieldW = w;
    occ = new bool*[fieldH];
    for (int r = 0; r < fieldH; ++r) {
        occ[r] = new bool[fieldW];
    }
    clearOcc();
}

void Snake::clearOcc() {
    if (!occ) return;
    for (int r = 0; r < fieldH; ++r)
        for (int c = 0; c < fieldW; ++c)
            occ[r][c] = false;
}

void Snake::initialize(int startY, int startX) {
    // direzione di default a destra
    cur_dir = right;
    next_dir = right;

    // sicurezza per stare lontani dal bordo sinistro (corpo a sinistra della testa)
    if (startX < 3) startX = 3;

    headY = startY;
    headX = startX;

    // FIFO: 0 testa, poi corpo verso sinistra
    segY[0] = headY;     segX[0] = headX;
    segY[1] = headY;     segX[1] = headX - 1;
    segY[2] = headY;     segX[2] = headX - 2;
    segY[3] = headY;     segX[3] = headX - 3;

    clearOcc();
    for (int i = 0; i < FIXED_LENGTH; ++i) {
        if (segY[i] >= 0 && segY[i] < fieldH && segX[i] >= 0 && segX[i] < fieldW) {
            occ[segY[i]][segX[i]] = true;
        }
    }
}

void Snake::setDirection(Direction d) {
    // no inversioni immediate
    if ((cur_dir == up    && d == down) ||
        (cur_dir == down  && d == up)   ||
        (cur_dir == left  && d == right)||
        (cur_dir == right && d == left)) {
        return;
    }
    next_dir = d;
}

Direction Snake::getDirection() const {
    return cur_dir;
}

void Snake::move() {
    // Applica la direzione richiesta
    cur_dir = next_dir;

    // calcola nuova testa
    int ny = headY, nx = headX;
    switch (cur_dir) {
        case up:    --ny; break;
        case down:  ++ny; break;
        case left:  --nx; break;
        case right: ++nx; break;
    }

    // rimuovi coda dalla matrice
    int tailY = segY[FIXED_LENGTH - 1];
    int tailX = segX[FIXED_LENGTH - 1];
    if (tailY >= 0 && tailY < fieldH && tailX >= 0 && tailX < fieldW) {
        occ[tailY][tailX] = false;
    }

    // shift FIFO: dal fondo verso l'alto
    for (int i = FIXED_LENGTH - 1; i > 0; --i) {
        segY[i] = segY[i - 1];
        segX[i] = segX[i - 1];
    }

    // nuova testa
    segY[0] = ny;
    segX[0] = nx;
    headY = ny;
    headX = nx;

    // marca testa nella matrice
    if (ny >= 0 && ny < fieldH && nx >= 0 && nx < fieldW) {
        occ[ny][nx] = true;
    }
}

bool Snake::isAt(int y, int x) const {
    if (!occ) return false;
    if (y < 0 || y >= fieldH || x < 0 || x >= fieldW) return false;
    return occ[y][x];
}

int Snake::getHeadY() const { return headY; }
int Snake::getHeadX() const { return headX; }

int Snake::getBodyY(int index) const { return segY[index]; }
int Snake::getBodyX(int index) const { return segX[index]; }

int Snake::getTailY() const { return segY[FIXED_LENGTH - 1]; }
int Snake::getTailX() const { return segX[FIXED_LENGTH - 1]; }
