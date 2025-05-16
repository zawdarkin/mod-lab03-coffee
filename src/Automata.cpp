// Copyright 2023 BeverageTech
#include "Automata.h"

#include <chrono>
#include <thread>

BeverageMachine::BeverageMachine(std::ostream& os) : output{os} {
    beverageOptions.push_back({"Espresso", 15.0});
    beverageOptions.push_back({"Americano", 20.0});
    beverageOptions.push_back({"Cappuccino", 35.0});
    beverageOptions.push_back({"Latte", 50.0});
    beverageOptions.push_back({"Macchiato", 65.0});
}

BeverageMachine::BeverageMachine(std::ostream& os, 
    std::vector<BeverageOption> customOptions) : beverageOptions{customOptions}, output{os} {}

void BeverageMachine::powerOn() {
    if (currentStatus == MachineState::POWER_OFF) {
        currentStatus = MachineState::IDLE;
    }
}

void BeverageMachine::powerOff() {
    if (currentStatus != MachineState::POWER_OFF) {
        currentStatus = MachineState::POWER_OFF;
        totalSales = 0;
    }
}

void BeverageMachine::insertMoney(double amount) {
    if (currentStatus == MachineState::IDLE || 
        currentStatus == MachineState::PAYMENT_ACCEPTING) {
        if (amount <= 0) {
            output << "Invalid amount entered\n";
            return;
        }
        currentBalance += amount;
        currentStatus = MachineState::PAYMENT_ACCEPTING;
    }
}

std::vector<BeverageOption> BeverageMachine::showMenu() {
    if (currentStatus != MachineState::POWER_OFF) {
        for (const auto& option : beverageOptions) {
            output << option.name << ": " << option.price << "\n";
        }
        return beverageOptions;
    }
    return std::vector<BeverageOption>();
}

double BeverageMachine::getCurrentBalance() {
    if (currentStatus != MachineState::POWER_OFF) {
        output << "Current balance: " << currentBalance << "\n";
        return currentBalance;
    }
    return 0;
}

MachineState BeverageMachine::getCurrentStatus() {
    switch (currentStatus) {
        case MachineState::POWER_OFF:
            output << "POWER_OFF\n";
            break;
        case MachineState::IDLE:
            output << "IDLE\n";
            break;
        case MachineState::PAYMENT_ACCEPTING:
            output << "PAYMENT_ACCEPTING\n";
            break;
        case MachineState::ORDER_VERIFICATION:
            output << "ORDER_VERIFICATION\n";
            break;
        case MachineState::BEVERAGE_PREPARATION:
            output << "BEVERAGE_PREPARATION\n";
            break;
        default:
            output << "UNKNOWN_STATUS\n";
            break;
    }
    return currentStatus;
}

std::pair<OrderStatus, double> BeverageMachine::selectBeverage(size_t beverageIndex) {
    if (currentStatus == MachineState::PAYMENT_ACCEPTING) {
        if (beverageIndex < beverageOptions.size()) {
            currentStatus = MachineState::ORDER_VERIFICATION;
            if (verifyOrder(beverageIndex)) {
                totalSales += beverageOptions[beverageIndex].price;
                currentBalance -= beverageOptions[beverageIndex].price;
                double change = currentBalance;
                startPreparation();
                return processOrderResult(
                    std::pair(OrderStatus::SUCCESS, change));
            } else {
                return processOrderResult(std::pair(
                    OrderStatus::INSUFFICIENT_FUNDS, cancelTransaction()));
            }
        } else {
            return processOrderResult(
                std::pair(OrderStatus::INVALID_SELECTION, cancelTransaction()));
        }
    }
    return processOrderResult(std::pair(OrderStatus::UNAVAILABLE, 0));
}

std::pair<OrderStatus, double> BeverageMachine::processOrderResult(
    std::pair<OrderStatus, double> result) {
    switch (result.first) {
        case OrderStatus::SUCCESS:
            output << "Order successful, change: " << result.second << "\n";
            break;
        case OrderStatus::INVALID_SELECTION:
            output << "Invalid selection, change: " << result.second << "\n";
            break;
        case OrderStatus::INSUFFICIENT_FUNDS:
            output << "Insufficient funds, change: " << result.second << "\n";
            break;
        case OrderStatus::UNAVAILABLE:
            output << "Service unavailable, change: " << result.second << "\n";
            break;
        default:
            output << "Unknown error, change: 0\n";
            break;
    }
    return result;
}

bool BeverageMachine::verifyOrder(size_t beverageIndex) {
    if (currentStatus == MachineState::ORDER_VERIFICATION)
        return currentBalance >= beverageOptions[beverageIndex].price;
    return false;
}

double BeverageMachine::cancelTransaction() {
    if (currentStatus != MachineState::POWER_OFF) {
        double refundAmount = currentBalance;
        currentBalance = 0;
        currentStatus = MachineState::IDLE;
        return refundAmount;
    }
    return 0;
}

void BeverageMachine::startPreparation() {
    if (currentStatus == MachineState::ORDER_VERIFICATION) {
        currentStatus = MachineState::BEVERAGE_PREPARATION;
        output << "Preparing your beverage...\n";
        std::this_thread::sleep_for(std::chrono::seconds(5));
        finalizeTransaction();
    }
}

void BeverageMachine::finalizeTransaction() {
    if (currentStatus == MachineState::BEVERAGE_PREPARATION) {
        output << "Order complete\n";
        currentBalance = 0;
        currentStatus = MachineState::IDLE;
    }
}
