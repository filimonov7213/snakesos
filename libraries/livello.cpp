#include "livello.h"

Livello::Livello(int id, int speed, int h, int w)
    : levelId(id), speed(speed), height(h), width(w), next(nullptr), prev(nullptr) {}

int Livello::getSpeed() const { return speed; }
int Livello::getHeight() const { return height; }
int Livello::getWidth() const { return width; }

std::string Livello::getName() const {
    return "Livello " + std::to_string(levelId);
}
