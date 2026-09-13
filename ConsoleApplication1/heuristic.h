#pragma once

#include "board.h"
#include "state.h"

// Lower bound on the number of moves still needed to free the red car:
//
//   h = 0                                        if the red car is at the exit
//   h = 1 + blockers + (1 if some blocker is stuck)  otherwise
//
// A blocker is a car standing between the red car and the exit. A blocker is stuck
// when it cannot clear the red car's lane in a single move because other cars are in
// its way. Every blocker has to move at least once, the red car has to move at least
// once, and freeing a stuck blocker needs at least one extra move by a car that is
// neither the red car nor a blocker. Only one extra move is counted, because a single
// car can be in the way of several blockers at once.
//
// The estimate never overestimates (admissible) and changes by at most 1 per move
// (consistent), so A* with a closed set returns the minimum number of moves.
class heuristic {
public:
	[[nodiscard]] static int estimate(const state& s, const board& b);
};
