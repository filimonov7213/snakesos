/*#include "../libraries/Board.h"
#include "../libraries/Snakegame.h"
#include "../libraries/Drawable.h"

int main() {
  // todo: roba di curses
  initscr();            // inizializza lo schermo
  noecho();             // non mostra i tasti premuti
  cbreak();             // leggi i tasti subito, senza invio
  curs_set(0);          // nascondi cursore
  keypad(stdscr, TRUE); // abilita tasti speciali

  // implementazione menu
  Menu menu;
  int choice = -1;

  while (choice == -1) {
    menu.display();
    choice = menu.gestisciInput();
  }

  switch (choice) {
    case 0:
      printw("Hai scelto: Nuova partita\n");
    // game
      initscr();
      Game game;
      game.start();
      endwin();
    break;
    case 1:
      printw("Hai scelto: Visualizza classifica\n");
    break;
  }
}*/
//ciao
#include <curses.h>
#include "Menu.h"
#include "Game.h"
#include "Leaderboard.h"
#include <iostream>

void cleanupNcurses() {
  endwin();
}

void initNcurses() {
  setlocale(LC_ALL, ""); // Importante per Linux
  initscr();

  if (!stdscr) {
    std::cerr << "Errore: impossibile inizializzare ncurses!" << std::endl;
    return;
  }

  initscr();
  noecho();
  cbreak();
  curs_set(0);
  keypad(stdscr, TRUE);
  timeout(0);

  // Inizializza colori se supportati
  if (has_colors()) {
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(2, COLOR_RED, COLOR_BLACK);
    init_pair(3, COLOR_GREEN, COLOR_BLACK);
    init_pair(4, COLOR_BLUE, COLOR_BLACK);
    init_pair(5, COLOR_CYAN, COLOR_BLACK);
    init_pair(6, COLOR_YELLOW, COLOR_BLACK);
    printw("Colori inizializzati\n");
  }
}

int main() {
  setlocale(LC_ALL, "");
  bool running = true;

  while (running) {
    // Inizializza ncurses ad ogni ciclo
    initNcurses();

    Menu menu;
    int choice = menu.show();

    if (choice == 0) {
      Game game;
      game.start();
    }
    else if (choice == 1) {
      Leaderboard leaderboard("../scoreboard/scoreboard.txt");
      leaderboard.show();
    }
    else {
      running = false;
    }

    // Pulisci completamente ncurses prima del prossimo ciclo
    cleanupNcurses();
  }

  return 0;
}
