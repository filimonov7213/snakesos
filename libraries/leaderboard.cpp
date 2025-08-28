#include "leaderboard.h"
#include <fstream>
#include <algorithm>
#include <curses.h>
#include <iostream>

Leaderboard::Leaderboard(const std::string& filename) : file(filename) {
    load();
}

void Leaderboard::load() {
    scores.clear();

    std::ifstream in(file);
    if (!in.is_open()) {
        // DEBUG: mostra messaggio di errore
        // mvprintw(20, 10, "File %s non trovato!", file.c_str());
        // refresh();
        return;
    }

    ScoreEntry entry;
    while (in >> entry.name >> entry.score) {
        scores.push_back(entry);
    }
    in.close();

    std::sort(scores.begin(), scores.end(), [](auto& a, auto& b){
        return a.score > b.score;
    });

    if (scores.size() > 10) scores.resize(10);
}

void Leaderboard::save() const {
    std::ofstream out(file, std::ios::trunc);
    if (!out.is_open()) {
        // DEBUG: mostra errore di salvataggio
        // mvprintw(20, 10, "Errore salvataggio in %s!", file.c_str());
        // refresh();
        return;
    }

    for (auto& e : scores) {
        out << e.name << " " << e.score << "\n";
    }
    out.close();
}

void Leaderboard::addScore(const std::string& name, int score) {
    scores.push_back({name, score});
    std::sort(scores.begin(), scores.end(), [](auto& a, auto& b){
        return a.score > b.score;
    });
    if (scores.size() > 10) scores.resize(10);
    save();
}

void Leaderboard::show() const {
    clear();
    mvprintw(5, 10, "===== CLASSIFICA TOP 10 =====");
    //mvprintw(6, 10, "File: %s", file.c_str());

    int y = 8;
    if (scores.empty()) {
        mvprintw(8, 10, "Nessun punteggio registrato!");
        //mvprintw(9, 10, "Il file potrebbe non esistere o essere vuoto");
    } else {
        for (size_t i = 0; i < scores.size(); ++i) {
            mvprintw(y++, 10, "%2zu. %-10s %5d", i+1, scores[i].name.c_str(), scores[i].score);
        }
    }
    
    refresh();
    mvprintw(y+2, 10, "Premi un tasto per tornare al menu...");
    getch();
}