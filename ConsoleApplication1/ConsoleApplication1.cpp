#include <chrono>
#include <iostream>
#include <thread>
#include <vector>

#include "parking.h"
#include "solver.h"

int main()
{
	int T;
	//std::vector<solver> parkingSolvers;
	std::vector<std::thread> solvers;
	std::cin >> T;

	for (int i = 0; i < T; i++) {
		int m, n, v;
		std::cin >> m >> n >> v;
		parking p(m, n, v);
		solver s(p);
		auto j = [=]() mutable {s.solve(i+1); };
		solvers.push_back(std::thread{ j });
	}
	auto now = std::chrono::high_resolution_clock::now();
	for (auto& w : solvers) {
		w.join();
	}
	auto next = std::chrono::high_resolution_clock::now();

	auto time = std::chrono::duration<float>(next - now).count();
	std::cout << "calculation took " << time << " seconds";
}