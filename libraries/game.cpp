#include "game.h"
#include <curses.h>
#include "leaderboard.h"
#include <string>
#include <iostream>

Game::Game() : playerName("") {
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

void Game::askPlayerName() {
    clear();
    echo(); // Abilita l'echo per l'input
    mvprintw(10, 10, "Inserisci il tuo nome: ");
    refresh();

    char name[50];
    getnstr(name, 49);
    playerName = name;

    noecho(); // Disabilita l'echo
}

void Game::start() {
    // Chiedi il nome del giocatore all'inizio
    askPlayerName();

    Livello* current = head;
    int totalScore = 0;

    while (true) {
        if (!current) {
            clear();
            mvprintw(5, 5, "Errore: livello non trovato!");
            refresh();
            getch();
            break;
        }

        // avvia il gioco sul livello corrente
        SnakeGame game(current->getHeight(), current->getWidth(),
                      current->getTimeLimit(), current->getAppleFactor());
        game.setGameSpeed(current->getSpeed());

        int startTime = static_cast<int>(time(nullptr));
        int timeLimit = current->getTimeLimit();

        while (!game.isOver()) {
            game.processInput();
            game.updateState();
            game.redraw();
        }

        // Aggiungi il punteggio del livello al punteggio totale
        int levelScore = game.getScore();
        totalScore += levelScore;
        current->addScore(levelScore);

        // messaggio post-partita
        clear();
        mvprintw(10, 10, "Game Over su %s", current->getName().c_str());
        mvprintw(11, 10, "Punteggio livello: %d", levelScore);
        mvprintw(12, 10, "Punteggio totale: %d", totalScore);

        // Controlla se è l'ultimo livello
        if (current == tail) {
            mvprintw(14, 10, "Hai completato tutti i livelli!");
            mvprintw(15, 10, "Punteggio finale: %d", totalScore);
            mvprintw(17, 10, "Premi un tasto per vedere la classifica...");
            refresh();
            getch();

            // Aggiungi alla classifica
            Leaderboard leaderboard("../scoreboard/scoreboard.txt");
            leaderboard.addScore(playerName, totalScore);
            leaderboard.show();

            break;
        } else {
            mvprintw(14, 10, "Premi 'n' (next) per livello successivo");
            mvprintw(15, 10, "Premi 'b' (back) per livello precedente");
            mvprintw(16, 10, "Premi 'q' per tornare al menu");
            refresh();

            int ch;
            while (true) {
                ch = getch();
                if (ch == 'q') {
                    if (totalScore > 0) {
                        Leaderboard leaderboard("../scoreboard/scoreboard.txt");
                        leaderboard.addScore(playerName, totalScore);
                    }
                    if (totalScore > 0) {
                        Leaderboard leaderboard("../scoreboard/scoreboard.txt");
                        leaderboard.addScore(playerName, totalScore);
                        leaderboard.show();
                    }
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
}