#include "parking.h"

#include <format>
#include <stdexcept>

parking parking::read(std::istream& in)
{
	parking p;
	int V;
	if (!(in >> p.M >> p.N >> V)) {
		throw std::runtime_error("expected \"M N V\"");
	}
	if (p.M < 1 || p.N < 1 || p.M > maxBoardSize || p.N > maxBoardSize) {
		throw std::runtime_error(std::format("board size must be between 1x1 and {0}x{0}", maxBoardSize));
	}
	if (V < 1) {
		throw std::runtime_error("there must be at least one car (the red car)");
	}

	std::vector<bool> occupied(p.M * p.N, false);
	for (int i = 1; i <= V; i++) {
		int r, c, l;
		char o;
		if (!(in >> r >> c >> o >> l)) {
			throw std::runtime_error(std::format("car {}: expected \"row col h|v length\"", i));
		}
		if (o != 'h' && o != 'v' && o != 'H' && o != 'V') {
			throw std::runtime_error(std::format("car {}: orientation must be 'h' or 'v'", i));
		}
		if (l < 1) {
			throw std::runtime_error(std::format("car {}: length must be positive", i));
		}

		const bool horizontal = (o == 'h' || o == 'H');
		const Car car{ .horizontal = horizontal, .line = horizontal ? r - 1 : c - 1, .length = l };
		const int position = horizontal ? c - 1 : r - 1;

		if (r < 1 || r > p.M || c < 1 || c > p.N || position + l > p.axisLength(car)) {
			throw std::runtime_error(std::format("car {}: does not fit on the board", i));
		}
		for (int k = 0; k < l; k++) {
			const int cell = horizontal ? car.line * p.N + position + k : (position + k) * p.N + car.line;
			if (occupied[cell]) {
				throw std::runtime_error(std::format("car {}: overlaps another car", i));
			}
			occupied[cell] = true;
		}

		p.cars.push_back(car);
		p.startPositions.push_back(position);
	}
	return p;
}

int parking::getM() const
{
	return M;
}

int parking::getN() const
{
	return N;
}

const std::vector<parking::Car>& parking::getCars() const
{
	return cars;
}

const parking::Car& parking::getRedCar() const
{
	return cars.front();
}

const std::vector<int>& parking::getStartPositions() const
{
	return startPositions;
}

int parking::axisLength(const Car& c) const
{
	return c.horizontal ? N : M;
}
