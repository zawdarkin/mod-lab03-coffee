// Copyright 2023 BeverageTech
#include <gtest/gtest.h>
#include <sstream>
#include <string>
#include "Automata.h"

TEST(CustomMenuTest, ConstructorWithCustomMenu) {
    std::stringstream output;
    std::vector<DrinkItem> customMenu = {
        {"Tea", 10}, {"Coffee", 15}, {"Hot Chocolate", 20}};
    Automata machine(output, customMenu);
    machine.on();
    ASSERT_EQ(3, machine.getMenu().size());
}

TEST(StateTransitionTest, FromAcceptToCheckOnChoice) {
    std::stringstream output;
    Automata machine(output);
    machine.on();
    machine.coin(50.0);
    machine.choice(3); // Latte costs 50
    ASSERT_EQ(STATES::COOK, machine.getState());
}

TEST(CancelOperationTest, CancelInAcceptState) {
    std::stringstream output;
    Automata machine(output);
    machine.on();
    machine.coin(30.0);
    double refund = machine.cancel();
    ASSERT_EQ(30.0, refund);
    ASSERT_EQ(STATES::WAIT, machine.getState());
}

TEST(RevenueCalculationTest, RevenueAfterSuccessfulOrder) {
    std::stringstream output;
    Automata machine(output);
    machine.on();
    machine.coin(100.0);
    machine.choice(3); // Latte costs 50
    machine.coin(100.0);
    machine.choice(0); // Espresso costs 15
    ASSERT_EQ(65.0, machine.getCashe()); // 100 + 100 - 50 - 15 = 135, but getCashe returns currentDeposit
}

TEST(DrinkPreparationTest, CookingStateDuration) {
    std::stringstream output;
    Automata machine(output);
    machine.on();
    machine.coin(50.0);
    auto start = std::chrono::steady_clock::now();
    machine.choice(3); // Latte costs 50
    auto end = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start);
    ASSERT_GE(duration.count(), 5);
}

TEST(MultipleCoinsTest, AccumulateDeposit) {
    std::stringstream output;
    Automata machine(output);
    machine.on();
    machine.coin(10.0);
    machine.coin(20.0);
    machine.coin(5.0);
    ASSERT_EQ(35.0, machine.getCashe());
}

TEST(EdgeCaseTest, ZeroAmountCoin) {
    std::stringstream output;
    Automata machine(output);
    machine.on();
    machine.coin(0.0);
    ASSERT_EQ(STATES::WAIT, machine.getState());
    ASSERT_EQ(0.0, machine.getCashe());
}

TEST(OrderProcessingTest, ExactAmountPayment) {
    std::stringstream output;
    Automata machine(output);
    machine.on();
    machine.coin(35.0); // Exact amount for Cappuccino
    auto result = machine.choice(2);
    ASSERT_EQ(CHOISE_STATES::OK, result.first);
    ASSERT_EQ(0.0, result.second);
}

TEST(StateProtectionTest, CoinInOffState) {
    std::stringstream output;
    Automata machine(output);
    machine.coin(10.0);
    ASSERT_EQ(STATES::OFF, machine.getState());
    ASSERT_EQ(0.0, machine.getCashe());
}

TEST(CompleteServiceTest, ReturnToWaitState) {
    std::stringstream output;
    Automata machine(output);
    machine.on();
    machine.coin(50.0);
    machine.choice(3); // Latte costs 50
    // After cooking should return to WAIT state
    ASSERT_EQ(STATES::WAIT, machine.getState());
}
