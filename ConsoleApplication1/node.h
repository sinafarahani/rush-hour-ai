#pragma once

#include <vector>

#include "state.h"

// A search node: a state, the number of moves made to reach it and its heuristic value.
class node {
public:
	static node root(const state& s);

	const state& getState() const;
	int getDepth() const;
	int getF() const;

	// All nodes one move away. A move slides a single car any number of free cells.
	[[nodiscard]] std::vector<node> expand() const;

	// "Greater" means "expand later": higher f first, and on equal f the shallower node,
	// since deeper nodes are usually closer to the goal.
	bool operator>(const node& n) const;

private:
	node(state s, int depth, int h);

	state s;
	int depth;
	int h;
};
