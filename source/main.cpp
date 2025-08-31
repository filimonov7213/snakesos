/*#include "../libraries/board.h"
#include "../libraries/snakegame.h"
#include "../libraries/drawable.h"

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
#include "menu.h"
#include "game.h"
#include "leaderboard.h"
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
  }
}

int main() {
  std::cout << "Inizializzazione ncurses..." << std::endl;
  initscr();
  std::cout << "ncurses inizializzato" << std::endl;

  noecho();
  cbreak();
  curs_set(0);
  keypad(stdscr, TRUE);
  timeout(0);

  std::cout << "Configurazione completata" << std::endl;

  bool running = true;

  while (running) {
    std::cout << "Creazione menu..." << std::endl;
    Menu menu;
    std::cout << "Menu creato" << std::endl;

    int choice = menu.show();
    std::cout << "Scelta menu: " << choice << std::endl;

    if (choice == 0) {
      std::cout << "Avvio gioco..." << std::endl;
      Game game;
      game.start();
      std::cout << "Gioco terminato" << std::endl;
    }
    else if (choice == 1) {
      std::cout << "Mostro classifica..." << std::endl;
      Leaderboard leaderboard("scoreboard.txt");
      leaderboard.show();
      std::cout << "Classifica mostrata" << std::endl;
    }
    else {
      running = false;
    }
  }

  endwin();
  return 0;
}
