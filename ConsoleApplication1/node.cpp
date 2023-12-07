#include "node.h"

node::node(node* parent, state s)
	:
	s(s)
{
	this->parent = parent;
	if (parent == nullptr)
		this->depth = 0;
	else
		this->depth = parent->depth + 1;

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
		node n(this, st);
		childs.push_back(n);
	}
	return childs;
}

int node::getDepth() const
{
	return depth;
}
