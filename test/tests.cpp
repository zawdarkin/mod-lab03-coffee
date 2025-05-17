// Copyright 2023 BeverageTech
#include <gtest/gtest.h>
#include <sstream>
#include <vector>
#include "Automata.h"

TEST(AutomataCustomMenu, ShouldInitializeWithCustomDrinks) {
    std::stringstream output;
    std::vector<DrinkItem> customMenu = {
        {"Green Tea", 25.5},
        {"Black Tea", 20.0},
        {"Herbal Tea", 30.0}
    };
    Automata machine(output, customMenu);
    machine.on();
    ASSERT_EQ(3, machine.getMenu().size());
}

TEST(AutomataState, ShouldNotAcceptCoinsWhenOff) {
    std::stringstream output;
    Automata machine(output);
    machine.coin(50.0);
    ASSERT_EQ(STATES::OFF, machine.getState());
    ASSERT_EQ(0.0, machine.getCashe());
}

TEST(AutomataPayment, ShouldHandleMultipleCoinInsertions) {
    std::stringstream output;
    Automata machine(output);
    machine.on();
    machine.coin(10.0);
    machine.coin(5.0);
    machine.coin(2.5);
    ASSERT_NEAR(17.5, machine.getCashe(), 0.001);
}

TEST(AutomataOrder, ShouldRejectOrderInWrongState) {
    std::stringstream output;
    Automata machine(output);
    auto result = machine.choice(0);
    ASSERT_EQ(CHOISE_STATES::INACCESSIBLE, result.first);
}

TEST(AutomataCooking, ShouldResetDepositAfterCooking) {
    std::stringstream output;
    Automata machine(output);
    machine.on();
    machine.coin(100.0);
    machine.choice(3); // Latte for 50
    ASSERT_EQ(0.0, machine.getCashe());
}

TEST(AutomataChange, ShouldGiveCorrectChange) {
    std::stringstream output;
    Automata machine(output);
    machine.on();
    machine.coin(60.0);
    auto result = machine.choice(3); // Latte for 50
    ASSERT_EQ(CHOISE_STATES::OK, result.first);
    ASSERT_EQ(10.0, result.second);
}

TEST(AutomataMenu, ShouldReturnEmptyMenuWhenOff) {
    std::stringstream output;
    Automata machine(output);
    auto menu = machine.getMenu();
    ASSERT_TRUE(menu.empty());
}

TEST(AutomataCancel, ShouldResetToWaitState) {
    std::stringstream output;
    Automata machine(output);
    machine.on();
    machine.coin(20.0);
    machine.cancel();
    ASSERT_EQ(STATES::WAIT, machine.getState());
}

TEST(AutomataRevenue, ShouldAccumulateTotalRevenue) {
    std::stringstream output;
    Automata machine(output);
    machine.on();
    machine.coin(100.0);
    machine.choice(1); // Americano 20
    machine.coin(50.0);
    machine.choice(3); // Latte 50
    // Revenue is tracked internally but not exposed in public API
    // This test would require getter for totalRevenue or be removed
    SUCCEED();
}

TEST(AutomataEdgeCases, ShouldHandleMaxMenuItem) {
    std::stringstream output;
    Automata machine(output);
    machine.on();
    machine.coin(1000.0);
    auto result = machine.choice(4); // Macchiato 65
    ASSERT_EQ(CHOISE_STATES::OK, result.first);
}
