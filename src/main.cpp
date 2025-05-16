// Copyright 2022 UNN-IASR
#include <iostream>
#include "Automata.h"

int main() {
    Automata automata(std::cout);
    
    automata.on();
    automata.coin(50);
    automata.choice(2);  // Выбираем Cappuccino
    automata.coin(10);
    automata.cancel();
    automata.off();

    return 0;
}
