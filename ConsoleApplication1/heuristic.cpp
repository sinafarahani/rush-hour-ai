#include "heuristic.h"

int heuristic::estimate(const state& s, const board& b)
{
	if (s.solved()) {
		return 0;
	}

	const parking& p = s.getParking();
	const auto& cars = p.getCars();
	const parking::Car& red = p.getRedCar();
	const int aheadOfRed = s.getPosition(0) + red.length;

	int blockers = 0;
	bool stuck = false;
	for (std::size_t i = 1; i < cars.size(); i++) {
		const parking::Car& car = cars[i];
		// A parallel car in the red car's lane can never leave it, so the puzzle is
		// unsolvable anyway; only perpendicular cars are counted.
		if (car.horizontal == red.horizontal || car.line < aheadOfRed) {
			continue;
		}
		const int position = s.getPosition(i);
		if (red.line < position || red.line >= position + car.length) {
			continue;
		}

		blockers++;
		if (!stuck) {
			// Distance the blocker has to slide to clear the lane in each direction.
			const int neededBefore = position + car.length - red.line;
			const int neededAfter = red.line - position + 1;
			stuck = b.freeBefore(car, position) < neededBefore && b.freeAfter(car, position) < neededAfter;
		}
	}
	return 1 + blockers + (stuck ? 1 : 0);
}
