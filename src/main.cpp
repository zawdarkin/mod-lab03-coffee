// Copyright 2022 UNN-IASR
#include <iostream>
#include "CoffeeMachine.h"

int main() {
	CoffeeMachine* myMachine = new CoffeeMachine(std::cout);

	myMachine->activate();
	myMachine->showMenu();

	myMachine->acceptPayment(20);
	myMachine->getDepositAmount();

	myMachine->checkStatus();
	myMachine->makeSelection(2);

	myMachine->acceptPayment(100);
	myMachine->makeSelection(2);

	delete myMachine;
	return 0;
}