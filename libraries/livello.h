#pragma once
#include <string>

class Livello {
public:
    Livello(int id, int speed, int h, int w, int timeLimit, int appleFactor);

    // getter
    int getSpeed() const;
    int getHeight() const;
    int getWidth() const;
    int getTimeLimit() const;
    int getAppleFactor() const;
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
    int appleFactor; // moltiplicatore punti per mela
};
