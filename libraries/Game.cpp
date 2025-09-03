#include "Game.h"
#include <curses.h>
#include "Leaderboard.h"
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

void Game::displayLevelCompleteScreen(Livello* current, int levelScore, int totalScore, bool isFinalLevel) {
    clear();

    // Ottieni le dimensioni dello schermo
    int yMax, xMax;
    getmaxyx(stdscr, yMax, xMax);

    // Calcola la posizione di partenza per centrare il contenuto
    int startY = (yMax - 15) / 2;
    int startX = (xMax - 50) / 2;

    // Colori e attributi
    attron(A_BOLD);

    // Titolo
    // Calcola le posizioni centrate
    std::string line1 = "==================================================";
    std::string line2 = "                 FINE LIVELLO                 ";
    std::string line3 = "==================================================";

    int startX1 = (xMax - line1.length()) / 2;
    int startX2 = (xMax - line2.length()) / 2;
    int startX3 = (xMax - line3.length()) / 2;

    // Stampa le linee centrate
    mvprintw(startY, startX1, "%s", line1.c_str());
    mvprintw(startY + 1, startX2, "%s", line2.c_str());
    mvprintw(startY + 2, startX3, "%s", line3.c_str());

    // Informazioni livello
    attroff(A_BOLD);
    mvprintw(startY + 4, startX, "Livello completato: %s", current->getName().c_str());
    mvprintw(startY + 5, startX, "Punteggio livello: %d", levelScore);
    mvprintw(startY + 6, startX, "Punteggio totale: %d", totalScore);

    // Linea separatrice
    attron(A_BOLD);
    mvprintw(startY + 8, startX, "--------------------------------------------------");
    attroff(A_BOLD);

    int currentLine = startY + 10; // Inizia dalle opzioni

    if (isFinalLevel) {
        // Messaggio per livello finale
        mvprintw(currentLine, startX, "Hai completato tutti i livelli!");
        currentLine++;
        mvprintw(currentLine, startX, "Punteggio finale: %d", totalScore);
        currentLine += 2; // Salta una riga
        mvprintw(currentLine, startX, "Premi un tasto per vedere la classifica...");
        currentLine++;
    } else {
        // Opzioni per livelli intermedi
        mvprintw(currentLine, startX, "Premi 'N' - Prossimo livello");
        currentLine++;

        // Mostra "Livello precedente" solo se non siamo al primo livello
        if (current != head) {
            mvprintw(currentLine, startX, "Premi 'B' - Livello precedente");
            currentLine++;
        }

        mvprintw(currentLine, startX, "Premi 'Q' - Esci e vedi classifica");
        currentLine++;
    }

    // Cornice inferiore - posizionata dinamicamente
    attron(A_BOLD);
    mvprintw(currentLine + 1, startX, "==================================================");
    attroff(A_BOLD);

    refresh();
}

void Game::askPlayerName() {
    clear();
    timeout(-1);
    noecho();
    curs_set(1);

    // Messaggio di input
    mvprintw(10, 10, "Inserisci il tuo nome: ");
    refresh();

    char name[50];
    int ch;
    int pos = 0;

    mvprintw(11, 10, "> ");
    refresh();

    int input_x = 12;

    while (pos < 49) {
        ch = getch();

        if (ch == '\n' || ch == KEY_ENTER || ch == '\r') {
            break;
        } else if (ch == KEY_BACKSPACE || ch == 127 || ch == 8) {
            if (pos > 0) {
                pos--;
                mvprintw(11, input_x + pos, " ");
                refresh();
            }
        } else if (isprint(ch)) {
            name[pos] = ch;
            mvprintw(11, input_x + pos, "%c", ch);
            pos++;
            refresh();
        }
    }

    name[pos] = '\0';
    playerName = name;

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
    timeout(0);

    clear();
    refresh();
}

void Game::start() {
    def_prog_mode();
    askPlayerName();

    Livello* current = head;
    int totalScore = 0;
    bool game_over = false;
    int last_choice = 0;

    while (!game_over && current) {
        SnakeGame game(current->getHeight(), current->getWidth(),
                      current->getTimeLimit(), current->getAppleFactor());
        game.setGameSpeed(current->getSpeed());

        while (!game.isOver()) {
            game.processInput();
            game.updateState();
            game.redraw(totalScore);
            napms(10);
        }

        int levelScore = game.getScore();
        totalScore += levelScore;
        current->addScore(levelScore);

        // USA IL NUOVO METODO
        bool isFinalLevel = (current == tail);
        displayLevelCompleteScreen(current, levelScore, totalScore, isFinalLevel);

        if (isFinalLevel) {
            timeout(-1);
            getch();
            timeout(0);

            Leaderboard leaderboard("../scoreboard/scoreboard.txt");
            leaderboard.addScore(playerName, totalScore);
            leaderboard.show();

            game_over = true;
        } else {
            int ch;
            timeout(-1);
            noecho();

            while (true) {
                ch = getch();
                if (ch == 'q' || ch == 'Q') {
                    last_choice = 1;
                    break;
                } else if (ch == 'n' || ch == 'N') {
                    current = current->next;
                    break;
                } else if ((ch == 'b' || ch == 'B') && current != head) {
                    // Permetti di tornare indietro solo se non siamo al primo livello
                    current = current->prev;
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
        int yMax, xMax;
        getmaxyx(stdscr, yMax, xMax);
        int startY = (yMax - 10) / 2;
        int startX = (xMax - 50) / 2;

        attron(A_BOLD);
        mvprintw(startY, startX, "==================================================");
        mvprintw(startY + 1, startX, "            PARTITA INTERROTTA               ");
        mvprintw(startY + 2, startX, "==================================================");
        attroff(A_BOLD);

        mvprintw(startY + 4, startX, "Punteggio finale: %d", totalScore);
        mvprintw(startY + 6, startX, "Premi un tasto per vedere la classifica...");
        refresh();

        timeout(-1);
        getch();
        timeout(0);

        leaderboard.show();
    }

    reset_prog_mode();
    refresh();
}