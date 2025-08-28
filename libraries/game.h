#pragma once
#include "livello.h"
#include "snakegame.h"
#include <string>

class Game {
public:
    Game();
    void start(); // gestisce i livelli e avvia SnakeGame

private:
    void askPlayerName(); // nuovo metodo per chiedere il nome

    Livello* head;   // primo livello
    Livello* tail;   // ultimo livello
    std::string playerName; // nome del giocatore
};