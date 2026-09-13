#pragma once

#include <istream>
#include <vector>

// The static part of a puzzle: the board size and the fixed properties of every car.
// Everything that changes during the search (car positions) lives in `state`.
// Coordinates are 0-based internally; the input format is 1-based.
class parking {
public:
	class Car {
	public:
		bool horizontal;
		int line;   // row of a horizontal car, column of a vertical car; never changes
		int length;
	};

	// `state` stores one byte per car position, which limits the board size.
	static constexpr int maxBoardSize = 255;

	// Reads one puzzle: "M N V" followed by V lines of "row col h|v length".
	// The first car is the red car. Throws std::runtime_error on invalid input.
	[[nodiscard]] static parking read(std::istream& in);

	int getM() const;
	int getN() const;
	const std::vector<Car>& getCars() const;
	const Car& getRedCar() const;
	const std::vector<int>& getStartPositions() const;
	// Number of cells along the axis the car slides on.
	int axisLength(const Car& c) const;

private:
	int M = 0; // rows
	int N = 0; // columns
	std::vector<Car> cars;
	std::vector<int> startPositions; // first cell of each car along its axis
};
