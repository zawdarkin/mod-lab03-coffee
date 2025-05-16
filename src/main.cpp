// Copyright 2023 BeverageTech
#include <iostream>
#include "Automata.h"

int main() {
    BeverageMachine* coffeeMachine = new BeverageMachine(std::cout);
    coffeeMachine->powerOn();
    coffeeMachine->showMenu();
    coffeeMachine->getCurrentBalance();
    coffeeMachine->insertMoney(20.0);
    coffeeMachine->getCurrentBalance();
    coffeeMachine->getCurrentStatus();
    coffeeMachine->selectBeverage(2);
    coffeeMachine->insertMoney(100.0);
    coffeeMachine->selectBeverage(2);
    delete coffeeMachine;
    return 0;
}
