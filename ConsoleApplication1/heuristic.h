#pragma once

#include "state.h"

class heuristic {
public:
	heuristic(state s);
	int getVal() const;
private:
	void calcMinMoves();
	std::vector<parking::Car> getBlockingCars(parking::Car c);
	bool checkBlocked(parking::Car c1, parking::Car c2) const;
	bool hasSpace(parking::Car c1, parking::Car c2);
	int val;
	state s;
};