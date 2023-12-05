#include <iostream>
#include <vector>
#include <thread>

#include "parking.h"
#include "solver.h"

int main()
{
	int T;
	std::vector<parking> parkings;

	std::cin >> T;

	for (int i = 0; i < T; i++) {
		int m, n, v;
		std::cin >> m >> n >> v;
		parking p(m, n, v);
		parkings.push_back(p);
	}
	for (const auto& p : parkings) {
		solver s(p);
		auto j = [&s] {s.solve(); };
		std::jthread{ j }.join();
	}
}