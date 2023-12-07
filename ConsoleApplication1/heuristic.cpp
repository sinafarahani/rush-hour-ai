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
	for (const auto& car : getBlockingCars(s.getRedCar())) {
		if (hasSpace(s.getRedCar(), car)) {
			val++;
		}
		else {
			val += 2;
			/*for (const auto& c2 : getBlockingCars(car))
			{
				if (!hasSpace(car, c2))
					val++;
			}*/
		}
	}
}

std::vector<parking::Car> heuristic::getBlockingCars(parking::Car c)
{
	std::vector<parking::Car> blocking;

	for (const auto& car : s.getParking().cars) {
		if (car.h == c.h) {
			continue;
		}
		if (c.getFixedPos() < car.getVarPos().first || c.getFixedPos() > car.getVarPos().second) {
			continue;
		}
		if (checkBlocked(c, car)) {
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
