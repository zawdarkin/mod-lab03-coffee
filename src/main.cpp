// Copyright 2022 UNN-IASR
#include <iostream>

#include "Automata.h"

int main() {
  Automata* automata = new Automata(std::cout);
  automata->on();
  automata->getMenu();
  automata->getCashe();
  automata->coin(20);
  automata->getCashe();
  automata->getState();
  automata->choice(2);
  automata->coin(100);
  automata->choice(2);
  return 0;
}
