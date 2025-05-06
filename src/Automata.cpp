// Copyright 2022 UNN-IASR
#include "CoffeeMachine.h"

#include <chrono>
#include <iostream>
#include <thread>

CoffeeMachine::CoffeeMachine(std::ostream& os) : display{ os } {
    beverageList.push_back(Beverage{ "Espresso", 15 });
    beverageList.push_back(Beverage{ "Americano", 20 });
    beverageList.push_back(Beverage{ "Cappuccino", 35 });
    beverageList.push_back(Beverage{ "Latte", 50 });
    beverageList.push_back(Beverage{ "Macchiato", 65 });
}

CoffeeMachine::CoffeeMachine(std::ostream& os, std::vector<Beverage> customMenu)
    : beverageList{ customMenu }, display{ os } {}

bool CoffeeMachine::isOperational() const {
    return currentMode != MachineState::POWER_OFF;
}

void CoffeeMachine::displayMessage(const std::string& msg) const {
    display << msg << "\n";
}

void CoffeeMachine::activate() {
    if (currentMode == MachineState::POWER_OFF) {
        currentMode = MachineState::READY;
        displayMessage("Machine activated");
    }
}

void CoffeeMachine::deactivate() {
    if (isOperational()) {
        currentMode = MachineState::POWER_OFF;
        totalRevenue = 0;
        displayMessage("Machine deactivated");
    }
}

void CoffeeMachine::acceptPayment(double payment) {
    if (!isOperational()) return;

    if (currentMode == MachineState::READY || currentMode == MachineState::PAYMENT) {
        if (payment > 0) {
            customerDeposit += payment;
            currentMode = MachineState::PAYMENT;
            displayMessage("Payment accepted");
        }
        else {
            displayMessage("Invalid payment amount");
        }
    }
}

std::vector<Beverage> CoffeeMachine::showMenu() {
    if (!isOperational()) return {};

    for (const auto& item : beverageList) {
        display << item.title << ": " << item.cost << " credits\n";
    }
    return beverageList;
}

double CoffeeMachine::getDepositAmount() {
    if (!isOperational()) return 0;

    display << "Current deposit: " << customerDeposit << "\n";
    return customerDeposit;
}

MachineState CoffeeMachine::checkStatus() {
    switch (currentMode) {
    case MachineState::POWER_OFF:
        displayMessage("Status: POWER_OFF");
        break;
    case MachineState::READY:
        displayMessage("Status: READY");
        break;
    case MachineState::PAYMENT:
        displayMessage("Status: PAYMENT");
        break;
    case MachineState::VERIFICATION:
        displayMessage("Status: VERIFICATION");
        break;
    case MachineState::BREWING:
        displayMessage("Status: BREWING");
        break;
    }
    return currentMode;
}

std::pair<SelectionResult, double> CoffeeMachine::makeSelection(size_t index) {
    if (currentMode != MachineState::PAYMENT) {
        return handleSelectionOutcome({ SelectionResult::UNAVAILABLE, 0 });
    }

    if (index >= beverageList.size()) {
        return handleSelectionOutcome(
            { SelectionResult::INVALID_SELECTION, refundPayment() });
    }

    currentMode = MachineState::VERIFICATION;
    if (verifySelection(index)) {
        totalRevenue += beverageList[index].cost;
        customerDeposit -= beverageList[index].cost;
        double change = customerDeposit;
        brewBeverage();
        return handleSelectionOutcome({ SelectionResult::SUCCESS, change });
    }

    return handleSelectionOutcome(
        { SelectionResult::INSUFFICIENT_FUNDS, refundPayment() });
}

std::pair<SelectionResult, double> CoffeeMachine::handleSelectionOutcome(
    std::pair<SelectionResult, double> outcome) {
    switch (outcome.first) {
    case SelectionResult::SUCCESS:
        display << "Selection successful. Change: " << outcome.second << "\n";
        break;
    case SelectionResult::INVALID_SELECTION:
        display << "Invalid selection. Refund: " << outcome.second << "\n";
        break;
    case SelectionResult::INSUFFICIENT_FUNDS:
        display << "Insufficient funds. Refund: " << outcome.second << "\n";
        break;
    case SelectionResult::UNAVAILABLE:
        display << "Service unavailable\n";
        break;
    }
    return outcome;
}

bool CoffeeMachine::verifySelection(size_t index) {
    return currentMode == MachineState::VERIFICATION &&
        customerDeposit >= beverageList[index].cost;
}

double CoffeeMachine::refundPayment() {
    if (!isOperational()) return 0;

    double refund = customerDeposit;
    customerDeposit = 0;
    currentMode = MachineState::READY;
    displayMessage("Payment refunded");
    return refund;
}

void CoffeeMachine::brewBeverage() {
    if (currentMode != MachineState::VERIFICATION) return;

    currentMode = MachineState::BREWING;
    displayMessage("Preparing your beverage...");
    std::this_thread::sleep_for(std::chrono::seconds(5));
    resetAfterBrewing();
}

void CoffeeMachine::resetAfterBrewing() {
    if (currentMode == MachineState::BREWING) {
        customerDeposit = 0;
        currentMode = MachineState::READY;
        displayMessage("Beverage ready. Enjoy!");
    }
}