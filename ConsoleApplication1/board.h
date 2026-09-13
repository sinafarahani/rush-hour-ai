#pragma once

#include <vector>

#include "state.h"

// Occupancy grid of a state. A node builds one grid when it is expanded and updates it
// in place for every successor, so generating successors needs no per-move allocations.
class board {
public:
	explicit board(const state& s);

	// Free cells directly before the car (above a vertical car, left of a horizontal one).
	int freeBefore(const parking::Car& c, int position) const;
	// Free cells directly after the car (below a vertical car, right of a horizontal one).
	int freeAfter(const parking::Car& c, int position) const;
	// Moves a car's cells from one position to another along its axis.
	void move(const parking::Car& c, int from, int to);

private:
	// Index of the cell at `along` on the car's axis.
	int cell(const parking::Car& c, int along) const;
	void fill(const parking::Car& c, int position, bool value);

	const parking& p;
	std::vector<char> cells;
};
