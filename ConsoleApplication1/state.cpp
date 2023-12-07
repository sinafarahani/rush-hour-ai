#include "state.h"

state::state(parking p)
	:
	p(p)
{
	boardPoints = new bool* [p.getM()];
	for (int i = 0; i < p.getM(); ++i) {
		boardPoints[i] = new bool[p.getN()];
	}
	for (int i = 0; i < p.getM(); i++) {
		for (int j = 0; j < p.getN(); j++) {
			boardPoints[i][j] = false;
		}
	}
	for (const auto& car : p.cars) {
		for (int i = car.row[0]; i <= car.row[1]; i++) {
			for (int j = car.col[0]; j <= car.col[1]; j++) {
				boardPoints[i - 1][j - 1] = true;
			}
		}
	}
	brdpts.clear();
	for (int i = 0; i < p.getM(); i++) {
		for (int j = 0; j < p.getN(); j++) {
			brdpts.push_back(boardPoints[i][j]);
		}
	}
	red = p.cars[0];
}

bool state::operator==(state& s)
{
	bool r = true;
	if (this->p.cars.size() == s.p.cars.size()) {
		for (int i = 0; i < this->p.cars.size(); i++) {
			r &= (this->p.cars[i] == s.p.cars[i]);
		}
		return r;
	}
	else
		return false;
}

bool state::operator==(const state& s) const
{
	bool r = true;
	if (this->p.cars.size() == s.p.cars.size()) {
		for (int i = 0; i < this->p.cars.size(); i++) {
			r &= (this->p.cars[i] == s.p.cars[i]);
		}
		return r;
	}
	else
		return false;
}

std::vector<state> state::getNextStates()
{
	parking p2 = p;
	for (int i = 0; i < p.cars.size(); i++) {
		for (int j = std::abs(checkMoveL(p2.cars[i])); j > 0; j--) {
			p2.cars[i] -= j;
			state ss(p2);
			ss.movedCar = i;
			nextStates.push_back(ss);
			p2 = p;
		}
	}
	for (int i = 0; i < p.cars.size(); i++) {
		for (int j = std::abs(checkMoveR(p2.cars[i])); j > 0; j--) {
			p2.cars[i] += j;
			state ss(p2);
			ss.movedCar = i;
			nextStates.push_back(ss);
			p2 = p;
		}
	}
	return nextStates;
}

parking state::getParking() const
{
	return p;
}

bool state::solved() const
{
	if (red.h == 'h')
		return red.col[1] == p.getN();
	else
		return red.row[1] == p.getM();
}

parking::Car state::getRedCar()
{
	return red;
}

int state::checkMoveL(parking::Car c)
{
	int space = -1;
	if (c.h == 'h') {
		if (c.col[0] <= 1) {
			return 0;
		}
		while (!boardPoints[c.row[0] - 1][c.col[0] - 1 + space]) {
			space--;
			if (c.col[0] - 1 + space < 0) {
				return space + 1;
			}
		}
	}
	else if (c.h == 'v') {
		if (c.row[0] <= 1) {
			return 0;
		}
		while (!boardPoints[c.row[0] - 1 + space][c.col[0] - 1]) {
			space--;
			if (c.row[0] - 1 + space < 0) {
				return space + 1;
			}
		}
	}
	return space + 1;
}
int state::checkMoveR(parking::Car c)
{
	int space = 1;
	if (c.h == 'h') {
		if (c.col[1] >= p.getN()) {
			return 0;
		}
		while (!boardPoints[c.row[1] - 1][c.col[1] - 1 + space]) {
			space++;
			if (c.col[1] - 1 + space >= p.getN()) {
				return space - 1;
			}
		}
	}
	else if (c.h == 'v') {
		if (c.row[1] >= p.getM()) {
			return 0;
		}
		while (!boardPoints[c.row[1] - 1 + space][c.col[1] - 1]) {
			space++;
			if (c.row[1] - 1 + space >= p.getM()) {
				return space - 1;
			}
		}
	}
	return space - 1;
}

void state::print() const
{
	for (int i = 0; i < p.getM(); i++) {
		for (int j = 0; j < p.getN(); j++) {
			std::cout << boardPoints[i][j] << " ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}
