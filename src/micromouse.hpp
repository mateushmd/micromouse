#pragma once

#include <cstdint>
#include "path.hpp"

#define UP 0
#define RIGHT 1
#define DOWN 2
#define LEFT 3

#define target_x1 15
#define target_x2 15
#define target_y1 15
#define target_y2 15

#define byte uint8_t

void printMaze();
bool explore();
Path astar(byte* maze, byte start_x, byte start_y);
