#include "Livello.h"

Livello::Livello(int id, int speed, int h, int w, int timeLimit, int appleFactor)
    : levelId(id), speed(speed), height(h), width(w),
      timeLimit(timeLimit), appleFactor(appleFactor), score(0), next(nullptr), prev(nullptr) {}

int Livello::getSpeed() const { return speed; }
int Livello::getHeight() const { return height; }
int Livello::getWidth() const { return width; }
int Livello::getTimeLimit() const { return timeLimit; }
int Livello::getAppleFactor() const { return appleFactor; }

std::string Livello::getName() const { return "Livello " + std::to_string(levelId); }

void Livello::addScore(int points) {
    score += points;
}

int Livello::getScore() const {
    return score;
}
