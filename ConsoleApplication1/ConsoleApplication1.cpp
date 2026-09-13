#include <algorithm>
#include <atomic>
#include <charconv>
#include <chrono>
#include <cstddef>
#include <cstdio>
#include <exception>
#include <iostream>
#include <optional>
#include <print>
#include <string_view>
#include <thread>
#include <vector>

#include "parking.h"
#include "solver.h"

namespace {

void printUsage(std::string_view program)
{
	std::println(stderr, "usage: {} [--threads N] < input.txt", program);
	std::println(stderr, "Reads the test cases from standard input and prints the minimum number of");
	std::println(stderr, "moves for each one. By default one worker thread per CPU core is used.");
}

std::optional<unsigned> parsePositive(std::string_view text)
{
	unsigned value = 0;
	const auto [end, error] = std::from_chars(text.data(), text.data() + text.size(), value);
	if (error != std::errc{} || end != text.data() + text.size() || value == 0) {
		return std::nullopt;
	}
	return value;
}

} // namespace

int main(int argc, char* argv[])
{
	unsigned threadCount = std::max(1u, std::thread::hardware_concurrency());
	for (int i = 1; i < argc; i++) {
		const std::string_view arg = argv[i];
		if ((arg == "-t" || arg == "--threads") && i + 1 < argc) {
			const auto n = parsePositive(argv[++i]);
			if (!n) {
				std::println(stderr, "error: --threads needs a positive number");
				return 1;
			}
			threadCount = *n;
		}
		else {
			printUsage(argv[0]);
			return arg == "-h" || arg == "--help" ? 0 : 1;
		}
	}

	int T;
	if (!(std::cin >> T) || T < 0) {
		std::println(stderr, "error: expected the number of test cases on the first line");
		return 1;
	}
	std::vector<parking> puzzles;
	for (int i = 0; i < T; i++) {
		try {
			puzzles.push_back(parking::read(std::cin));
		}
		catch (const std::exception& e) {
			std::println(stderr, "error in test #{}: {}", i + 1, e.what());
			return 1;
		}
	}

	// A fixed pool of workers takes puzzles off a shared counter. Every worker writes
	// only to its own slot in `results`, so no locking is needed.
	std::vector<std::optional<int>> results(puzzles.size());
	std::atomic<std::size_t> next{ 0 };
	auto worker = [&]() {
		for (std::size_t i = next++; i < puzzles.size(); i = next++) {
			results[i] = solver(puzzles[i]).solve();
		}
	};

	threadCount = static_cast<unsigned>(std::clamp<std::size_t>(threadCount, 1, std::max<std::size_t>(1, puzzles.size())));
	const auto start = std::chrono::steady_clock::now();
	{
		std::vector<std::jthread> workers;
		for (unsigned i = 0; i < threadCount; i++) {
			workers.emplace_back(worker);
		}
	} // std::jthread joins when it goes out of scope
	const std::chrono::duration<double> elapsed = std::chrono::steady_clock::now() - start;

	for (std::size_t i = 0; i < results.size(); i++) {
		if (results[i]) {
			std::println("Test #{}: {}", i + 1, *results[i]);
		}
		else {
			std::println("Test #{}: no solution", i + 1);
		}
	}
	std::fflush(stdout);
	std::println(stderr, "solved {} puzzle(s) in {:.4f} seconds using {} thread(s)",
		puzzles.size(), elapsed.count(), threadCount);
	return 0;
}
