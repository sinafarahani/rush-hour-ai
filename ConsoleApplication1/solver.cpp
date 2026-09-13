#include "solver.h"

#include <functional>
#include <queue>
#include <unordered_map>
#include <utility>
#include <vector>

#include "node.h"
#include "state.h"

solver::solver(parking p)
	:
	p(std::move(p))
{
}

std::optional<int> solver::solve() const
{
	const state start(p);
	std::priority_queue<node, std::vector<node>, std::greater<node>> frontier;
	// Shortest known depth of every state generated so far.
	std::unordered_map<state, int, state::hash> bestDepth;

	frontier.push(node::root(start));
	bestDepth.emplace(start, 0);

	while (!frontier.empty()) {
		const node cur = frontier.top();
		frontier.pop();

		// A shorter path to this state was found after this entry was queued.
		if (cur.getDepth() > bestDepth.find(cur.getState())->second) {
			continue;
		}
		if (cur.getState().solved()) {
			return cur.getDepth();
		}

		for (node& child : cur.expand()) {
			auto [it, inserted] = bestDepth.try_emplace(child.getState(), child.getDepth());
			if (!inserted) {
				if (child.getDepth() >= it->second) {
					continue;
				}
				it->second = child.getDepth();
			}
			frontier.push(std::move(child));
		}
	}
	return std::nullopt;
}
