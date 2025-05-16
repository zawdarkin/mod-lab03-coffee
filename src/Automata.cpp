// Copyright 2022 UNN-IASR
#include "Automata.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <string>
#include <vector>

Automata::Automata() : cash(0), state(STATES::OFF) {
    menu = {"Espresso", "Americano", "Cappuccino", "Latte", "Macchiato"};
    prices = {15, 20, 35, 50, 65};
}

Automata::Automata(std::ostream& os) : cash(0), state(STATES::OFF) {
    menu = {"Espresso", "Americano", "Cappuccino", "Latte", "Macchiato"};
    prices = {15, 20, 35, 50, 65};
    os << "Automata initialized\n";
}

void Automata::on() {
    if (state == STATES::OFF) {
        state = STATES::WAIT;
    }
}

void Automata::off() {
    if (state != STATES::OFF) {
        state = STATES::OFF;
        cash = 0;
    }
}

void Automata::coin(double amount) {
    if (state == STATES::WAIT || state == STATES::ACCEPT) {
        cash += amount;
        state = STATES::ACCEPT;
    }
}

std::vector<std::string> Automata::getMenu() {
    return menu;
}

std::vector<double> Automata::getPrices() {
    return prices;
}

STATES Automata::getState() {
    return state;
}

bool Automata::check(int drink_index) {
    if (state == STATES::ACCEPT && drink_index >= 0 &&
        drink_index < static_cast<int>(menu.size()) && cash >= prices[drink_index]) {
        state = STATES::CHECK;
        return true;
    }
    return false;
}

void Automata::choice(int drink_index) {
    if (check(drink_index)) {
        cook(drink_index);
    }
}

void Automata::cook(int drink_index) {
    if (state == STATES::CHECK) {
        state = STATES::COOK;
        cash -= prices[drink_index];
        std::this_thread::sleep_for(std::chrono::seconds(2));
        finish();
    }
}

void Automata::finish() {
    if (state == STATES::COOK) {
        state = STATES::WAIT;
    }
}

void Automata::cancel() {
    if (state == STATES::ACCEPT || state == STATES::CHECK) {
        cash = 0;
        state = STATES::WAIT;
    }
}
