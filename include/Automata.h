// Copyright 2022 UNN-IASR
#ifndef INCLUDE_AUTOMATA_H_  // NOLINT
#define INCLUDE_AUTOMATA_H_

#include <string>
#include <utility>
#include <vector>

enum class STATES { OFF, WAIT, ACCEPT, CHECK, COOK };
enum class CHOISE_STATES { OK, INVALID_ITEM, NOT_ENOUGHT_MONEY, INACCESSIBLE };

struct menuItem {
  std::string name;
  double price;
};

class Automata {
 private:
  double cash = 0;
  double singleCash = 0;
  std::ostream& stream;
  std::vector<menuItem> menu;
  STATES state = STATES::OFF;

  bool check(size_t drinkIndex);
  void cook();
  void finish();
  std::pair<CHOISE_STATES, double> printChoiseState(
      std::pair<CHOISE_STATES, double> state);

 public:
  explicit Automata(std::ostream& os);
  Automata(std::ostream& os, std::vector<menuItem> customMenu);

  void on();
  void off();
  void coin(double amount);
  std::vector<menuItem> getMenu();
  double getCashe();
  STATES getState();
  std::pair<CHOISE_STATES, double> choice(size_t drinkIndex);
  double cancel();
};

#endif  // INCLUDE_AUTOMATA_H_
