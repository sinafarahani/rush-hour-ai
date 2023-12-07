#pragma once

#include "state.h"

class heuristic {
public:
	heuristic(state s);
	int getVal() const;
private:
	void calcMinMoves();
	std::vector<parking::Car> getBlockingCarsR(parking::Car c);
	std::vector<parking::Car> getBlockingCarsL(parking::Car c);
	bool checkBlockedR(parking::Car c1, parking::Car c2) const;
	bool checkBlockedL(parking::Car c1, parking::Car c2) const;
	bool hasSpace(parking::Car c1, parking::Car c2);
	int val;
	state s;
};