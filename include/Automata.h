// Copyright 2022 UNN-IASR
#ifndef INCLUDE_AUTOMATA_H_
#define INCLUDE_AUTOMATA_H_

#include <string>
#include <vector>
#include <ostream>

enum class STATES { OFF, WAIT, ACCEPT, CHECK, COOK };

class Automata {
 private:
    double cash;
    std::vector<std::string> menu;
    std::vector<double> prices;
    STATES state;

    bool check(int drink_index);
    void cook(int drink_index);
    void finish();

 public:
    Automata();
    explicit Automata(std::ostream& os);

    void on();
    void off();
    void coin(double amount);
    std::vector<std::string> getMenu();
    std::vector<double> getPrices();
    STATES getState();
    void choice(int drink_index);
    void cancel();
};

#endif  // INCLUDE_AUTOMATA_H_
