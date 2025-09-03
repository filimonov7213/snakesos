#ifndef MENU_H
#define MENU_H

#include <string>
#include <iostream>

class Menu {
private:
    static const int N_OPTIONS = 2;
    std::string options[N_OPTIONS];
    int selectedIndex;

public:
    Menu();
    void display() const;
    int gestisciInput();
    int show();
};



#endif //MENU_H
