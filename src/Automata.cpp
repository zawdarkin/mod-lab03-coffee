// Copyright 2022 UNN-IASR
#include "Automata.h"

#include <chrono>
#include <iostream>
#include <thread>
#include <vector>
#include <utility>
#include <string>

Automata::Automata(std::ostream& os) : outputStream{os} {
    drinkMenu.push_back(DrinkItem{"Espresso", 15});
    drinkMenu.push_back(DrinkItem{"Americano", 20});
    drinkMenu.push_back(DrinkItem{"Cappuccino", 35});
    drinkMenu.push_back(DrinkItem{"Latte", 50});
    drinkMenu.push_back(DrinkItem{"Macchiato", 65});
}

Automata::Automata(std::ostream& os, std::vector<DrinkItem> customMenu)
    : drinkMenu{customMenu}, outputStream{os} {}

void Automata::on() {
    if (currentMode == STATES::OFF) currentMode = STATES::WAIT;
}

void Automata::off() {
    if (currentMode != STATES::OFF) {
        currentMode = STATES::OFF;
        totalRevenue = 0;
    }
}

void Automata::coin(double amount) {
    if (currentMode == STATES::WAIT || currentMode == STATES::ACCEPT) {
        if (amount <= 0) {
            outputStream << "invalid amount value\n";
            return;
        }
        currentDeposit += amount;
        currentMode = STATES::ACCEPT;
    }
}

std::vector<DrinkItem> Automata::getMenu() {
    if (currentMode != STATES::OFF) {
        for (DrinkItem item : drinkMenu) {
            outputStream << item.drinkName << ": " << item.cost << "\n";
        }
        return drinkMenu;
    }
    return std::vector<DrinkItem>();
}

double Automata::getCashe() {
    if (currentMode != STATES::OFF) {
        outputStream << "Amount contributed: " << currentDeposit << "\n";
        return currentDeposit;
    }
    return 0;
}

STATES Automata::getState() {
    switch (currentMode) {
        case STATES::OFF:
            outputStream << "OFF\n";
            break;
        case STATES::WAIT:
            outputStream << "WAIT\n";
            break;
        case STATES::ACCEPT:
            outputStream << "ACCEPT\n";
            break;
        case STATES::CHECK:
            outputStream << "CHECK\n";
            break;
        case STATES::COOK:
            outputStream << "COOK\n";
            break;
        default:
            outputStream << "UNKNOWN\n";
            break;
    }
    return currentMode;
}

std::pair<CHOISE_STATES, double> Automata::choice(size_t drinkIndex) {
    if (currentMode == STATES::ACCEPT) {
        if (drinkIndex < drinkMenu.size()) {
            currentMode = STATES::CHECK;
            if (validateOrder(drinkIndex)) {
                totalRevenue += drinkMenu[drinkIndex].cost;
                currentDeposit -= drinkMenu[drinkIndex].cost;
                double change = currentDeposit;
                prepareDrink();
                return processChoiceResult(
                    std::pair(CHOISE_STATES::OK, change));
            } else {
                return processChoiceResult(std::pair(
                    CHOISE_STATES::NOT_ENOUGHT_MONEY, cancel()));
            }
        } else {
            return processChoiceResult(
                std::pair(CHOISE_STATES::INVALID_ITEM, cancel()));
        }
    }
    return processChoiceResult(std::pair(CHOISE_STATES::INACCESSIBLE, 0));
}

std::pair<CHOISE_STATES, double> Automata::processChoiceResult(
    std::pair<CHOISE_STATES, double> result) {
    switch (result.first) {
        case CHOISE_STATES::OK:
            outputStream << "OK, change: " << result.second << "\n";
            break;
        case CHOISE_STATES::INVALID_ITEM:
            outputStream << "INVALID_ITEM, change: " << result.second << "\n";
            break;
        case CHOISE_STATES::NOT_ENOUGHT_MONEY:
            outputStream << "NOT_ENOUGHT_MONEY, change: " 
                         << result.second << "\n";
            break;
        case CHOISE_STATES::INACCESSIBLE:
            outputStream << "INACCESSIBLE, change: " << result.second << "\n";
            break;
        default:
            outputStream << "UNKNOWN, change: 0\n";
            break;
    }
    return result;
}

bool Automata::validateOrder(size_t drinkIndex) {
    if (currentMode == STATES::CHECK)
        return currentDeposit >= drinkMenu[drinkIndex].cost;
    return false;
}

double Automata::cancel() {
    if (currentMode != STATES::OFF) {
        double refund = currentDeposit;
        currentDeposit = 0;
        currentMode = STATES::WAIT;
        return refund;
    }
    return 0;
}

void Automata::prepareDrink() {
    if (currentMode == STATES::CHECK) {
        currentMode = STATES::COOK;
        outputStream << "Start cooking\n";
        std::this_thread::sleep_for(std::chrono::seconds(5));
        completeService();
    }
}

void Automata::completeService() {
    if (currentMode == STATES::COOK) {
        outputStream << "Finished\n";
        currentDeposit = 0;
        currentMode = STATES::WAIT;
    }
}
