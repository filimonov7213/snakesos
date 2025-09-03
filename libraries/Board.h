#pragma once
#include <curses.h>
#include "Drawable.h"

class Board {
public:
    Board();

    Board(int height, int width);

    // metodo descrittivo: aggiunge un bordo alla finestra
    void addBorder();

    // metodo descrittivo: sposta il cursore in una posizione (y, x) della finestra e inserisce un carattere
    void addAt(int y, int x, chtype ch);

    // restituisce il tasto o il carattere letto dalla finestra ed esegue un refresh libero
    chtype getInput();

    // questa funzione cicla finché non trova un carattere vuoto in una posizione casuale (y, x)
    // quando termina, salva il valore trovato in y e x
    void getEmptyCoordinates(int& y, int& x);

    // esegue clear() e refresh()
    void initialize();

    // pulisce l’intera finestra ed esegue addBorder()
    void clear();

    // metodo descrittivo: aggiorna la finestra
    void refresh();

    // utilizza la classe Drawable per aggiungere "icone" alla finestra. Es. add(apple) o add(snake)
    void add(Drawable drawable);

    // disegna un bordo attorno alla finestra
    void drawBorder();

    // restituisce il puntatore alla finestra
    WINDOW* getWindow() const { return board_win; }

    // per l’implementazione della funzione PAUSA
    void setTimeout(int timeout);

    // restituisce l’altezza della finestra
    const int getHeight();

    // restituisce la larghezza della finestra
    const int getWidth();

    // restituisce il carattere presente nella posizione (y, x)
    chtype getCharAt(int y, int x);

private:
    WINDOW *board_win;
    int height, width;

    // crea una finestra, usata per implementare il costruttore di default
    void construct(int height, int width);
};
