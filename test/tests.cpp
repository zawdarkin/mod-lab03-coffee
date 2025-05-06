// Copyright 2022 GHA Test Team
#include <gtest/gtest.h>
#include <sstream>
#include "CoffeeMachine.h"

TEST(CoffeeMachineTests, InitialStateCheck) {
    std::stringstream output;
    CoffeeMachine machine(output);
    EXPECT_EQ(machine.checkStatus(), MachineState::POWER_OFF);
    EXPECT_TRUE(output.str().find("POWER_OFF") != std::string::npos);
}

TEST(CoffeeMachineTests, SuccessfulActivation) {
    std::stringstream output;
    CoffeeMachine machine(output);
    machine.activate();
    EXPECT_EQ(machine.checkStatus(), MachineState::READY);
    EXPECT_TRUE(output.str().find("activated") != std::string::npos);
}

TEST(CoffeeMachineTests, DoubleActivation) {
    std::stringstream output;
    CoffeeMachine machine(output);
    machine.activate();
    machine.activate(); // Повторная активация
    EXPECT_EQ(machine.checkStatus(), MachineState::READY);
}

TEST(CoffeeMachineTests, Deactivation) {
    std::stringstream output;
    CoffeeMachine machine(output);
    machine.activate();
    machine.deactivate();
    EXPECT_EQ(machine.checkStatus(), MachineState::POWER_OFF);
    EXPECT_TRUE(output.str().find("deactivated") != std::string::npos);
}

TEST(CoffeeMachineTests, MenuDisplayWhenOff) {
    std::stringstream output;
    CoffeeMachine machine(output);
    auto menu = machine.showMenu();
    EXPECT_TRUE(menu.empty());
    EXPECT_TRUE(output.str().empty());
}

TEST(CoffeeMachineTests, MenuDisplayWhenOn) {
    std::stringstream output;
    CoffeeMachine machine(output);
    machine.activate();
    auto menu = machine.showMenu();
    EXPECT_EQ(menu.size(), 5);
    EXPECT_TRUE(output.str().find("Espresso") != std::string::npos);
}

TEST(CoffeeMachineTests, AcceptValidPayment) {
    std::stringstream output;
    CoffeeMachine machine(output);
    machine.activate();
    machine.acceptPayment(50);
    EXPECT_EQ(machine.getDepositAmount(), 50);
    EXPECT_EQ(machine.checkStatus(), MachineState::PAYMENT);
}

TEST(CoffeeMachineTests, AcceptInvalidPayment) {
    std::stringstream output;
    CoffeeMachine machine(output);
    machine.activate();
    machine.acceptPayment(-10);
    EXPECT_EQ(machine.getDepositAmount(), 0);
    EXPECT_TRUE(output.str().find("Invalid") != std::string::npos);
}

TEST(CoffeeMachineTests, PaymentWhenOff) {
    std::stringstream output;
    CoffeeMachine machine(output);
    machine.acceptPayment(100);
    EXPECT_EQ(machine.getDepositAmount(), 0);
}

TEST(CoffeeMachineTests, SuccessfulSelection) {
    std::stringstream output;
    CoffeeMachine machine(output);
    machine.activate();
    machine.acceptPayment(100);
    auto result = machine.makeSelection(2); // Cappuccino
    EXPECT_EQ(result.first, SelectionResult::SUCCESS);
    EXPECT_EQ(result.second, 65);
    EXPECT_TRUE(output.str().find("Preparing") != std::string::npos);
}

TEST(CoffeeMachineTests, InsufficientFunds) {
    std::stringstream output;
    CoffeeMachine machine(output);
    machine.activate();
    machine.acceptPayment(20);
    auto result = machine.makeSelection(2); // Cappuccino costs 35
    EXPECT_EQ(result.first, SelectionResult::INSUFFICIENT_FUNDS);
    EXPECT_EQ(result.second, 20);
}

TEST(CoffeeMachineTests, InvalidSelection) {
    std::stringstream output;
    CoffeeMachine machine(output);
    machine.activate();
    machine.acceptPayment(50);
    auto result = machine.makeSelection(10); // Несуществующий напиток
    EXPECT_EQ(result.first, SelectionResult::INVALID_SELECTION);
    EXPECT_EQ(result.second, 50);
}

TEST(CoffeeMachineTests, SelectionWithoutPayment) {
    std::stringstream output;
    CoffeeMachine machine(output);
    machine.activate();
    auto result = machine.makeSelection(0);
    EXPECT_EQ(result.first, SelectionResult::UNAVAILABLE);
    EXPECT_EQ(result.second, 0);
}

TEST(CoffeeMachineTests, FullRefund) {
    std::stringstream output;
    CoffeeMachine machine(output);
    machine.activate();
    machine.acceptPayment(75);
    double refund = machine.refundPayment();
    EXPECT_EQ(refund, 75);
    EXPECT_EQ(machine.getDepositAmount(), 0);
    EXPECT_EQ(machine.checkStatus(), MachineState::READY);
}

TEST(CoffeeMachineTests, BrewingProcess) {
    std::stringstream output;
    CoffeeMachine machine(output);
    machine.activate();
    machine.acceptPayment(50);
    machine.makeSelection(3); // Latte
    // Проверяем сообщения о процессе приготовления
    EXPECT_TRUE(output.str().find("Preparing") != std::string::npos);
    EXPECT_TRUE(output.str().find("Enjoy") != std::string::npos);
}