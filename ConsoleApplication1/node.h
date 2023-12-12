#pragma once

#include <algorithm>

#include "heuristic.h"
#include "state.h"

class node {
public:
	bool operator==(node& n) {
		return this->s == n.s;
	}
	bool operator==(const node& n) const {
		return this->s == n.s;
	}

	bool operator<(node& n) {
		return this->h + this->depth < n.h + n.depth;
	}
	bool operator<(const node& n) const {
		return this->h + this->depth < n.h + n.depth;
	}

	bool operator>(node& n) {
		return this->h + this->depth > n.h + n.depth;
	}
	bool operator>(const node& n) const {
		return this->h + this->depth > n.h + n.depth;
	}

	bool operator>=(node& n) {
		return !(this->h + this->depth < n.h + n.depth);
	}
	bool operator>=(const node& n) const {
		return !(this->h + this->depth < n.h + n.depth);
	}

	bool operator<=(node& n) {
		return !(this->h + this->depth > n.h + n.depth);
	}
	bool operator<=(const node& n) const {
		return !(this->h + this->depth > n.h + n.depth);
	}

	node(const state s, int d = 0);
	state getState() const;
	std::vector<node> expand();
	int getDepth() const;
private:
	int depth = 0;
	int h;
	state s;
};