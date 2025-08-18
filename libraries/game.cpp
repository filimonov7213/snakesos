#include "game.h"
#include <curses.h>

Game::Game() {
    running = true;

    // per ora livelli base
    Livello* livello1 = new Livello(1, 500); // 500ms
    Livello* livello2 = new Livello(2, 300);
    Livello* livello3 = new Livello(3, 150);

    livello1->setNext(livello2);
    livello2->setPrev(livello1);
    livello2->setNext(livello3);
    livello3->setPrev(livello2);

    livelloCorrente = livello1;
}

void Game::start() {
    initscr();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);

    loop();

    endwin();
}

void Game::loop() {
    while (running) {
        clear();
        mvprintw(0, 0, "Snake Game - Livello %d", livelloCorrente->getNumero());
        mvprintw(2, 0, "Premi 'n' per nuovo livello, 'b' per tornare indietro, 'q' per uscire, qualsiasi altro tasto per giocare.");
        refresh();

        int ch = getch();
        if (ch == 'q') {
            running = false;
            break;
        } else if (ch == 'n' && livelloCorrente->hasNext()) {
            livelloCorrente = livelloCorrente->getNext();
        } else if (ch == 'b' && livelloCorrente->hasPrev()) {
            livelloCorrente = livelloCorrente->getPrev();
        } else {
            // avvia una partita con SnakeGame
            SnakeGame partita(20, 50); // grandezza board
            // modifica velocità in base al livello
            // SnakeGame già usa wtimeout(500) -> lo cambiamo qui
            partita.initialize();

            while (!partita.isOver()) {
                partita.processInput();
                partita.updateState();
                partita.redraw();
            }

            mvprintw(22, 0, "Game Over! Premi un tasto per tornare al menu...");
            getch();
        }
    }
}
