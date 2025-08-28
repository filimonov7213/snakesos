//
// Created by victor on 28/08/2025.
//
#include "leaderboard.h"
#include <fstream>
#include <algorithm>
#include <curses.h>

Leaderboard::Leaderboard(const std::string& filename) : file(filename) {
    load();
}

void Leaderboard::load() {
    scores.clear();
    std::ifstream in(file);
    if (!in.is_open()) return;

    ScoreEntry entry;
    while (in >> entry.name >> entry.score) {
        scores.push_back(entry);
    }
    in.close();

    // ordina decrescente per punteggio
    std::sort(scores.begin(), scores.end(), [](auto& a, auto& b){
        return a.score > b.score;
    });

    // tieni solo i primi 10
    if (scores.size() > 10) scores.resize(10);
}

void Leaderboard::save() const {
    std::ofstream out(file, std::ios::trunc);
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

    int y = 7;
    for (size_t i = 0; i < scores.size(); ++i) {
        mvprintw(y++, 10, "%2zu. %-10s %5d", i+1, scores[i].name.c_str(), scores[i].score);
    }
    refresh();
    mvprintw(y+2, 10, "Premi un tasto per tornare al menu...");
    getch();
}
