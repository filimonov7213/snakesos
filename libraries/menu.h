//
// Created by victor on 17/07/2025.
//

#ifndef MENU_H
#define MENU_H

#include <string>
#include <iostream>

class Menu {
private:
    static const int N_OPTIONS = 2;
    string options[N_OPTIONS];
    int selectedIndex;

public:
    Menu();
    void display() const;
    int gestisciInput();
};



#endif //MENU_H
