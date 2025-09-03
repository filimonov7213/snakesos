#include "Leaderboard.h"
#include <fstream>
#include <curses.h>
#include <iostream>
#include <cstring>

Leaderboard::Leaderboard(const std::string& filename) : file(filename), scoreCount(0) {
    // Inizializza l'array
    for (int i = 0; i < 10; i++) {
        scores[i].name = "";
        scores[i].score = 0;
    }
    load();
}

void Leaderboard::load() {
    scoreCount = 0;

    std::ifstream in(file);
    if (!in.is_open()) {
        return;
    }

    ScoreEntry entry;
    while (in >> entry.name >> entry.score && scoreCount < 10) {
        scores[scoreCount++] = entry;
    }
    in.close();

    // Ordinamento manuale (bubble sort)
    for (int i = 0; i < scoreCount - 1; i++) {
        for (int j = 0; j < scoreCount - i - 1; j++) {
            if (scores[j].score < scores[j + 1].score) {
                // Scambia
                ScoreEntry temp = scores[j];
                scores[j] = scores[j + 1];
                scores[j + 1] = temp;
            }
        }
    }
}

void Leaderboard::save() const {
    std::ofstream out(file, std::ios::trunc);
    if (!out.is_open()) {
        return;
    }

    for (int i = 0; i < scoreCount; i++) {
        out << scores[i].name << " " << scores[i].score << "\n";
    }
    out.close();
}

void Leaderboard::addScore(const std::string& name, int score) {
    // Controlla se già esiste
    for (int i = 0; i < scoreCount; i++) {
        if (scores[i].name == name && scores[i].score == score) {
            return; // già presente
        }
    }

    // Aggiungi il nuovo punteggio
    if (scoreCount < 10) {
        scores[scoreCount].name = name;
        scores[scoreCount].score = score;
        scoreCount++;
    } else if (score > scores[9].score) {
        // Sostituisci l'ultimo se il punteggio è migliore
        scores[9].name = name;
        scores[9].score = score;
    }

    // Riordina (bubble sort)
    for (int i = 0; i < scoreCount - 1; i++) {
        for (int j = 0; j < scoreCount - i - 1; j++) {
            if (scores[j].score < scores[j + 1].score) {
                ScoreEntry temp = scores[j];
                scores[j] = scores[j + 1];
                scores[j + 1] = temp;
            }
        }
    }

    save();
}

void Leaderboard::show() const {
    int old_flags = curs_set(0);
    bool old_echo = (echo() == OK);
    if (old_echo) noecho();

    clear();
    refresh();

    // Centra il titolo
    int yMax, xMax;
    getmaxyx(stdscr, yMax, xMax);
    std::string title = "===== CLASSIFICA TOP 10 =====";
    int titleX = (xMax - title.length()) / 2;

    mvprintw(5, titleX, "%s", title.c_str());

    int y = 7;
    if (scoreCount == 0) {
        std::string noScores = "Nessun punteggio registrato!";
        int noScoresX = (xMax - noScores.length()) / 2;
        mvprintw(y++, noScoresX, "%s", noScores.c_str());
    } else {
        for (int i = 0; i < scoreCount; ++i) {
            char buffer[50];
            snprintf(buffer, sizeof(buffer), "%2d. %-15s %5d",
                    i+1, scores[i].name.c_str(), scores[i].score);

            int lineX = (xMax - strlen(buffer)) / 2;
            mvprintw(y++, lineX, "%s", buffer);
        }
    }

    std::string pressKey = "Premi un tasto per tornare al menu...";
    int pressKeyX = (xMax - pressKey.length()) / 2;
    mvprintw(y+2, pressKeyX, "%s", pressKey.c_str());

    refresh();

    timeout(-1);
    getch();
    timeout(0);

    if (old_echo) echo();
    curs_set(0);

    clear();
    refresh();
}