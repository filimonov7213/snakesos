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
    for (auto& e : scores) {
        if (e.name == name && e.score == score) {
            return; // già presente
        }
    }
    scores.push_back({name, score});
    std::sort(scores.begin(), scores.end(), [](auto& a, auto& b){
        return a.score > b.score;
    });
    if (scores.size() > 10) scores.resize(10);
    save();
}

void Leaderboard::show() const {
    // Salva lo stato corrente dell'input
    int old_flags = curs_set(0); // Nascondi cursore
    bool old_echo = (echo() == OK);
    if (old_echo) noecho();

    clear();
    refresh();

    // Mostra la classifica
    mvprintw(5, 10, "===== CLASSIFICA TOP 10 =====");

    int y = 7;
    if (scores.empty()) {
        mvprintw(y++, 10, "Nessun punteggio registrato!");
    } else {
        for (size_t i = 0; i < scores.size(); ++i) {
            mvprintw(y++, 10, "%2zu. %-15s %5d",
                    i+1, scores[i].name.c_str(), scores[i].score);
        }
    }

    mvprintw(y+2, 10, "Premi un tasto per tornare al menu...");
    refresh();

    // Pulisce il buffer di input e aspetta realmente
    flushinp(); // Pulisce il buffer di input
    timeout(-1); // Modalità bloccante
    getch();     // Aspetta un input reale
    timeout(0);  // Ritorna a non bloccante

    // Ripristina lo stato originale
    if (old_echo) echo();
    curs_set(0); // Mantieni cursore nascosto

    clear();
    refresh();
}