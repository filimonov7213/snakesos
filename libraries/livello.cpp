//
// Created by victor on 20/07/2025.
//
#include "livello.h"

Livello::Livello(int num, int vel) {
    numero = num;
    velocita = vel;
    next = nullptr;
    prev = nullptr;
}

// Setters
void Livello::setNext(Livello* n) {
    this->next = n;
}

void Livello::setPrev(Livello* p) {
    this->prev = p;
}

// Getters
int Livello::getNumero() const {
    return numero;
}

int Livello::getVelocita() const {
    return velocita;
}

Livello* Livello::getNext() const {
    return next;
}

Livello* Livello::getPrev() const {
    return prev;
}

bool Livello::hasNext() const {
    return next != nullptr;
}

bool Livello::hasPrev() const {
    return prev != nullptr;
}