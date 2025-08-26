#pragma once
#include "livello.h"
#include "snakegame.h"
#include <vector>
class Game {
public:
    Game();
    void start(); // gestisce i livelli e avvia SnakeGame
    void classifica(); // mostra la classifica top 5
private:
    int scores[5];
    static const int SCORES_SIZE = 5;
    Livello* head;   // primo livello
    Livello* tail;   // ultimo livello
};
