cmake_minimum_required(VERSION 3.20)

# Default to an optimised build (must be set before project(), which would pick Debug).
# Pass -DCMAKE_BUILD_TYPE=... to override; multi-config generators ignore it.
set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type")

project(rush_hour_ai LANGUAGES CXX)

find_package(Threads REQUIRED)

add_executable(rush-hour
	ConsoleApplication1/ConsoleApplication1.cpp
	ConsoleApplication1/board.cpp
	ConsoleApplication1/heuristic.cpp
	ConsoleApplication1/node.cpp
	ConsoleApplication1/parking.cpp
	ConsoleApplication1/solver.cpp
	ConsoleApplication1/state.cpp
)
target_compile_features(rush-hour PRIVATE cxx_std_23)
set_target_properties(rush-hour PROPERTIES CXX_EXTENSIONS OFF)
target_link_libraries(rush-hour PRIVATE Threads::Threads)

if(MSVC)
	target_compile_options(rush-hour PRIVATE /W4 /permissive-)
else()
	target_compile_options(rush-hour PRIVATE -Wall -Wextra -Wpedantic)
endif()
