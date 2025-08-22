#pragma once
#include "livello.h"
#include "snakegame.h"

class Game {
public:
    Game();
    void start(); // gestisce i livelli e avvia SnakeGame

private:
    Livello* head;   // primo livello
    Livello* tail;   // ultimo livello
};
