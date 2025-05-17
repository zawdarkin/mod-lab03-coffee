// Copyright 2023 BeverageTech
#include <gtest/gtest.h>
#include <sstream>
#include <string>
#include <vector>
#include "Automata.h"

TEST(MachineInitialization, PowerOffState) {
    std::stringstream output;
    Automata* machine = new Automata(output);
    ASSERT_EQ(STATES::OFF, machine->getState());
    EXPECT_EQ(output.str(), "OFF\n");
    delete machine;
}

TEST(PowerManagement, TurnOnFromOff) {
    std::stringstream output;
    Automata* machine = new Automata(output);
    machine->on();
    ASSERT_EQ(STATES::WAIT, machine->getState());
    EXPECT_EQ(output.str(), "WAIT\n");
    delete machine;
}

TEST(PowerManagement, TurnOffFromIdle) {
    std::stringstream output;
    Automata* machine = new Automata(output);
    machine->on();
    machine->off();
    ASSERT_EQ(STATES::OFF, machine->getState());
    EXPECT_EQ(output.str(), "OFF\n");
    delete machine;
}

TEST(PaymentProcessing, InsertValidAmount) {
    std::stringstream output;
    Automata* machine = new Automata(output);
    machine->on();
    machine->coin(25.0);
    ASSERT_EQ(STATES::ACCEPT, machine->getState());
    EXPECT_EQ(25.0, machine->getCashe());
    delete machine;
}

TEST(PaymentProcessing, InsertNegativeAmount) {
    std::stringstream output;
    Automata* machine = new Automata(output);
    machine->on();
    machine->coin(-10.0);
    ASSERT_EQ(STATES::WAIT, machine->getState());
    EXPECT_EQ(0.0, machine->getCashe());
    delete machine;
}

TEST(OrderProcessing, InsufficientFunds) {
    std::stringstream output;
    Automata* machine = new Automata(output);
    machine->on();
    machine->coin(20.0);
    auto result = machine->choice(2);
    ASSERT_EQ(CHOISE_STATES::NOT_ENOUGHT_MONEY, result.first);
    EXPECT_EQ(20.0, result.second);
    delete machine;
}

TEST(OrderProcessing, SuccessfulOrder) {
    std::stringstream output;
    Automata* machine = new Automata(output);
    machine->on();
    machine->coin(100.0);
    auto result = machine->choice(2);
    ASSERT_EQ(CHOISE_STATES::OK, result.first);
    EXPECT_EQ(65.0, result.second);
    delete machine;
}

TEST(OrderProcessing, InvalidSelection) {
    std::stringstream output;
    Automata* machine = new Automata(output);
    machine->on();
    machine->coin(20.0);
    auto result = machine->choice(10);
    ASSERT_EQ(CHOISE_STATES::INVALID_ITEM, result.first);
    EXPECT_EQ(20.0, result.second);
    delete machine;
}

TEST(MenuDisplay, WhenPoweredOff) {
    std::stringstream output;
    Automata* machine = new Automata(output);
    size_t result = machine->getMenu().size();
    ASSERT_EQ(0, result);
    EXPECT_EQ(output.str(), "");
    delete machine;
}

TEST(MenuDisplay, WhenPoweredOn) {
    std::stringstream output;
    Automata* machine = new Automata(output);
    machine->on();
    size_t result = machine->getMenu().size();
    EXPECT_EQ(5, result);
    delete machine;
}
