#include "node.h"

node::node(state s, int d)
	:
	s(s)
{
	this->depth = d + 1;
	heuristic he(s);
	h = he.getVal();
}

state node::getState() const
{
	return s;
}

std::vector<node> node::expand()
{
	std::vector<node> childs;
	for (const auto& st : s.getNextStates()) {
		node n(st, depth);
		childs.push_back(n);
	}
	return childs;
}

int node::getDepth() const
{
	return depth;
}
