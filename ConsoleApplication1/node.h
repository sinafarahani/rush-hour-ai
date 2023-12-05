#pragma once

#include "state.h"
#include "heuristic.h"

class node {
public:
	bool operator==(node& n) {
		return this->s == n.s;
	}
	bool operator==(const node& n) const{
		return this->s == n.s;
	}

	bool operator!=(node& n) {
		return this->s != n.s;
	}
	bool operator!=(const node& n) const {
		return this->s != n.s;
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

	node(node* parent, state s);
	state getState();
	int getHeuristic() const;
	std::vector<node> expand();
	int getDepth() const;
	node* getParent() const;
private:
	int depth = 0;
	int h;
	state s;
	node* parent;
};