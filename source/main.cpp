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
  initscr();          // inizializza ncurses
  noecho();           // non mostra i tasti premuti
  cbreak();           // input immediato (senza invio)
  curs_set(0);        // nasconde il cursore
  keypad(stdscr, TRUE);

  bool running = true;

  while (running) {
    Menu menu;
    int choice = menu.show();

    if (choice == 0) {   // Nuova Partita
      Game game;
      game.start();    // gestisce livelli e loop interno
    }
    else if (choice == 1) { // Visualizza Classifica
      clear();
      mvprintw(5, 5, "Classifica (da implementare)");
      mvprintw(7, 5, "Premi un tasto per tornare al menu...");
      refresh();
      getch();
    }
    else {
      running = false; // ESC o uscita dal menu
    }
  }

  endwin();  // chiude ncurses
  return 0;
}

