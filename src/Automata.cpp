// Copyright 2022 UNN-IASR
#include "Automata.h"
#include <chrono>
#include <thread>

namespace BeverageMachine {
    BeverageVendor::BeverageVendor(std::ostream& os) : output{os} {
        drinkList.push_back({"Espresso", 15});
        drinkList.push_back({"Americano", 20});
        drinkList.push_back({"Cappuccino", 35});
        drinkList.push_back({"Latte", 50});
        drinkList.push_back({"Macchiato", 65});
    }

    BeverageVendor::BeverageVendor(std::ostream& os, std::vector<DrinkOption> customDrinks)
        : drinkList{customDrinks}, output{os} {}

    void BeverageVendor::powerOn() {
        if (currentMode == MachineState::POWER_OFF) currentMode = MachineState::READY;
    }

    void BeverageVendor::powerOff() {
        if (currentMode != MachineState::POWER_OFF) {
            currentMode = MachineState::POWER_OFF;
            totalDeposited = 0;
        }
    }

    void BeverageVendor::insertMoney(double value) {
        if (currentMode == MachineState::READY || currentMode == MachineState::PAYMENT) {
            if (value <= 0) {
                output << "invalid amount value\n";
                return;
            }
            currentPayment += value;
            currentMode = MachineState::PAYMENT;
        }
    }

    std::vector<DrinkOption> BeverageVendor::showMenu() {
        if (currentMode != MachineState::POWER_OFF) {
            for (DrinkOption item : drinkList) {
                output << item.title << ": " << item.cost << "\n";
            }
            return drinkList;
        }
        return std::vector<DrinkOption>();
    }

    double BeverageVendor::getCurrentBalance() {
        if (currentMode != MachineState::POWER_OFF) {
            output << "Amount contributed: " << currentPayment << "\n";
            return currentPayment;
        }
        return 0;
    }

    MachineState BeverageVendor::getCurrentState() {
        switch (currentMode) {
            case MachineState::POWER_OFF: output << "OFF\n"; break;
            case MachineState::READY: output << "WAIT\n"; break;
            case MachineState::PAYMENT: output << "ACCEPT\n"; break;
            case MachineState::VERIFICATION: output << "CHECK\n"; break;
            case MachineState::BREWING: output << "COOK\n"; break;
            default: output << "UNKNOWN\n"; break;
        }
        return currentMode;
    }

    std::pair<SelectionStatus, double> BeverageVendor::makeSelection(size_t drinkIndex) {
        if (currentMode == MachineState::PAYMENT) {
            if (drinkIndex < drinkList.size()) {
                currentMode = MachineState::VERIFICATION;
                if (verifyPurchase(drinkIndex)) {
                    totalDeposited += drinkList[drinkIndex].cost;
                    currentPayment -= drinkList[drinkIndex].cost;
                    double change = currentPayment;
                    startBrewing();
                    return displaySelectionResult(std::pair(SelectionStatus::SUCCESS, change));
                } else {
                    return displaySelectionResult(
                        std::pair(SelectionStatus::INSUFFICIENT_FUNDS, abortTransaction()));
                }
            } else {
                return displaySelectionResult(std::pair(SelectionStatus::INVALID_SELECTION, abortTransaction()));
            }
        }
        return displaySelectionResult(std::pair(SelectionStatus::UNAVAILABLE, 0));
    }

    std::pair<SelectionStatus, double> BeverageVendor::displaySelectionResult(
        std::pair<SelectionStatus, double> result) {
        switch (result.first) {
            case SelectionStatus::SUCCESS:
                output << "OK, change: " << result.second << "\n"; break;
            case SelectionStatus::INVALID_SELECTION:
                output << "INVALID_ITEM, change: " << result.second << "\n"; break;
            case SelectionStatus::INSUFFICIENT_FUNDS:
                output << "NOT_ENOUGHT_MONEY, change: " << result.second << "\n"; break;
            case SelectionStatus::UNAVAILABLE:
                output << "INACCESSIBLE, change: " << result.second << "\n"; break;
            default:
                output << "UNKNOWN, change: 0\n"; break;
        }
        return result;
    }

    bool BeverageVendor::verifyPurchase(size_t selection) {
        if (currentMode == MachineState::VERIFICATION) {
            return currentPayment >= drinkList[selection].cost;
        }
        return false;
    }

    double BeverageVendor::abortTransaction() {
        if (currentMode != MachineState::POWER_OFF) {
            double refund = currentPayment;
            currentPayment = 0;
            currentMode = MachineState::READY;
            return refund;
        }
        return 0;
    }

    void BeverageVendor::startBrewing() {
        if (currentMode == MachineState::VERIFICATION) {
            currentMode = MachineState::BREWING;
            output << "Start cooking\n";
            std::this_thread::sleep_for(std::chrono::seconds(5));
            completeTransaction();
        }
    }

    void BeverageVendor::completeTransaction() {
        if (currentMode == MachineState::BREWING) {
            output << "Finished\n";
            currentPayment = 0;
            currentMode = MachineState::READY;
        }
    }
}
