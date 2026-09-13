# Rush Hour AI

A fast, multithreaded C++23 solver for [Rush Hour](https://en.wikipedia.org/wiki/Rush_Hour_(puzzle)) sliding-block puzzles.
It uses **A\* search** with an admissible, consistent heuristic, so the answer it prints is always the **minimum number of moves**.

```
    1 2 3 4 5 6
 1  . . . . . .
 2  . . . A . .
 3  R R . A . .  →  exit
 4  . . . A . .
 5  . . B B B .
 6  . . . . . .
```

The red car `R` has to reach the right edge. `A` is in the way, and `A` can't move down until `B` does.
The solver finds the shortest solution: move `B` left, move `A` down, then drive `R` out. That's **3 moves**.

## Features

- **Optimal answers**: A\* with a heuristic that never overestimates and never jumps by more than one move between neighbouring states.
- **Object-oriented design**: every concept has its own class (`parking`, `state`, `board`, `node`, `heuristic`, `solver`).
- **Multithreaded**: test cases are solved in parallel on a fixed pool of worker threads, one per CPU core by default.
- **Fast**: compact 1-byte-per-car states, hash-based duplicate detection and a binary-heap frontier.
- **Any board size** up to 255×255, cars of any length, and a red car that is either horizontal or vertical.
- **Input validation**: malformed input, overlapping cars and cars outside the board are reported with the test and car number.

## Rules

- The board has `M` rows and `N` columns.
- Every car is 1 cell wide, has a length, and is either horizontal (`h`) or vertical (`v`).
- Cars only slide along their own direction and cannot pass through each other or leave the board.
- **One move = sliding one car any number of free cells.**
- The **first car** in the input is the red car. The puzzle is solved when it touches the far edge in its direction of travel:
  - a horizontal red car must reach the **right** edge.
  - a vertical red car must reach the **bottom** edge.

## Input format

The program reads from standard input:

```
T                            number of test cases
M N V                        rows, columns, number of cars   ┐ repeated
row col orientation length   one line per car (V lines)      ┘ T times
```

Coordinates are 1-based and `orientation` is `h` or `v`.

`row col` is the top-left cell of the car. Example with the puzzle from the top of this page:

```
1
6 6 3
3 1 h 2
2 4 v 3
5 3 h 3
```

## Output format

One line per test case, in input order:

```
Test #1: 3
Test #2: no solution
```

A summary line such as `solved 2 puzzle(s) in 0.0041 seconds using 2 thread(s)` goes to standard error, so standard output only contains results.

## Building

You need a C++23 compiler with `<print>` support: Visual Studio 2026 or 2022 (17.13+), GCC 14+, or Clang 18+.

### Visual Studio

Open `ConsoleApplication1.sln` and build the **Release | x64** configuration.
The project targets the Visual Studio 2026 toolset (`v145`) and C++23 (`/std:c++23preview`).
On Visual Studio 2022, right-click the solution and choose **Retarget solution** to switch to `v143`.

### CMake (Windows, Linux, macOS)

```sh
cmake -S . -B build
cmake --build build --config Release
```

The binary is `build/rush-hour` (or `build/Release/rush-hour.exe` with the Visual Studio generator).
Builds default to `Release`; the solver is many times slower in `Debug`.

## Usage

```sh
rush-hour < examples/input.txt
rush-hour --threads 1 < examples/input.txt   # limit the number of worker threads
rush-hour --help
```

`examples/input.txt` contains five puzzles, including an unsolvable one and one that needs 23 moves.
`examples/expected-output.txt` has the correct answers.

## How it works

### Classes

| Class       | File              | Responsibility |
|-------------|-------------------|----------------|
| `parking`   | `parking.h/.cpp`  | Static puzzle data: board size and each car's orientation, lane and length. Parses and validates input. |
| `state`     | `state.h/.cpp`    | One board configuration: each car's position along its lane, packed one byte per car. Hashable. |
| `board`     | `board.h/.cpp`    | Occupancy grid of a state; answers "how far can this car slide?". |
| `heuristic` | `heuristic.h/.cpp`| Lower bound on the moves still needed. |
| `node`      | `node.h/.cpp`     | Search node: a state, its depth `g` and heuristic value `h`; generates successor nodes. |
| `solver`    | `solver.h/.cpp`   | A\* search for one puzzle. |
| `main`      | `ConsoleApplication1.cpp` | Reads the input, runs the thread pool and prints the results. |

### State representation

A car can only move along its lane, so its row (horizontal car) or column (vertical car) never changes.
A state therefore only has to store **one number per car**: where the car starts along its lane.
These numbers are packed into a `std::string`, one byte each. Copying a state is cheap (small-string optimisation, no heap allocation for up to about 15 cars), and states can be hashed and compared directly.

### A\* search

`solver` keeps:

- a **frontier**: a `std::priority_queue` ordered by `f = g + h`. Ties go to the deeper node, which is usually closer to the goal.
- a **`bestDepth` hash map** from each generated state to the shortest known path length to it.

A successor is queued only if it is new or has been reached by a strictly shorter path. Outdated frontier entries are skipped when popped.
Because the heuristic is consistent, the first time a solved state is popped its depth is the optimal number of moves.
If the frontier runs out first, the puzzle has no solution.

When a node is expanded, the occupancy grid is built **once**. It is then updated in place for every successor, so the heuristic can be evaluated without allocating a new grid per move.

### Heuristic

```
h = 0                                        if the red car is at the exit
h = 1 + blockers + (1 if a blocker is stuck) otherwise
```

- **Blockers** are the cars that stand between the red car and the exit.
- A blocker is **stuck** when it can't clear the red car's lane in a single move because other cars block it in both directions.

Why this is a valid lower bound (**admissible**):

- The red car has to move at least once.
- Every blocker has to move at least once.
- A stuck blocker can only get out after some *other* car moves, and that car is neither the red car nor a blocker.

Only **one** extra move is added no matter how many blockers are stuck, because a single car can be in the way of several blockers at once. Counting one extra move per stuck blocker would overestimate, and A\* would then return non-optimal answers.

One move changes the number of blockers by at most one. It can't do that and also change the "stuck" term in the same direction, so `h` changes by at most 1 per move (**consistent**). That is what lets A\* close a state as soon as it is expanded.

### Multithreading

All puzzles are read before solving starts. A fixed pool of `std::jthread` workers, one per hardware thread by default, takes test cases from a shared `std::atomic` counter.
Each `solver` owns its frontier and hash map, and each worker writes only to its own slot in the results vector, so **no locks are needed**.
Results are printed in input order after all workers finish, so the output is identical regardless of the thread count.

## Performance

60 random 6×6 puzzles with 13 cars that need 12 to 23 moves, on a laptop:

| Threads | Time    |
|---------|---------|
| 1       | ~0.29 s |
| 5       | ~0.20 s |

The easy puzzles finish almost instantly, so most of the multithreaded time goes to the few hardest ones. Speed-up is largest when many hard puzzles are solved together.

## Project structure

```
.
├── CMakeLists.txt
├── ConsoleApplication1.sln
├── ConsoleApplication1/
│   ├── ConsoleApplication1.cpp      entry point, thread pool
│   ├── parking.h / parking.cpp      puzzle definition and input parsing
│   ├── state.h / state.cpp          board configuration
│   ├── board.h / board.cpp          occupancy grid
│   ├── heuristic.h / heuristic.cpp  A* heuristic
│   ├── node.h / node.cpp            search node, successor generation
│   └── solver.h / solver.cpp        A* search
└── examples/
    ├── input.txt
    └── expected-output.txt
```
