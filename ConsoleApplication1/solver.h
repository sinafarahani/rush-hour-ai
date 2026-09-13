#pragma once

#include <optional>

#include "parking.h"

// A* search for a single puzzle. A solver owns all of its search data, so separate
// solvers can run on separate threads without any locking.
class solver {
public:
	explicit solver(parking p);

	// Minimum number of moves to get the red car to the exit, or nothing if the
	// puzzle has no solution.
	[[nodiscard]] std::optional<int> solve() const;

private:
	parking p;
};
