#pragma once

enum Direction {
    up = -1,
    down = 1,
    left = -2,
    right = 2
};

class Snake {
public:
    static const int FIXED_LENGTH = 4; // testa + 3 segmenti

    Snake();
    ~Snake();

    // La matrice booleana di occupazione ha dimensione = campo di gioco
    void setFieldSize(int h, int w);

    // posiziona lo snake: testa a (startY, startX), corpo a sinistra
    void initialize(int startY, int startX);

    void setDirection(Direction d);
    Direction getDirection() const;

    // sposta: rimuove la coda (false nella matrice), aggiunge la testa (true)
    void move();

    // occupancy (matrice booleana): true se lo snake occupa (y,x)
    bool isAt(int y, int x) const;

    // accessori
    int getHeadY() const;
    int getHeadX() const;

    // segmenti in ordine: 0 = testa, 3 = coda
    int getBodyY(int index) const;
    int getBodyX(int index) const;

    int getTailY() const;
    int getTailX() const;

private:
    int fieldH, fieldW;
    bool **occ;              // matrice booleana di occupazione (snake-only)

    int headY, headX;        // coordinate testa
    int segY[FIXED_LENGTH];  // coda
    int segX[FIXED_LENGTH];

    Direction cur_dir;
    Direction next_dir;

    void clearOcc();         // setta tutta la matrice a false
    void freeOcc();          // dealloca la matrice
};
