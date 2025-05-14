// Copyright 2022 UNN-IASR
#ifndef VENDING_MACHINE_H
#define VENDING_MACHINE_H

#include <string>
#include <utility>
#include <vector>
#include <ostream>

namespace BeverageMachine {
    enum class MachineState { POWER_OFF, READY, PAYMENT, VERIFICATION, BREWING };
    enum class SelectionStatus { SUCCESS, INVALID_SELECTION, INSUFFICIENT_FUNDS, UNAVAILABLE };

    struct DrinkOption {
        std::string title;
        double cost;
    };

    class BeverageVendor {
    private:
        double totalDeposited = 0;
        double currentPayment = 0;
        std::ostream& output;
        std::vector<DrinkOption> drinkList;
        MachineState currentMode = MachineState::POWER_OFF;

        bool verifyPurchase(size_t selection);
        void startBrewing();
        void completeTransaction();
        std::pair<SelectionStatus, double> displaySelectionResult(
            std::pair<SelectionStatus, double> result);

    public:
        explicit BeverageVendor(std::ostream& os);
        BeverageVendor(std::ostream& os, std::vector<DrinkOption> customDrinks);

        void powerOn();
        void powerOff();
        void insertMoney(double value);
        std::vector<DrinkOption> showMenu();
        double getCurrentBalance();
        MachineState getCurrentState();
        std::pair<SelectionStatus, double> makeSelection(size_t drinkIndex);
        double abortTransaction();
    };
}

#endif // INCLUDE_AUTOMATA_H_
