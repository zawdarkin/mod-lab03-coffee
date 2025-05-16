// Copyright 2022 UNN-IASR
#ifndef INCLUDE_AUTOMATA_H_
#define INCLUDE_AUTOMATA_H_

#include <string>
#include <utility>
#include <vector>

enum class STATES {
    OFF,
    WAIT,
    ACCEPT,
    CHECK,
    COOK
};

enum class CHOISE_STATES {
    OK,
    INVALID_ITEM,
    NOT_ENOUGHT_MONEY,
    INACCESSIBLE
};

struct DrinkItem {
    std::string drinkName;
    double cost;
};

class Automata {
 private:
    double totalRevenue = 0;
    double currentDeposit = 0;
    std::ostream& outputStream;
    std::vector<DrinkItem> drinkMenu;
    STATES currentMode = STATES::OFF;

    bool validateOrder(size_t itemIndex);
    void prepareDrink();
    void completeService();
    std::pair<CHOISE_STATES, double> processChoiceResult(
        std::pair<CHOISE_STATES, double> result);

 public:
    explicit Automata(std::ostream& os);
    Automata(std::ostream& os, std::vector<DrinkItem> customMenu);

    void on();
    void off();
    void coin(double amount);
    std::vector<DrinkItem> getMenu();
    double getCashe();
    STATES getState();
    std::pair<CHOISE_STATES, double> choice(size_t drinkIndex);
    double cancel();
};

#endif  // INCLUDE_AUTOMATA_H_
