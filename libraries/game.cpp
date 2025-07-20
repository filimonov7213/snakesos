//
// Created by victor on 20/07/2025.
//

#include "game.h"
#include <curses.h>

Game::Game() {
    grid = new Grid(20, 10);
    snake = new Snake();
    livelloCorrente = new Livello(1); // inizialmente livello 1
    punteggio = 0;
    running = true;
}

Game::~Game() {
    delete grid;
    delete snake;
    delete livelloCorrente;
}

void Game::start() {
    // imposta ncurses (simile a main)
    clear();
    curs_set(0);
    keypad(stdscr, TRUE);

    // Ottieni la velocità dal livello corrente
    int velocitaMs = livelloCorrente->getVelocita();
    timeout(velocitaMs);

    loop();
}

void Game::loop() {
    while (true) {
        int ch = getch();
        handleInput(ch);
        update();
        render();
        usleep(100000); // può cambiare con il livello
    }

    // dopo il gioco: salva punteggio o ritorna al menu
}

void Game::handleInput(int ch) {
    switch (ch) {
        case KEY_UP: snake->changeDirection(0, -1); break;
        case KEY_DOWN: snake->changeDirection(0, 1); break;
        case KEY_LEFT: snake->changeDirection(-1, 0); break;
        case KEY_RIGHT: snake->changeDirection(1, 0); break;
        case 'q': running = false; break;
        case 'n': // avanza di livello
            if (livelloCorrente->hasNext()) {
                livelloCorrente = livelloCorrente->getNext();
                timeout(livelloCorrente->getVelocita());
            }
        break;

        case 'b': // torna indietro di livello
            if (livelloCorrente->hasPrev()) {
                livelloCorrente = livelloCorrente->getPrev();
                timeout(livelloCorrente->getVelocita());
            }
        break;
    }
}

void Game::update() {
    snake->move();

    // TODO: collisioni, mele, punteggio

    // TODO: aggiorna punteggio, cambia livello se serve
    if (/* condizione */) {
        // livello successivo
        if (livelloCorrente->getNext() != nullptr) {
            livelloCorrente = livelloCorrente->getNext();
            timeout(livelloCorrente->getVelocita());
        }
        timeout(livelloCorrente->getVelocita());      // aggiorna velocità
    }
}

void Game::render() {
    clear();
    grid->draw();
    snake->draw();
    // TODO: disegna mela, punteggio, ecc.
    mvprintw(0, 0, "Livello: %d", livelloCorrente->getNumero());
    refresh();
}