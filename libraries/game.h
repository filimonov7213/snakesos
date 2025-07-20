#ifndef GAME_H
#define GAME_H

#include "snake.hpp"
#include "grid.hpp"
#include "level.hpp"

class Game {
 private:
    Grid* grid;
    Snake* snake;
    Livello* livelloCorrente;

    int punteggio;
    bool running;

 public:
    Game();
    ~Game();

    void start(); // avvia una nuova partita
    void loop();  // gestisce il ciclo principale
    void handleInput(int ch);
    void update();
    void render();
};