#pragma once
#include <string>

struct ScoreEntry {
    std::string name;
    int score;
};

class Leaderboard {
public:
    Leaderboard(const std::string& filename);
    void load();
    void save() const;
    void addScore(const std::string& name, int score);
    void show() const;

private:
    std::string file;
    ScoreEntry scores[10]; // Array fisso di 10 elementi
    int scoreCount;        // Numero effettivo di punteggi
};