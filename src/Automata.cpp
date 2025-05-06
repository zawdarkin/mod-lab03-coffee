// Copyright 2022 UNN-IASR
#include "Automata.h"

#include <chrono>
#include <iostream>
#include <thread>
#include <vector>
#include <utility>
#include <string>

Automata::Automata(std::ostream& os) : stream{os} {
  menu.push_back(menuItem{"Espresso", 15});
  menu.push_back(menuItem{"Americano", 20});
  menu.push_back(menuItem{"Cappuccino", 35});
  menu.push_back(menuItem{"Latte", 50});
  menu.push_back(menuItem{"Macchiato", 65});
}

Automata::Automata(std::ostream& os, std::vector<menuItem> customMenu)
    : menu{customMenu}, stream{os} {}

void Automata::on() {
  if (state == STATES::OFF) state = STATES::WAIT;
}

void Automata::off() {
  if (state != STATES::OFF) {
    state = STATES::OFF;
    cash = 0;
  }
}

void Automata::coin(double amount) {
  if (state == STATES::WAIT || state == STATES::ACCEPT) {
    if (amount <= 0) {
      stream << "invalid amount value\n";
      return;
    }
    singleCash += amount;
    state = STATES::ACCEPT;
  }
}

std::vector<menuItem> Automata::getMenu() {
  if (state != STATES::OFF) {
    for (menuItem item : menu) {
      stream << item.name << ": " << item.price << "\n";
    }
    return menu;
  }
  return std::vector<menuItem>();
}

double Automata::getCashe() {
  if (state != STATES::OFF) {
    stream << "Amount contributed: " << singleCash << "\n";
    return singleCash;
  }
  return 0;
}

STATES Automata::getState() {
  switch (state) {
    case STATES::OFF:
      stream << "OFF\n";
      break;
    case STATES::WAIT:
      stream << "WAIT\n";
      break;
    case STATES::ACCEPT:
      stream << "ACCEPT\n";
      break;
    case STATES::CHECK:
      stream << "CHECK\n";
      break;
    case STATES::COOK:
      stream << "COOK\n";
      break;
    default:
      stream << "UNKNOWN\n";
      break;
  }
  return state;
}

std::pair<CHOISE_STATES, double> Automata::choice(size_t drinkIndex) {
  if (state == STATES::ACCEPT) {
    if (drinkIndex < menu.size()) {
      state = STATES::CHECK;
      if (check(drinkIndex)) {
        cash += menu[drinkIndex].price;
        singleCash -= menu[drinkIndex].price;
        double toReturn = singleCash;
        cook();
        return printChoiseState(std::pair(CHOISE_STATES::OK, toReturn));
      } else {
        return printChoiseState(
            std::pair(CHOISE_STATES::NOT_ENOUGHT_MONEY, cancel()));
      }
    } else {
      return printChoiseState(std::pair(CHOISE_STATES::INVALID_ITEM, cancel()));
    }
  }
  return printChoiseState(std::pair(CHOISE_STATES::INACCESSIBLE, 0));
}

std::pair<CHOISE_STATES, double> Automata::printChoiseState(
    std::pair<CHOISE_STATES, double> state) {
  switch (state.first) {
    case CHOISE_STATES::OK:
      stream << "OK, change: " << state.second << "\n";
      break;
    case CHOISE_STATES::INVALID_ITEM:
      stream << "INVALID_ITEM, change: " << state.second << "\n";
      break;
    case CHOISE_STATES::NOT_ENOUGHT_MONEY:
      stream << "NOT_ENOUGHT_MONEY, change: " << state.second << "\n";
      break;
    case CHOISE_STATES::INACCESSIBLE:
      stream << "INACCESSIBLE, change: " << state.second << "\n";
      break;
    default:
      stream << "UNKNOWN, change: 0\n";
      break;
  }
  return state;
}

bool Automata::check(size_t drinkIndex) {
  if (state == STATES::CHECK) return singleCash >= menu[drinkIndex].price;
  return false;
}

double Automata::cancel() {
  if (state != STATES::OFF) {
    double toReturn = singleCash;
    singleCash = 0;
    state = STATES::WAIT;
    return toReturn;
  }
  return 0;
}

void Automata::cook() {
  if (state == STATES::CHECK) {
    state = STATES::COOK;
    stream << "Start cooking\n";
    std::this_thread::sleep_for(std::chrono::seconds(5));
    finish();
  }
}

void Automata::finish() {
  if (state == STATES::COOK) {
    stream << "Finished\n";
    singleCash = 0;
    state = STATES::WAIT;
  }
}
