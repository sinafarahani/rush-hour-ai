#include "board.h"

board::board(const state& s)
	:
	p(s.getParking()),
	cells(p.getM() * p.getN(), false)
{
	const auto& cars = p.getCars();
	for (std::size_t i = 0; i < cars.size(); i++) {
		fill(cars[i], s.getPosition(i), true);
	}
}

int board::freeBefore(const parking::Car& c, int position) const
{
	int space = 0;
	for (int along = position - 1; along >= 0 && !cells[cell(c, along)]; along--) {
		space++;
	}
	return space;
}

int board::freeAfter(const parking::Car& c, int position) const
{
	int space = 0;
	for (int along = position + c.length; along < p.axisLength(c) && !cells[cell(c, along)]; along++) {
		space++;
	}
	return space;
}

void board::move(const parking::Car& c, int from, int to)
{
	fill(c, from, false);
	fill(c, to, true);
}

int board::cell(const parking::Car& c, int along) const
{
	return c.horizontal ? c.line * p.getN() + along : along * p.getN() + c.line;
}

void board::fill(const parking::Car& c, int position, bool value)
{
	for (int k = 0; k < c.length; k++) {
		cells[cell(c, position + k)] = value;
	}
}
