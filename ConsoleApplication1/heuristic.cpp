#include "heuristic.h"

heuristic::heuristic(state s)
	:
	s(s)
{
	if (s.solved()) {
		val = 0;
	}
	else {
		calcMinMoves();
	}
}

int heuristic::getVal() const
{
	return val;
}

void heuristic::calcMinMoves()
{
	val = 0;
	for (const auto& car : getBlockingCars()) {
		if (hasSpace(s.getRedCar(), car)) {
			val++;
		}
		else {
			val += 2;/*
			for (const auto& c2 : s.getParking().cars)
			{
				if (checkBlocked(car, c2) && !hasSpace(car, c2))
					val++;
			}*/
		}
	}
}

std::vector<parking::Car> heuristic::getBlockingCars()
{
	std::vector<parking::Car> blocking;

	for (const auto& car : s.getParking().cars) {
		if (car.h == s.getRedCar().h) {
			continue;
		}
		if (s.getRedCar().getFixedPos() < car.getVarPos().first || s.getRedCar().getFixedPos() > car.getVarPos().second) {
			continue;
		}
		if (checkBlocked(s.getRedCar(), car)) {
			blocking.push_back(car);
		}
	}

	return blocking;
}

bool heuristic::checkBlocked(parking::Car c1, parking::Car c2) const
{
	return (c1.getFixedPos() >= c2.getVarPos().first && c1.getFixedPos() <= c2.getVarPos().second);
}

bool heuristic::hasSpace(parking::Car c1, parking::Car c2)
{
	return (std::abs(c1.getFixedPos() - c2.getVarPos().second) - s.checkMoveL(c2) < 0) || (std::abs(c1.getFixedPos() - c2.getVarPos().first) - s.checkMoveR(c2) < 0);
}
