// Copyright 2022 GHA Test Team
#include <gtest/gtest.h>
#include "Automata.h"

TEST(AutomataTest, InitialState) {
    Automata automata;
    ASSERT_EQ(automata.getState(), STATES::OFF);
}

TEST(AutomataTest, TurnOn) {
    Automata automata;
    automata.on();
    ASSERT_EQ(automata.getState(), STATES::WAIT);
}

TEST(AutomataTest, TurnOff) {
    Automata automata;
    automata.on();
    automata.off();
    ASSERT_EQ(automata.getState(), STATES::OFF);
}

TEST(AutomataTest, InsertCoin) {
    Automata automata;
    automata.on();
    automata.coin(20);
    ASSERT_EQ(automata.getState(), STATES::ACCEPT);
}

TEST(AutomataTest, GetMenu) {
    Automata automata;
    auto menu = automata.getMenu();
    ASSERT_FALSE(menu.empty());
}

TEST(AutomataTest, CheckPrices) {
    Automata automata;
    auto prices = automata.getPrices();
    ASSERT_EQ(prices.size(), 5);
}

TEST(AutomataTest, SuccessfulPurchase) {
    Automata automata;
    automata.on();
    automata.coin(50);
    automata.choice(1);  // Americano
    ASSERT_EQ(automata.getState(), STATES::WAIT);
}

TEST(AutomataTest, InsufficientFunds) {
    Automata automata;
    automata.on();
    automata.coin(10);
    automata.choice(2);  // Cappuccino
    ASSERT_EQ(automata.getState(), STATES::ACCEPT);
}

TEST(AutomataTest, CancelOperation) {
    Automata automata;
    automata.on();
    automata.coin(30);
    automata.cancel();
    ASSERT_EQ(automata.getState(), STATES::WAIT);
}

TEST(AutomataTest, InvalidChoice) {
    Automata automata;
    automata.on();
    automata.coin(50);
    automata.choice(10);  // Несуществующий напиток
    ASSERT_EQ(automata.getState(), STATES::ACCEPT);
}
