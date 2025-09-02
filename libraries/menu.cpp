#include "menu.h"
#include <curses.h>

Menu::Menu() {
    options[0] = "Nuova Partita";
    options[1] = "Visualizza Classifica";
    selectedIndex = 0;
}

void Menu::display() const {
    clear();
    mvprintw(1, 5, "==== MENU ====");
    for (int i = 0; i < N_OPTIONS; ++i) {
        if (i == selectedIndex) {
            attron(A_REVERSE); // evidenzia
        }
        mvprintw(3 + i, 5, "%s", options[i].c_str());
        if (i == selectedIndex) {
            attroff(A_REVERSE);
        }
    }
    refresh();
}

int Menu::show() {
    int choice = -1;
    nodelay(stdscr, FALSE); // Disabilita nodelay per input bloccante
    timeout(-1); // Input bloccante

    display(); // Mostra il menu una volta

    while (choice == -1) {
        choice = gestisciInput();
    }

    return choice;
}

int Menu::gestisciInput() {
    int ch = getch(); // Blocca finché non c'è input

    switch (ch) {
        case KEY_UP:
        case 'w':
        case 'W':
            selectedIndex = (selectedIndex - 1 + N_OPTIONS) % N_OPTIONS;
            display(); // Ridisegna solo quando c'è un cambiamento
        break;
        case KEY_DOWN:
        case 's':
        case 'S':
            selectedIndex = (selectedIndex + 1) % N_OPTIONS;
            display(); // Ridisegna solo quando c'è un cambiamento
        break;
        case '\n':
        case KEY_ENTER:
        case ' ': // Anche spazio per selezione
            return selectedIndex;
    }
    return -1;
}