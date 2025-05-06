// Copyright 2022 UNN-IASR
#ifndef INCLUDE_COFFEE_MACHINE_H_
#define INCLUDE_COFFEE_MACHINE_H_

#include <string>
#include <utility>
#include <vector>

enum class MachineState { POWER_OFF, READY, PAYMENT, VERIFICATION, BREWING };
enum class SelectionResult { SUCCESS, INVALID_SELECTION, INSUFFICIENT_FUNDS, UNAVAILABLE };

struct Beverage {
    std::string title;
    double cost;
};

class CoffeeMachine {
private:
    double totalRevenue = 0;
    double customerDeposit = 0;
    std::ostream& display;
    std::vector<Beverage> beverageList;
    MachineState currentMode = MachineState::POWER_OFF;

    bool verifySelection(size_t index);
    void brewBeverage();
    void resetAfterBrewing();
    std::pair<SelectionResult, double> handleSelectionOutcome(
        std::pair<SelectionResult, double> outcome);

    // Helper methods
    bool isOperational() const;
    void displayMessage(const std::string& msg) const;

public:
    explicit CoffeeMachine(std::ostream& os);
    CoffeeMachine(std::ostream& os, std::vector<Beverage> customMenu);

    void activate();
    void deactivate();
    void acceptPayment(double payment);
    std::vector<Beverage> showMenu();
    double getDepositAmount();
    MachineState checkStatus();
    std::pair<SelectionResult, double> makeSelection(size_t index);
    double refundPayment();
};

#endif  // INCLUDE_COFFEE_MACHINE_H_