//
// Created by victor on 17/07/2025.
//

#include "Menu.h"
#include <curses.h>

Menu::Menu() {
    options[0] = "Nuova Partita";
    options[1] = "Visualizza Classifica";
    selectedIndex = 0;
}

void Menu::display() {
  clear();
  mvprintw(1, 5, "==== MENU ====");
    for (int i = 0; i < N_OPTIONS; ++i) {
        if (i == selectedIndex) {
            attron(A_REVERSE); // evidenzia
        }
        mvprintw(3 + i, 5, options[i].c_str()); // mvprintw vuole un array e c_str manda l'array di char
        if (i == selectedIndex) {
            attroff(A_REVERSE);
        }
    }
    refresh();
}

int Menu::gestisciInput() {
    int ch = getch(); // prende il carattere che l'utente clicca
    switch (ch) {
        case KEY_UP:
        case 'w':
        case 'W':
            selectedIndex = (selectedIndex - 1 + NUM_OPTIONS) % NUM_OPTIONS; // trucchetto per scorrere in alto ciclicamente, cioè se sei in cima al menu, torni all’ultima LOOP CIRCOLARE
        break;
        case KEY_DOWN:
        case 's':
        case 'S':
            selectedIndex = (selectedIndex + 1) % NUM_OPTIONS; // trucchetto per scorrere in alto ciclicamente, cioè se sei in cima al menu, torni all’ultima LOOP CIRCOLARE
        break;
        case '\n': // Invio
        case KEY_ENTER:
            return selectedIndex;
    }
    return -1;
}