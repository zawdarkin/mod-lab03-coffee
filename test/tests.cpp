// Copyright 2023 BeverageTech
#include <gtest/gtest.h>
#include <sstream>
#include <string>
#include "Automata.h"

TEST(MachineInitialization, PowerOffState) {
    std::stringstream output;
    BeverageMachine* machine = new BeverageMachine(output);
    ASSERT_EQ(MachineState::POWER_OFF, machine->getCurrentStatus());
    EXPECT_EQ(output.str(), "POWER_OFF\n");
    delete machine;
}

TEST(PowerManagement, TurnOnFromOff) {
    std::stringstream output;
    BeverageMachine* machine = new BeverageMachine(output);
    machine->powerOn();
    ASSERT_EQ(MachineState::IDLE, machine->getCurrentStatus());
    EXPECT_EQ(output.str(), "IDLE\n");
    delete machine;
}

TEST(PowerManagement, TurnOffFromIdle) {
    std::stringstream output;
    BeverageMachine* machine = new BeverageMachine(output);
    machine->powerOn();
    machine->powerOff();
    ASSERT_EQ(MachineState::POWER_OFF, machine->getCurrentStatus());
    EXPECT_EQ(output.str(), "POWER_OFF\n");
    delete machine;
}

TEST(PaymentProcessing, InsertValidAmount) {
    std::stringstream output;
    BeverageMachine* machine = new BeverageMachine(output);
    machine->powerOn();
    machine->insertMoney(25.0);
    ASSERT_EQ(MachineState::PAYMENT_ACCEPTING, machine->getCurrentStatus());
    EXPECT_EQ(25.0, machine->getCurrentBalance());
    delete machine;
}

TEST(PaymentProcessing, InsertNegativeAmount) {
    std::stringstream output;
    BeverageMachine* machine = new BeverageMachine(output);
    machine->powerOn();
    machine->insertMoney(-10.0);
    ASSERT_EQ(MachineState::IDLE, machine->getCurrentStatus());
    EXPECT_EQ(0.0, machine->getCurrentBalance());
    delete machine;
}

TEST(OrderProcessing, InsufficientFunds) {
    std::stringstream output;
    BeverageMachine* machine = new BeverageMachine(output);
    machine->powerOn();
    machine->insertMoney(20.0);
    auto result = machine->selectBeverage(2);
    ASSERT_EQ(OrderStatus::INSUFFICIENT_FUNDS, result.first);
    EXPECT_EQ(20.0, result.second);
    delete machine;
}

TEST(OrderProcessing, SuccessfulOrder) {
    std::stringstream output;
    BeverageMachine* machine = new BeverageMachine(output);
    machine->powerOn();
    machine->insertMoney(100.0);
    auto result = machine->selectBeverage(2);
    ASSERT_EQ(OrderStatus::SUCCESS, result.first);
    EXPECT_EQ(65.0, result.second);
    delete machine;
}

TEST(OrderProcessing, InvalidSelection) {
    std::stringstream output;
    BeverageMachine* machine = new BeverageMachine(output);
    machine->powerOn();
    machine->insertMoney(20.0);
    auto result = machine->selectBeverage(10);
    ASSERT_EQ(OrderStatus::INVALID_SELECTION, result.first);
    EXPECT_EQ(20.0, result.second);
    delete machine;
}

TEST(MenuDisplay, WhenPoweredOff) {
    std::stringstream output;
    BeverageMachine* machine = new BeverageMachine(output);
    size_t result = machine->showMenu().size();
    ASSERT_EQ(0, result);
    EXPECT_EQ(output.str(), "");
    delete machine;
}

TEST(MenuDisplay, WhenPoweredOn) {
    std::stringstream output;
    BeverageMachine* machine = new BeverageMachine(output);
    machine->powerOn();
    size_t result = machine->showMenu().size();
    EXPECT_EQ(5, result);
    delete machine;
}
