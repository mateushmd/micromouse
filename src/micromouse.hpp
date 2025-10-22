#pragma once

#include <cstdint>
#include "node.hpp"
#include "path.hpp"

#define UP 0
#define RIGHT 1
#define DOWN 2
#define LEFT 3

#define byte uint8_t

void printMaze();
bool explore();
Path astar(byte* maze, byte start_x, byte start_y, byte target_x, byte target_y);
