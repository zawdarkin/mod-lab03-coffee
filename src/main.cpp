// Copyright 2023 BeverageTech
#include <iostream>
#include "Automata.h"

int main() {
    Automata* coffeeMachine = new Automata(std::cout);
    coffeeMachine->on();
    coffeeMachine->getMenu();
    coffeeMachine->getCashe();
    coffeeMachine->coin(20.0);
    coffeeMachine->getCashe();
    coffeeMachine->getState();
    coffeeMachine->choice(2);
    coffeeMachine->coin(100.0);
    coffeeMachine->choice(2);
    delete coffeeMachine;
    return 0;
}
