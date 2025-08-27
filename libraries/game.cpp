#include "game.h"
#include <curses.h>


Game::Game() {
    // creo la lista doppiamente collegata con tempo limite per livello
    Livello* lvl1 = new Livello(1, 200, 30, 60, 20, 1);   // lento - 20 secondi
    Livello* lvl2 = new Livello(2, 100, 30, 60, 30, 3);   // medio - 30 secondi
    Livello* lvl3 = new Livello(3, 50,  30, 60, 40, 5);   // veloce - 40 secondi

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

        // avvia il gioco sul livello corrente
        SnakeGame game(current->getHeight(), current->getWidth(), current->getTimeLimit(), current->getAppleFactor());

        game.setGameSpeed(current->getSpeed());

        // calcola tempo limite
        int startTime = static_cast<int>(time(nullptr));
        //auto startTime = std::chrono::steady_clock::now();
        int timeLimit = current->getTimeLimit();

        while (!game.isOver()) {
            game.processInput();
            game.updateState();
            game.redraw();

            // calcola tempo trascorso
            /*auto now = std::chrono::steady_clock::now();
            int elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - startTime).count();*/

            //int elapsed = static_cast<int>(time(nullptr) - startTime);

            /*if (elapsed >= timeLimit) {
                game.forceGameOver(); // serve aggiungere metodo in SnakeGame
                break;
            }*/
            // 1 punto ogni ciclo vivo (puoi bilanciarlo)
        }

        // messaggio post-partita
        clear();
        mvprintw(10, 10, "Game Over su %s", current->getName().c_str());
        mvprintw(11, 10, "Punteggio: %d", game.getScore());
        mvprintw(13, 10, "Premi 'n' (next) per livello successivo");
        mvprintw(14, 10, "Premi 'b' (back) per livello precedente");
        mvprintw(15, 10, "Premi 'q' per tornare al menu");
        refresh();

        int ch;
        while (true) {
            ch = getch();
            if (ch == 'q') {
                clear();
                refresh();
                return;
            } else if (ch == 'n' && current->next != nullptr) {
                clear();
                refresh();
                current = current->next;
                break;
            } else if (ch == 'b' && current->prev != nullptr) {
                clear();
                refresh();
                current = current->prev;
                break;
            }
        }
    }
}
