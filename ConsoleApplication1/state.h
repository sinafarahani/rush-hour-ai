#pragma once

#include "parking.h"

class state {
public:
	state(parking p);
	bool operator==(state& s);
	bool operator==(const state& s) const;
	std::vector<state> getNextStates();
	parking getParking() const;
	bool solved() const;
	parking::Car getRedCar();
	int checkMoveL(parking::Car c);
	int checkMoveR(parking::Car c);
	int movedCar;
private:
	parking p;
	parking::Car red;
	bool** boardPoints;
	std::vector<state> nextStates;
};