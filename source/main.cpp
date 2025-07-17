#include <curses.h>
#include "../libraries/Menu.hpp"

int main() {
  // todo: roba di curses
  // ...

  // implementazione menu
  Menu menu;
  int choice = -1;

  while (choice == -1) {
    menu.display();
    choice = menu.handleInput();
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