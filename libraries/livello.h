//
// Created by victor on 20/07/2025.
//

#ifndef LIVELLO_H
#define LIVELLO_H

#include <iostream>

class Livello {
 private:
    int numero;           // Numero del livello (1, 2, 3...)
    int velocita;         // Velocità in millisecondi per timeout()
    Livello* next;        // Puntatore al livello successivo
    Livello* prev;        // Puntatore al livello precedente

 public:
    // Costruttore
    Livello(int num, int vel);

    // Setters per collegare la lista
    void setNext(Livello* n);
    void setPrev(Livello* p);

    // Getters
    int getNumero() const;
    int getVelocita() const;
    Livello* getNext() const;
    Livello* getPrev() const;
    bool hasNext() const;
    bool hasPrev() const;
};

#endif //LIVELLO_H
