#pragma once

#include <algorithm>

#include "parking.h"
#include "node.h"

class solver {
public:
	solver(const parking& p);
	void solve();

private:
	const parking& p;
	std::vector<node> frontier;
	std::vector<node> checked_nodes;
};