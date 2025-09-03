#pragma once
#include "Livello.h"
#include "Snakegame.h"
#include <string>

class Game {
public:
    Game();
    void start(); // gestisce i livelli e avvia SnakeGame

private:
    void askPlayerName(); // nuovo metodo per chiedere il nome
    void displayLevelCompleteScreen(Livello* current, int levelScore, int totalScore, bool isFinalLevel);

    Livello* head;   // primo livello
    Livello* tail;   // ultimo livello
    std::string playerName; // nome del giocatore
};