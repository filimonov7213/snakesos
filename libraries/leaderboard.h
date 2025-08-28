#pragma once
#include <string>
#include <vector>

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
    std::vector<ScoreEntry> scores;
};