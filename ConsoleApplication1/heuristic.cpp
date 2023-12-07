#include "heuristic.h"

heuristic::heuristic(state s)
	:
	s(s)
{
	val = 0;
	if (!s.solved()) {
		calcMinMoves();
	}
}

int heuristic::getVal() const
{
	return val;
}

void heuristic::calcMinMoves()
{
	for (const auto& car : getBlockingCarsR(s.getRedCar())) {
		val++;
		if (!hasSpace(s.getRedCar(), car)) {
			val++;
			bool next = true;
			for (const auto& c2 : getBlockingCarsL(car))
			{
				if (hasSpace(car, c2)) {
					next = false;
					break;
				}
			}
			for (const auto& c2 : getBlockingCarsR(car))
			{
				if (hasSpace(car, c2)) {
					next = false;
					break;
				}
			}
			if (next)
				val++;
		}

		
	}
}

std::vector<parking::Car> heuristic::getBlockingCarsR(parking::Car c)
{
	std::vector<parking::Car> blocking;

	for (const auto& car : s.getParking().cars) {
		if (car.h == c.h) {
			continue;
		}
		if (c.getFixedPos() < car.getVarPos().first || c.getFixedPos() > car.getVarPos().second) {
			continue;
		}
		if (checkBlockedR(c, car)) {
			blocking.push_back(car);
		}
	}

	return blocking;
}

std::vector<parking::Car> heuristic::getBlockingCarsL(parking::Car c)
{
	std::vector<parking::Car> blocking;

	for (const auto& car : s.getParking().cars) {
		if (car.h == c.h) {
			continue;
		}
		if (c.getFixedPos() < car.getVarPos().first || c.getFixedPos() > car.getVarPos().second) {
			continue;
		}
		if (checkBlockedL(c, car)) {
			blocking.push_back(car);
		}
	}

	return blocking;
}

bool heuristic::checkBlockedR(parking::Car c1, parking::Car c2) const
{
	return (c1.getVarPos().second < c2.getFixedPos()) && (c1.getFixedPos() >= c2.getVarPos().first && c1.getFixedPos() <= c2.getVarPos().second);
}

bool heuristic::checkBlockedL(parking::Car c1, parking::Car c2) const
{
	return (c1.getVarPos().second > c2.getFixedPos()) && (c1.getFixedPos() >= c2.getVarPos().first && c1.getFixedPos() <= c2.getVarPos().second);
}

bool heuristic::hasSpace(parking::Car c1, parking::Car c2)
{
	return (std::abs(c1.getFixedPos() - c2.getVarPos().second) - std::abs(s.checkMoveL(c2)) < 0) || (std::abs(c1.getFixedPos() - c2.getVarPos().first) - s.checkMoveR(c2) < 0);
}
