#pragma once
#include "livello.h"
#include "snakegame.h"

class Game {
public:
    Game();

    // avvia il gioco
    void start();

    // ciclo generale
    void loop();

private:
    bool running;
    Livello* livelloCorrente;
};