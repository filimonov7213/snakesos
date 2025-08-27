#pragma once
#include <string>

class Livello {
public:
    Livello(int id, int speed, int h, int w, int timeLimit);

    // getter
    int getSpeed() const;
    int getHeight() const;
    int getWidth() const;
    int getTimeLimit() const;
    std::string getName() const;

    // punteggio
    void addScore(int points);
    int getScore() const;

    // collegamenti lista
    Livello* next;
    Livello* prev;

private:
    int levelId;
    int speed;       // timeout in ms
    int height;
    int width;
    int timeLimit;   // in secondi
    int score;       // punti accumulati
};
