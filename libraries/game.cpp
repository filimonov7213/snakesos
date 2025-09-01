#include "game.h"
#include <curses.h>
#include "leaderboard.h"
#include <string>
#include <iostream>

Game::Game() : playerName("") {
    // creo la lista doppiamente collegata con tempo limite per livello
    Livello* lvl1 = new Livello(1, 200, 20, 60, 20, 1);   // lento - 20 secondi
    Livello* lvl2 = new Livello(2, 100, 20, 60, 30, 3);   // medio - 30 secondi
    Livello* lvl3 = new Livello(3, 50,  20, 60, 40, 5);   // veloce - 40 secondi

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
    timeout(-1); // Disabilita timeout per input bloccante
    noecho();    //  DISABILITA ECHO - lo gestiamo manualmente
    curs_set(1);

    // Messaggio di input
    mvprintw(10, 10, "Inserisci il tuo nome: ");
    refresh();

    char name[50];
    int ch;
    int pos = 0;

    // Usa una riga dedicata sotto il messaggio per l'input
    mvprintw(11, 10, "> "); // Prompt di input
    refresh();

    int input_x = 12; // Inizia dopo "> "

    while (pos < 49) {
        ch = getch();

        if (ch == '\n' || ch == KEY_ENTER || ch == '\r') {
            break;
        } else if (ch == KEY_BACKSPACE || ch == 127 || ch == 8) {
            if (pos > 0) {
                pos--;
                mvprintw(11, input_x + pos, " "); // Cancella il carattere
                refresh();
            }
        } else if (isprint(ch)) { // Carattere stampabile
            name[pos] = ch;
            mvprintw(11, input_x + pos, "%c", ch); //  Stampa manualmente
            pos++;
            refresh();
        }
    }

    name[pos] = '\0';
    playerName = name;

    // Messaggio di conferma
    if (!playerName.empty()) {
        mvprintw(13, 10, "Benvenuto, %s!", playerName.c_str());
    } else {
        playerName = "Player";
        mvprintw(13, 10, "Benvenuto, Player!");
    }
    mvprintw(15, 10, "Premi un tasto per continuare...");
    refresh();

    getch();

    curs_set(0);
    timeout(0); // Ripristina timeout normale

    clear();
    refresh();
}

void Game::start() {

    def_prog_mode();
    askPlayerName();

    Livello* current = head;
    int totalScore = 0;
    bool game_over = false;
    int last_choice = 0; // 0 = continua, 1 = quit

    while (!game_over && current) {
        SnakeGame game(current->getHeight(), current->getWidth(),
                      current->getTimeLimit(), current->getAppleFactor());
        game.setGameSpeed(current->getSpeed());

        while (!game.isOver()) {
            //flushinp();
            game.processInput();
            game.updateState();
            game.redraw();
            napms(10);
        }
        //flushinp();

        int levelScore = game.getScore();
        totalScore += levelScore;
        current->addScore(levelScore);

        clear();
        mvprintw(10, 10, "Game Over su %s", current->getName().c_str());
        mvprintw(11, 10, "Punteggio livello: %d", levelScore);
        mvprintw(12, 10, "Punteggio totale: %d", totalScore);

        if (current == tail) {
            mvprintw(14, 10, "Hai completato tutti i livelli!");
            mvprintw(15, 10, "Punteggio finale: %d", totalScore);
            mvprintw(17, 10, "Premi un tasto per vedere la classifica...");
            refresh();

            //flushinp();
            timeout(-1);
            getch();
            timeout(0);

            Leaderboard leaderboard("../scoreboard/scoreboard.txt");
            leaderboard.addScore(playerName, totalScore); // Aggiungi anche se 0
            leaderboard.show();

            game_over = true;
        } else {
            mvprintw(14, 10, "Premi 'n' (next) per livello successivo");
            mvprintw(15, 10, "Premi 'b' (back) per livello precedente");
            mvprintw(16, 10, "Premi 'q' per uscire e vedere la classifica"); // Modificato messaggio
            refresh();

            int ch;
            timeout(-1);
            noecho();

            while (true) {
                ch = getch();
                if (ch == 'q' || ch == 'Q') {
                    last_choice = 1; // quit
                    clear();
                    refresh();
                    break;
                } else if (ch == 'n' && current->next != nullptr) {
                    current = current->next;
                    clear();
                    refresh();
                    break;
                } else if (ch == 'b' && current->prev != nullptr) {
                    current = current->prev;
                    clear();
                    refresh();
                    break;
                }
            }

            echo();
            timeout(0);

            if (last_choice == 1) {
                game_over = true;
            }
        }
    }

    // MOSTRA CLASSIFICA SE ESCI PRIMA DEL TEMPO
    if (last_choice == 1) {
        Leaderboard leaderboard("../scoreboard/scoreboard.txt");
        leaderboard.addScore(playerName, totalScore);

        clear();
        if (totalScore > 0) {
            mvprintw(10, 10, "Hai abbandonato la partita");
            mvprintw(11, 10, "Punteggio finale: %d", totalScore);
        } else {
            mvprintw(10, 10, "Hai abbandonato la partita");
            mvprintw(11, 10, "Punteggio finale: 0");
        }
        mvprintw(13, 10, "Premi un tasto per vedere la classifica...");
        refresh();

        //flushinp();
        timeout(-1);
        getch();
        timeout(0);

        leaderboard.show();
    }

    reset_prog_mode();
    refresh();
}