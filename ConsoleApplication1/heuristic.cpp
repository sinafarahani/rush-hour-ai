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
			if (checkForSpace(car)) {
				val++;
			}
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

std::vector<parking::Car> heuristic::getBlockingCars(parking::Car c)
{
	std::vector<parking::Car> car;
	auto cr = getBlockingCarsR(c);
	auto cl = getBlockingCarsL(c);
	car.reserve(cr.size() + cl.size());
	car.insert(car.end(), cr.begin(), cr.end());
	car.insert(car.end(), cl.begin(), cl.end());
	return car;
}

bool heuristic::checkForSpace(parking::Car c1)
{
	for (const auto& c2 : getBlockingCarsL(c1))
	{
		if (hasSpace(c1, c2)) {
			return false;
		}
	}
	for (const auto& c2 : getBlockingCarsR(c1))
	{
		if (hasSpace(c1, c2)) {
			return false;;
		}
	}
	return true;
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
