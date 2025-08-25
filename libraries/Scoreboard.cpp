#include "Scoreboard.h"
#include <curses.h>

// Costruttore di default
Scoreboard::Scoreboard() {
  score_win = nullptr;
}

// Costruttore con parametri
Scoreboard::Scoreboard(int width, int y, int x) {
  score_win = newwin(1, width, y, x);
}

// Inizializza la scoreboard
void Scoreboard::initialize(int init_score) {
  wclear(score_win);
  mvwprintw(score_win, 0, 0, "Punteggio: %d", init_score);
  wrefresh(score_win);
}

// Aggiorna il punteggio
void Scoreboard::updateScore(int score) {
  wclear(score_win);
  mvwprintw(score_win, 0, 0, "Punteggio: %d", score);
  wrefresh(score_win);
}

// Pulisce la finestra
void Scoreboard::clear() {
  wclear(score_win);
  wrefresh(score_win);
}

// Aggiorna la visualizzazione
void Scoreboard::refresh() {
  wrefresh(score_win);
}