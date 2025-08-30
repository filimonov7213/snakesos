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

void Snake::initialize(int startY, int startX, int height, int width) {
    // direzione di default a destra
    cur_dir = right;
    next_dir = right;

    // sicurezza per stare lontani dal bordo sinistro (corpo a sinistra della testa)
    if (startX < 3) startX = 3;

    headY = startY;
    headX = startX;

    boardHeight = height;
    boardWidth = width;

    // Inizializza il campo di occupazione
    setFieldSize(height, width);
    clearOcc();

    // Inizializza le posizioni del serpente
    for (int i = 0; i < FIXED_LENGTH; i++) {
        segY[i] = headY;
        segX[i] = headX - i;

        // Gestione wrap-around per le posizioni iniziali
        if (segX[i] < 0) {
            segX[i] += boardWidth;
        }

        // Segna come occupato
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

bool Snake::move() {
    cur_dir = next_dir;

    // calcolo nuova testa
    int ny = headY, nx = headX;
    switch (cur_dir) {
        case up:    ny--; break;
        case down:  ny++; break;
        case left:  nx--; break;
        case right: nx++; break;
    }

    // wrap-around
    if (ny < 0) ny = boardHeight - 1;
    else if (ny >= boardHeight) ny = 0;
    if (nx < 0) nx = boardWidth - 1;
    else if (nx >= boardWidth) nx = 0;

    // Controlla collisione PRIMA di rimuovere la coda
    if (occ[ny][nx]) {
        return false; // collisione con il corpo
    }

    // 👇 Rimuovi la coda dall'occupazione SOLO DOPO aver controllato
    int tailY = segY[FIXED_LENGTH - 1];
    int tailX = segX[FIXED_LENGTH - 1];
    if (tailY >= 0 && tailY < fieldH && tailX >= 0 && tailX < fieldW) {
        occ[tailY][tailX] = false;
    }

    // shift corpo
    for (int i = FIXED_LENGTH - 1; i > 0; --i) {
        segY[i] = segY[i - 1];
        segX[i] = segX[i - 1];
    }

    // nuova testa
    segY[0] = ny;
    segX[0] = nx;
    headY = ny;
    headX = nx;

    // segna testa nella matrice
    if (ny >= 0 && ny < fieldH && nx >= 0 && nx < fieldW) {
        occ[ny][nx] = true;
    }

    return true;
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
