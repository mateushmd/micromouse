#include "micromouse.hpp"
#include <cstdint>
#include <iostream>

#define MAZE_DIMENSIONS 16
#define MAZE_SIZE 256
#define HALF_MAZE_SIZE 8
#define IDX2D(x, y) ((x) + (y) * MAZE_SIZE)

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

bool checkSensorUp() { return false; }

bool checkSensorLeft() { return false; }

bool checkSensorRight() { return false; }

void reflood() {
    bool changed = true;
    while(changed)
    {
        for(int i = 0; i < MAZE_DIMENSIONS; i++)
        {
            for(int j = 0; j < MAZE_DIMENSIONS; j++)
            {
            }
        }
    }
}

void move(uint8_t vx, uint8_t vy) {}

bool explore()
{
    uint8_t lowest = UINT8_MAX;
    if (rotation == UP)
    {
        if (flood[IDX2D(x, y + 1)] < lowest)
            lowest = flood[IDX2D(x, y + 1)];

        if (!(maze[IDX2D(x, y)] & WALL_U) &&
            flood[IDX2D(x, y + 1)] < flood[IDX2D(x, y)])
        {
            if (checkSensorUp())
            {
                maze[IDX2D(x, y)] |= WALL_U;
                maze[IDX2D(x, y + 1)] |= WALL_D;
            }
            else
            {
                move(0, 1);
                return true;
            }
        }

        if (flood[IDX2D(x + 1, y)] < lowest)
            lowest = flood[IDX2D(x, y + 1)];

        if (!(maze[IDX2D(x, y)] & WALL_R) &&
            flood[IDX2D(x + 1, y)] < flood[IDX2D(x, y)])
        {
            if (checkSensorRight())
            {
                maze[IDX2D(x, y)] |= WALL_R;
                maze[IDX2D(x + 1, y)] |= WALL_L;
            }
            else
            {
                move(1, 0);
                return true;
            }
        }

        if (flood[IDX2D(x - 1, y)] < lowest)
            lowest = flood[IDX2D(x - 1, y)];

        if (!(maze[IDX2D(x, y)] & WALL_L) &&
            flood[IDX2D(x - 1, y)] < flood[IDX2D(x, y)])
        {
            if (checkSensorLeft())
            {
                maze[IDX2D(x, y)] |= WALL_L;
                maze[IDX2D(x - 1, y)] |= WALL_R;
            }
            else
            {
                move(-1, 0);
                return true;
            }
        }
    }
    else if (rotation == DOWN)
    {
        if (flood[IDX2D(x, y - 1)] < lowest)
            lowest = flood[IDX2D(x, y - 1)];

        if (!(maze[IDX2D(x, y)] & WALL_D) &&
            flood[IDX2D(x, y - 1)] < flood[IDX2D(x, y)])
        {
            if (checkSensorUp())
            {
                maze[IDX2D(x, y)] |= WALL_D;
                maze[IDX2D(x, y - 1)] |= WALL_U;
            }
            else
            {
                move(0, -1);
                return true;
            }
        }

        if (flood[IDX2D(x - 1, y)] < lowest)
            lowest = flood[IDX2D(x - 1, y)];

        if (!(maze[IDX2D(x, y)] & WALL_R) &&
            flood[IDX2D(x - 1, y)] < flood[IDX2D(x, y)])
        {
            if (checkSensorRight())
            {
                maze[IDX2D(x, y)] |= WALL_L;
                maze[IDX2D(x - 1, y)] |= WALL_R;
            }
            else
            {
                move(-1, 0);
                return true;
            }
        }

        if (flood[IDX2D(x + 1, y)] < lowest)
            lowest = flood[IDX2D(x + 1, y)];

        if (!(maze[IDX2D(x, y)] & WALL_L) &&
            flood[IDX2D(x + 1, y)] < flood[IDX2D(x, y)])
        {
            if (checkSensorLeft())
            {
                maze[IDX2D(x, y)] |= WALL_R;
                maze[IDX2D(x + 1, y)] |= WALL_L;
            }
            else
            {
                move(1, 0);
                return true;
            }
        }
    }
    else if (rotation == LEFT)
    {

        if (flood[IDX2D(x - 1, y)] < lowest)
            lowest = flood[IDX2D(x - 1, y)];

        if (!(maze[IDX2D(x, y)] & WALL_L) &&
            flood[IDX2D(x - 1, y)] < flood[IDX2D(x, y)])
        {
            if (checkSensorUp())
            {
                maze[IDX2D(x, y)] |= WALL_L;
                maze[IDX2D(x - 1, y)] |= WALL_R;
            }
            else
            {
                move(-1, 0);
                return true;
            }
        }

        if (flood[IDX2D(x, y + 1)] < lowest)
            lowest = flood[IDX2D(x, y + 1)];

        if (!(maze[IDX2D(x, y)] & WALL_U) &&
            flood[IDX2D(x, y + 1)] < flood[IDX2D(x, y)])
        {
            if (checkSensorRight())
            {
                maze[IDX2D(x, y)] |= WALL_U;
                maze[IDX2D(x, y + 1)] |= WALL_D;
            }
            else
            {
                move(0, 1);
                return true;
            }
        }

        if (flood[IDX2D(x, y - 1)] < lowest)
            lowest = flood[IDX2D(x, y - 1)];

        if (!(maze[IDX2D(x, y)] & WALL_D) &&
            flood[IDX2D(x, y - 1)] < flood[IDX2D(x, y)])
        {
            if (checkSensorLeft())
            {
                maze[IDX2D(x, y)] |= WALL_D;
                maze[IDX2D(x, y - 1)] |= WALL_U;
            }
            else
            {
                move(0, -1);
                return true;
            }
        }
    }
    else // RIGHT
    {
        if (flood[IDX2D(x + 1, y)] < lowest)
            lowest = flood[IDX2D(x + 1, y)];

        if (!(maze[IDX2D(x, y)] & WALL_R) &&
            flood[IDX2D(x + 1, y)] < flood[IDX2D(x, y)])
        {
            if (checkSensorUp())
            {
                maze[IDX2D(x, y)] |= WALL_D;
                maze[IDX2D(x + 1, y)] |= WALL_L;
            }
            else
            {
                move(1, 0);
                return true;
            }
        }

        if (flood[IDX2D(x, y - 1)] < lowest)
            lowest = flood[IDX2D(x, y - 1)];

        if (!(maze[IDX2D(x, y)] & WALL_D) &&
            flood[IDX2D(x, y - 1)] < flood[IDX2D(x, y)])
        {
            if (checkSensorRight())
            {
                maze[IDX2D(x, y)] |= WALL_D;
                maze[IDX2D(x, y - 1)] |= WALL_U;
            }
            else
            {
                move(0, -1);
                return true;
            }
        }

        if (flood[IDX2D(x, y + 1)] < lowest)
            lowest = flood[IDX2D(x, y + 1)];

        if (!(maze[IDX2D(x, y)] & WALL_U) &&
            flood[IDX2D(x, y + 1)] < flood[IDX2D(x, y)])
        {
            if (checkSensorLeft())
            {
                maze[IDX2D(x, y)] |= WALL_U;
                maze[IDX2D(x, y + 1)] |= WALL_D;
            }
            else
            {
                move(0, 1);
                return true;
            }
        }
    }
    maze[IDX2D(x, y)] = lowest + 1;
    return false;
}

void printMaze()
{
    for (int i = 0; i < MAZE_SIZE; i++)
    {
        for (int j = 0; j < MAZE_SIZE; j++)
        {
            std::cout << flood[IDX2D(i, j)] << ", ";
        }
        std::cout << std::endl;
    }
}
