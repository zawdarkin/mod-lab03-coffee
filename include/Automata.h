// Copyright 2023 BeverageTech
#ifndef BEVERAGE_AUTOMATA_H_
#define BEVERAGE_AUTOMATA_H_

#include <string>
#include <vector>
#include <utility>
#include <ostream>

enum class MachineState { 
    POWER_OFF, 
    IDLE, 
    PAYMENT_ACCEPTING, 
    ORDER_VERIFICATION, 
    BEVERAGE_PREPARATION 
};

enum class OrderStatus { 
    SUCCESS, 
    INVALID_SELECTION, 
    INSUFFICIENT_FUNDS, 
    UNAVAILABLE 
};

struct BeverageOption {
    std::string name;
    double price;
};

class BeverageMachine {
 private:
    double totalSales;
    double currentBalance;
    std::ostream& output;
    std::vector<BeverageOption> beverageOptions;
    MachineState currentStatus;

    bool verifyOrder(size_t itemIndex);
    void startPreparation();
    void finalizeTransaction();
    std::pair<OrderStatus, double> processOrderResult(
        std::pair<OrderStatus, double> result);

 public:
    explicit BeverageMachine(std::ostream& os);
    BeverageMachine(std::ostream& os, std::vector<BeverageOption> customOptions);

    void powerOn();
    void powerOff();
    void insertMoney(double amount);
    std::vector<BeverageOption> showMenu();
    double getCurrentBalance();
    MachineState getCurrentStatus();
    std::pair<OrderStatus, double> selectBeverage(size_t beverageIndex);
    double cancelTransaction();
};

#endif  // BEVERAGE_AUTOMATA_H_
