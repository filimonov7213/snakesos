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

int main() {
  initscr();
  noecho();
  cbreak();
  curs_set(0);

  bool running = true;
  while (running) {
    Menu menu;
    int choice = menu.show();

    switch (choice) {
      case 0: { // Nuova partita
        Game game;
        game.start();
        break;
      }
      case 1: { // Classifica
        clear();
        mvprintw(5, 10, "Classifica non ancora implementata!");
        refresh();
        getch();
        break;
      }
      case 2: // Esci
        running = false;
      break;
    }
  }

  endwin();
  return 0;
}

