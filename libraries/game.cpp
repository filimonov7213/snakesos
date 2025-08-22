#include "game.h"
#include <curses.h>

Game::Game() {
    // crea manualmente la lista doppiamente collegata
    Livello* lvl1 = new Livello(1, 500, 17, 40);  // lento
    Livello* lvl2 = new Livello(2, 300, 20, 50);  // medio
    Livello* lvl3 = new Livello(3, 150, 25, 60);  // veloce

    // collegamenti
    lvl1->next = lvl2;
    lvl2->prev = lvl1;
    lvl2->next = lvl3;
    lvl3->prev = lvl2;

    head = lvl1;
    tail = lvl3;
}

void Game::start() {
    Livello* current = head;

    while (true) {
        if (!current) {
            clear();
            mvprintw(5, 5, "Errore: livello non trovato!");
            refresh();
            getch();
            break;
        }

        // controlla dimensioni minime per evitare crash con newwin
        if (current->getHeight() < 2 || current->getWidth() < 2) {
            clear();
            mvprintw(5, 5, "Errore: dimensioni livello non valide!");
            refresh();
            getch();
            break;
        }

        // avvia il gioco sul livello corrente
        SnakeGame game(current->getHeight(), current->getWidth());

        while (!game.isOver()) {
            game.processInput();
            game.updateState();
            game.redraw();
        }

        // messaggio post-partita
        clear();
        mvprintw(10, 10, "Game Over su %s", current->getName().c_str());
        mvprintw(12, 10, "Premi 'n' (next) per livello successivo");
        mvprintw(13, 10, "Premi 'b' (back) per livello precedente");
        mvprintw(14, 10, "Premi 'q' per tornare al menu");
        refresh();

        int ch = getch();
        if (ch == 'q') {
            break;
        } else if (ch == 'n' && current->next != nullptr) {
            current = current->next;
        } else if (ch == 'b' && current->prev != nullptr) {
            current = current->prev;
        } else {
            // se premi altro rimane sullo stesso livello
        }
    }

    // pausa finale prima di tornare al menu principale
    clear();
    mvprintw(10, 10, "Uscita dal gioco. Premi un tasto per tornare al menu...");
    refresh();
    getch();
}
