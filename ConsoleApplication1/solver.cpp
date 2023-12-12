#include "solver.h"

solver::solver(const parking p)
	:
	p(p)
{

}

void solver::solve(int i)
{
	state s(p);
	node root(nullptr, s);
	frontier.push_back(root);
	while (!frontier.empty())
	{
		std::sort(frontier.begin(), frontier.end(), [](node& n1, node& n2) {return n1 > n2; });
		auto& cur = frontier.back();
		if (cur.getState().solved()) {
			std::cout << "Test #" << i << ": " << cur.getDepth() << std::endl;
			return;
		}
		checked_nodes.push_back(cur);
		auto nextNodes = cur.expand();
		frontier.pop_back();
		for (const auto& succesor : nextNodes) {
			for (const auto& existing : frontier) {
				if (existing == succesor) {
					if (existing > succesor) {
						frontier.erase(std::find(frontier.begin(), frontier.end(), existing));
						frontier.push_back(succesor);
					}
				}
			}
			if (std::find(checked_nodes.begin(), checked_nodes.end(), succesor) == checked_nodes.end() && std::find(frontier.begin(), frontier.end(), succesor) == frontier.end()) {
				frontier.push_back(succesor);
			}
		}
	}
}