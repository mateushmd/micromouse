#include "micromouse.hpp"
#include "API.h"
#include <cstdint>
#include <iostream>

#define MAZE_DIMENSIONS 16
#define MAZE_SIZE 256
#define HALF_MAZE_SIZE 8
#define IDX2D(x, y) ((x) + (y) * MAZE_DIMENSIONS)

#define WALL_U 0b0001
#define WALL_D 0b0010
#define WALL_L 0b0100
#define WALL_R 0b1000

#define UP 0
#define RIGHT 1
#define DOWN 2
#define LEFT 3

// clang-format off
uint8_t flood[] = {
    14, 13, 12, 11, 10, 9,  8,  7,  7,  8,  9,  10, 11, 12, 13, 14, 
    13, 12, 11, 10, 9,  8,  7,  6,  6,  7,  8,  9,  10, 11, 12, 13, 
    12, 11, 10, 9,  8,  7,  6,  5,  5,  6,  7,  8,  9,  10, 11, 12, 
    11, 10, 9,  8,  7,  6,  5,  4,  4,  5,  6,  7,  8,  9,  10, 11, 
    10, 9,  8,  7,  6,  5,  4,  3,  3,  4,  5,  6,  7,  8,  9,  10, 
    9,  8,  7,  6,  5,  4,  3,  2,  2,  3,  4,  5,  6,  7,  8,  9,  
    8,  7,  6,  5,  4,  3,  2,  1,  1,  2,  3,  4,  5,  6,  7,  8,  
    7,  6,  5,  4,  3,  2,  1,  0,  0,  1,  2,  3,  4,  5,  6,  7,  
    7,  6,  5,  4,  3,  2,  1,  0,  0,  1,  2,  3,  4,  5,  6,  7,  
    8,  7,  6,  5,  4,  3,  2,  1,  1,  2,  3,  4,  5,  6,  7,  8,  
    9,  8,  7,  6,  5,  4,  3,  2,  2,  3,  4,  5,  6,  7,  8,  9,  
    10, 9,  8,  7,  6,  5,  4,  3,  3,  4,  5,  6,  7,  8,  9,  10, 
    11, 10, 9,  8,  7,  6,  5,  4,  4,  5,  6,  7,  8,  9,  10, 11, 
    12, 11, 10, 9,  8,  7,  6,  5,  5,  6,  7,  8,  9,  10, 11, 12, 
    13, 12, 11, 10, 9,  8,  7,  6,  6,  7,  8,  9,  10, 11, 12, 13, 
    14, 13, 12, 11, 10, 9,  8,  7,  7,  8,  9,  10, 11, 12, 13, 14
};

uint8_t maze[MAZE_SIZE] = {
    WALL_U | WALL_L, WALL_U, WALL_U, WALL_U, WALL_U, WALL_U, WALL_U, WALL_U, WALL_U, WALL_U, WALL_U, WALL_U, WALL_U, WALL_U, WALL_U, WALL_U | WALL_R,
    WALL_L,          0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      WALL_R,
    WALL_L,          0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      WALL_R,
    WALL_L,          0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      WALL_R,
    WALL_L,          0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      WALL_R,
    WALL_L,          0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      WALL_R,
    WALL_L,          0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      WALL_R,
    WALL_L,          0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      WALL_R,
    WALL_L,          0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      WALL_R,
    WALL_L,          0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      WALL_R,
    WALL_L,          0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      WALL_R,
    WALL_L,          0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      WALL_R,
    WALL_L,          0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      WALL_R,
    WALL_L,          0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      WALL_R,
    WALL_L,          0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      WALL_R,
    WALL_D | WALL_L, WALL_D, WALL_D, WALL_D, WALL_D, WALL_D, WALL_D, WALL_D, WALL_D, WALL_D, WALL_D, WALL_D, WALL_D, WALL_D, WALL_D, WALL_D | WALL_R,
};
// clang-format on

uint8_t x = 0;
uint8_t y = 0;
uint8_t rotation = UP;

void reflood() {

}

void move(uint8_t vx, uint8_t vy) {}

bool explore()
{
	API api;
    if (rotation == UP)
    {
        if (!(maze[IDX2D(x, y)] & WALL_U) &&
            flood[IDX2D(x, y + 1)] < flood[IDX2D(x, y)])
        {
            if (api.wallFront())
            {
                maze[IDX2D(x, y)] |= WALL_U;
                maze[IDX2D(x, y + 1)] |= WALL_D;
            }
            else
            {
				// Go up
				api.moveForward();
                return true;
            }
        }

        if (!(maze[IDX2D(x, y)] & WALL_R) &&
            flood[IDX2D(x + 1, y)] < flood[IDX2D(x, y)])
        {
            if (api.wallRight())
            {
                maze[IDX2D(x, y)] |= WALL_R;
                maze[IDX2D(x + 1, y)] |= WALL_L;
            }
            else
            {
				// Go to the right 
				api.turnRight();
				api.moveForward();
                return true;
            }
        }

        if (!(maze[IDX2D(x, y)] & WALL_L) &&
            flood[IDX2D(x - 1, y)] < flood[IDX2D(x, y)])
        {
            if (api.wallLeft())
            {
                maze[IDX2D(x, y)] |= WALL_L;
                maze[IDX2D(x - 1, y)] |= WALL_R;
            }
            else
            {
				// Go to the left
				api.turnLeft();
				api.moveForward();
                return true;
            }
        }
    }
    else if (rotation == DOWN)
    {
        if (!(maze[IDX2D(x, y)] & WALL_D) &&
            flood[IDX2D(x, y - 1)] < flood[IDX2D(x, y)])
        {
			if (api.wallFront()) {
				maze[IDX2D(x, y)] |= WALL_D;
				maze[IDX2D(x, y - 1)] |= WALL_U;
            } else {
				// Go down
				api.moveForward();
				return true;
            }
        }

        if (!(maze[IDX2D(x, y)] & WALL_R) &&
            flood[IDX2D(x - 1, y)] < flood[IDX2D(x, y)])
        {
            if (api.wallRight())
            {
                maze[IDX2D(x, y)] |= WALL_L;
                maze[IDX2D(x - 1, y)] |= WALL_R;
            }
            else
            {
				// Go to the left
				api.turnRight();
				api.moveForward();
                return true;
            }
        }

        if (!(maze[IDX2D(x, y)] & WALL_L) &&
            flood[IDX2D(x + 1, y)] < flood[IDX2D(x, y)])
        {
            if (api.wallLeft())
            {
                maze[IDX2D(x, y)] |= WALL_R;
                maze[IDX2D(x + 1, y)] |= WALL_L;
            }
            else
            {
				// Go to the right
				api.turnLeft();
				api.moveForward();
                return true;
            }
        }
    }
    else if (rotation == LEFT)
    {
        if (!(maze[IDX2D(x, y)] & WALL_L) &&
            flood[IDX2D(x - 1, y)] < flood[IDX2D(x, y)])
        {
            if (api.wallFront())
            {
                maze[IDX2D(x, y)] |= WALL_L;
                maze[IDX2D(x - 1, y)] |= WALL_R;
            }
            else
            {
				// Go to the left
				api.moveForward();
                return true;
            }
        }

        if (!(maze[IDX2D(x, y)] & WALL_U) &&
            flood[IDX2D(x, y + 1)] < flood[IDX2D(x, y)])
        {
            if (api.wallRight())
            {
                maze[IDX2D(x, y)] |= WALL_U;
                maze[IDX2D(x, y + 1)] |= WALL_D;
            }
            else
            {
				// Go up
				api.turnRight();
				api.moveForward();
                return true;
            }
        }

        if (!(maze[IDX2D(x, y)] & WALL_D) &&
            flood[IDX2D(x, y - 1)] < flood[IDX2D(x, y)])
        {
            if (api.wallLeft())
            {
                maze[IDX2D(x, y)] |= WALL_D;
                maze[IDX2D(x, y - 1)] |= WALL_U;
            }
            else
            {
				// Go down
				api.turnLeft();
				api.moveForward();
                return true;
            }
        }
    }
    else // RIGHT
    {
        if (!(maze[IDX2D(x, y)] & WALL_R) &&
            flood[IDX2D(x + 1, y)] < flood[IDX2D(x, y)])
        {
            if (api.wallFront())
            {
                maze[IDX2D(x, y)] |= WALL_D;
                maze[IDX2D(x + 1, y)] |= WALL_L;
            }
            else
            {
				// Go to the right
				api.moveForward();
                return true;
            }
        }
        if (!(maze[IDX2D(x, y)] & WALL_D) &&
            flood[IDX2D(x, y - 1)] < flood[IDX2D(x, y)])
        {
            if (api.wallRight())
            {
                maze[IDX2D(x, y)] |= WALL_D;
                maze[IDX2D(x, y - 1)] |= WALL_U;
            }
            else
            {
				// Go down
				api.turnRight();
				api.moveForward();
                return true;
            }
        }

        if (!(maze[IDX2D(x, y)] & WALL_U) &&
            flood[IDX2D(x, y + 1)] < flood[IDX2D(x, y)])
        {
            if (api.wallLeft())
            {
                maze[IDX2D(x, y)] |= WALL_U;
                maze[IDX2D(x, y + 1)] |= WALL_D;
            }
            else
            {
				// Go up
				api.turnLeft();
				api.moveForward();
                return true;
            }
        }
    }
    return false;
}

void printMaze()
{
    for (int i = 0; i < MAZE_DIMENSIONS; i++)
    {
        for (int j = 0; j < MAZE_DIMENSIONS; j++)
     {
            std::cerr << (int)flood[IDX2D(i, j)] << ", ";
            std::cout << "setText " << i << " " << j << " " << (int) flood[IDX2D(i, j)] << std::endl;
        }
        std::cerr << std::endl;
    }
}
