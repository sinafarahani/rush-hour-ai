#pragma once

#include <cstddef>
#include <string>

#include "parking.h"

// One configuration of the board: the position of every car along its axis.
// Positions are packed one byte per car into a std::string, so states are cheap to
// copy (small-string optimisation, no heap allocation for typical boards) and can be
// hashed with std::hash.
class state {
public:
	explicit state(const parking& p);

	bool operator==(const state& s) const;

	int getPosition(std::size_t car) const;
	state withPosition(std::size_t car, int position) const;
	bool solved() const;
	const parking& getParking() const;

	struct hash {
		std::size_t operator()(const state& s) const;
	};

private:
	const parking* p;
	std::string positions;
};
