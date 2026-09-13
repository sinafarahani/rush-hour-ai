#include "node.h"

#include <utility>

#include "board.h"
#include "heuristic.h"

node::node(state s, int depth, int h)
	:
	s(std::move(s)),
	depth(depth),
	h(h)
{
}

node node::root(const state& s)
{
	return node(s, 0, heuristic::estimate(s, board(s)));
}

const state& node::getState() const
{
	return s;
}

int node::getDepth() const
{
	return depth;
}

int node::getF() const
{
	return depth + h;
}

std::vector<node> node::expand() const
{
	std::vector<node> children;
	board b(s);
	const auto& cars = s.getParking().getCars();
	for (std::size_t i = 0; i < cars.size(); i++) {
		const parking::Car& car = cars[i];
		const int from = s.getPosition(i);
		const int first = from - b.freeBefore(car, from);
		const int last = from + b.freeAfter(car, from);
		for (int to = first; to <= last; to++) {
			if (to == from) {
				continue;
			}
			state next = s.withPosition(i, to);
			b.move(car, from, to);
			const int nextH = heuristic::estimate(next, b);
			b.move(car, to, from);
			children.push_back(node(std::move(next), depth + 1, nextH));
		}
	}
	return children;
}

bool node::operator>(const node& n) const
{
	if (getF() != n.getF()) {
		return getF() > n.getF();
	}
	return depth < n.depth;
}
