//
// Created by tomai on 25/08/2025.
//

#ifndef SCOREBOARD_H
#define SCOREBOARD_H

#include <curses.h>

class Scoreboard {
private:
    WINDOW *score_win;

public:
    // Costruttore di default
    Scoreboard();

    // Costruttore con parametri
    Scoreboard(int width, int y, int x);

    // Inizializza la scoreboard con punteggio iniziale
    void initialize(int init_score);

    // Aggiorna il punteggio mostrato
    void updateScore(int score);

    // Pulisce la finestra
    void clear();

    // Aggiorna la visualizzazione
    void refresh();
};

#endif // SCOREBOARD_H