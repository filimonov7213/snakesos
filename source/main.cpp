#include <curses.h>
#include "../libraries/menu.h"

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
    break;
    case 1:
      printw("Hai scelto: Visualizza classifica\n");
    break;
  }
}