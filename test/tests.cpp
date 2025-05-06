// Copyright 2022 GHA Test Team
#include <gtest/gtest.h>
#include <utility>
#include <iostream>
#include <sstream>
#include <string>

#include "Automata.h"

TEST(task1, test1) {
  std::stringstream strs;
  Automata* automata = new Automata(strs);
  ASSERT_EQ(STATES::OFF, automata->getState());
  EXPECT_EQ(strs.str(), "OFF\n");
}

TEST(task1, test2) {
  std::stringstream strs;
  Automata* automata = new Automata(strs);
  automata->on();
  ASSERT_EQ(STATES::WAIT, automata->getState());
  EXPECT_EQ(strs.str(), "WAIT\n");
}

TEST(task1, test3) {
  std::stringstream strs;
  Automata* automata = new Automata(strs);
  automata->on();
  automata->off();
  ASSERT_EQ(STATES::OFF, automata->getState());
  EXPECT_EQ(strs.str(), "OFF\n");
}

TEST(task1, test4) {
  std::stringstream strs;
  Automata* automata = new Automata(strs);
  automata->on();
  automata->on();
  ASSERT_EQ(STATES::WAIT, automata->getState());
  EXPECT_EQ(strs.str(), "WAIT\n");
}

TEST(task1, test5) {
  std::stringstream strs;
  Automata* automata = new Automata(strs);
  automata->off();
  ASSERT_EQ(STATES::OFF, automata->getState());
  EXPECT_EQ(strs.str(), "OFF\n");
}

TEST(task2, test1) {
  std::stringstream strs;
  Automata* automata = new Automata(strs);
  automata->on();
  automata->coin(20);
  STATES result = automata->getState();
  ASSERT_EQ(STATES::ACCEPT, result);
  std::pair<CHOISE_STATES, double> cookResult = automata->choice(2);
  ASSERT_EQ(CHOISE_STATES::NOT_ENOUGHT_MONEY, cookResult.first);
  EXPECT_EQ(20, cookResult.second);
}

TEST(task2, test2) {
  std::stringstream strs;
  Automata* automata = new Automata(strs);
  automata->on();
  std::pair<CHOISE_STATES, double> cookResult = automata->choice(0);
  ASSERT_EQ(CHOISE_STATES::INACCESSIBLE, cookResult.first);
  EXPECT_EQ(0, cookResult.second);
}

TEST(task2, test3) {
  std::stringstream strs;
  Automata* automata = new Automata(strs);
  automata->on();
  automata->coin(100);
  std::pair<CHOISE_STATES, double> cookResult = automata->choice(2);
  ASSERT_EQ(CHOISE_STATES::OK, cookResult.first);
  EXPECT_EQ(65, cookResult.second);
}

TEST(task2, test4) {
  std::stringstream strs;
  Automata* automata = new Automata(strs);
  automata->on();
  automata->coin(20);
  std::pair<CHOISE_STATES, double> cookResult = automata->choice(-2);
  ASSERT_EQ(CHOISE_STATES::INVALID_ITEM, cookResult.first);
  EXPECT_EQ(20, cookResult.second);
}

TEST(task2, test5) {
  std::stringstream strs;
  Automata* automata = new Automata(strs);
  automata->on();
  automata->coin(-10);
  ASSERT_EQ(0, automata->getCashe());
  EXPECT_EQ(STATES::WAIT, automata->getState());
}

TEST(task2, test6) {
  std::stringstream strs;
  Automata* automata = new Automata(strs);
  automata->on();
  automata->coin(10);
  automata->coin(-10);
  ASSERT_EQ(10, automata->getCashe());
  EXPECT_EQ(STATES::ACCEPT, automata->getState());
}

TEST(task2, test7) {
  std::stringstream strs;
  Automata* automata = new Automata(strs);
  automata->coin(10);
  ASSERT_EQ(0, automata->getCashe());
  EXPECT_EQ(STATES::OFF, automata->getState());
}

TEST(task2, test8) {
  std::stringstream strs;
  Automata* automata = new Automata(strs);
  automata->on();
  automata->coin(100);
  automata->choice(2);
  automata->coin(1000);
  ASSERT_EQ(1000, automata->getCashe());
  EXPECT_EQ(STATES::ACCEPT, automata->getState());
}

TEST(task3, test1) {
  std::stringstream strs;
  Automata* automata = new Automata(strs);
  size_t result = (automata->getMenu()).size();
  ASSERT_EQ(0, result);
  EXPECT_EQ(strs.str(), "");
}

TEST(task3, test2) {
  std::stringstream strs;
  Automata* automata = new Automata(strs);
  automata->on();
  size_t result = (automata->getMenu()).size();
  EXPECT_EQ(5, result);
}
