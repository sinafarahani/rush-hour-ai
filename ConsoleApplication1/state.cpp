#include "state.h"

#include <functional>

state::state(const parking& p)
	:
	p(&p)
{
	for (int position : p.getStartPositions()) {
		positions.push_back(static_cast<char>(position));
	}
}

bool state::operator==(const state& s) const
{
	return positions == s.positions;
}

int state::getPosition(std::size_t car) const
{
	return static_cast<unsigned char>(positions[car]);
}

state state::withPosition(std::size_t car, int position) const
{
	state next = *this;
	next.positions[car] = static_cast<char>(position);
	return next;
}

bool state::solved() const
{
	const parking::Car& red = p->getRedCar();
	return getPosition(0) + red.length == p->axisLength(red);
}

const parking& state::getParking() const
{
	return *p;
}

std::size_t state::hash::operator()(const state& s) const
{
	return std::hash<std::string>{}(s.positions);
}
