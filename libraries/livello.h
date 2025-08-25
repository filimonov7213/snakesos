#pragma once
#include <string>

class Livello {
public:
    Livello(int id, int speed, int h, int w);

    // getter
    int getId() const;
    int getSpeed() const;
    int getHeight() const;
    int getWidth() const;
    std::string getName() const;

    // collegamenti lista
    Livello* next;
    Livello* prev;

private:
    int levelId;
    int speed;   // timeout in ms
    int height;
    int width;
};
