#include "floodfill.hpp"
#include "API.h"
#include <cstdint>
#include <iostream>
#include <string>

#define MAZE_DIMENSION 16

#define W_U 0b0001
#define W_D 0b0010
#define W_L 0b0100
#define W_R 0b1000

#define UP 0
#define RIGHT 1
#define DOWN 2
#define LEFT 3

// clang-format off
uint8_t flood[16][16] = {
    {14, 13, 12, 11, 10,  9,  8,  7,  7,  8,  9, 10, 11, 12, 13, 14},
    {13, 12, 11, 10,  9,  8,  7,  6,  6,  7,  8,  9, 10, 11, 12, 13},
    {12, 11, 10,  9,  8,  7,  6,  5,  5,  6,  7,  8,  9, 10, 11, 12},
    {11, 10,  9,  8,  7,  6,  5,  4,  4,  5,  6,  7,  8,  9, 10, 11},
    {10,  9,  8,  7,  6,  5,  4,  3,  3,  4,  5,  6,  7,  8,  9, 10},
    { 9,  8,  7,  6,  5,  4,  3,  2,  2,  3,  4,  5,  6,  7,  8,  9},
    { 8,  7,  6,  5,  4,  3,  2,  1,  1,  2,  3,  4,  5,  6,  7,  8},
    { 7,  6,  5,  4,  3,  2,  1,  0,  0,  1,  2,  3,  4,  5,  6,  7},
    { 7,  6,  5,  4,  3,  2,  1,  0,  0,  1,  2,  3,  4,  5,  6,  7},
    { 8,  7,  6,  5,  4,  3,  2,  1,  1,  2,  3,  4,  5,  6,  7,  8},
    { 9,  8,  7,  6,  5,  4,  3,  2,  2,  3,  4,  5,  6,  7,  8,  9},
    {10,  9,  8,  7,  6,  5,  4,  3,  3,  4,  5,  6,  7,  8,  9, 10},
    {11, 10,  9,  8,  7,  6,  5,  4,  4,  5,  6,  7,  8,  9, 10, 11},
    {12, 11, 10,  9,  8,  7,  6,  5,  5,  6,  7,  8,  9, 10, 11, 12},
    {13, 12, 11, 10,  9,  8,  7,  6,  6,  7,  8,  9, 10, 11, 12, 13},
    {14, 13, 12, 11, 10,  9,  8,  7,  7,  8,  9, 10, 11, 12, 13, 14}
};

uint8_t maze[16][16] = {
    {W_D | W_L, W_D, W_D, W_D, W_D, W_D, W_D, W_D, W_D, W_D, W_D, W_D, W_D, W_D, W_D, W_D | W_R},
    {W_L,         0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,       W_R},
    {W_L,         0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,       W_R},
    {W_L,         0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,       W_R},
    {W_L,         0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,       W_R},
    {W_L,         0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,       W_R},
    {W_L,         0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,       W_R},
    {W_L,         0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,       W_R},
    {W_L,         0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,       W_R},
    {W_L,         0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,       W_R},
    {W_L,         0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,       W_R},
    {W_L,         0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,       W_R},
    {W_L,         0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,       W_R},
    {W_L,         0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,       W_R},
    {W_L,         0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,       W_R},
    {W_U | W_L, W_U, W_U, W_U, W_U, W_U, W_U, W_U, W_U, W_U, W_U, W_U, W_U, W_U, W_U, W_U | W_R}
};
// clang-format on

const int8_t dx[] = {0, 1, 0, -1};
const int8_t dy[] = {1, 0, -1, 0};
const uint8_t mouseWall[] = {W_U, W_R, W_D, W_L};
const uint8_t neighborWall[] = {W_D, W_L, W_U, W_R};

uint8_t x = 0;
uint8_t y = 0;
uint8_t rotation = UP;

API api;

void reflood()
{
    bool changed = true;
    while (changed)
    {
        changed = false;
        for (int y = 0; y < MAZE_DIMENSION; y++)
        {
            for (int x = 0; x < MAZE_DIMENSION; x++)
            {
                if (flood[x][y] == 0)
                    continue;

                uint8_t lowest = UINT8_MAX;

                if (y + 1 < MAZE_DIMENSION && !(maze[x][y] & W_U))
                    lowest =
                        (flood[x][y + 1] < lowest) ? flood[x][y + 1] : lowest;
                if (y - 1 > -1 && !(maze[x][y] & W_D))
                    lowest =
                        (flood[x][y - 1] < lowest) ? flood[x][y - 1] : lowest;
                if (x + 1 < MAZE_DIMENSION && !(maze[x][y] & W_R))
                    lowest =
                        (flood[x + 1][y] < lowest) ? flood[x + 1][y] : lowest;
                if (x - 1 > -1 && !(maze[x][y] & W_L))
                    lowest =
                        (flood[x - 1][y] < lowest) ? flood[x - 1][y] : lowest;

                if (lowest == 255)
                    continue;

                if (flood[x][y] != lowest + 1)
                {
                    flood[x][y] = lowest + 1;
                    changed = true;
                }
            }
        }
    }
}

bool move()
{
    uint8_t lowest_neighbor_val = 255;
    for (int d = 0; d < 4; d++)
    {
        int nx = x + dx[d];
        int ny = y + dy[d];

        if (nx < 0 || nx >= MAZE_DIMENSION || ny < 0 || ny >= MAZE_DIMENSION)
            continue;
        if (maze[x][y] & mouseWall[d])
            continue;

        if (flood[nx][ny] < lowest_neighbor_val)
        {
            lowest_neighbor_val = flood[nx][ny];
        }
    }

    uint8_t dir_forward = rotation;
    uint8_t dir_right = (rotation + 1) % 4;
    uint8_t dir_left = (rotation + 3) % 4;

    int nx = x + dx[dir_forward];
    int ny = y + dy[dir_forward];

    std::cerr << nx << " " << ny << std::endl;
    std::cerr << static_cast<int>(flood[x][y]) << " "
              << static_cast<int>(flood[nx][ny]) << std::endl;
    std::cerr << static_cast<int>(W_D) << " | " << static_cast<int>(W_L)
              << " = " << static_cast<int>(W_D | W_L) << std::endl;
    std::cerr << static_cast<int>(maze[x][y]) << " & "
              << static_cast<int>(mouseWall[dir_forward]) << " = "
              << static_cast<int>(maze[x][y] & mouseWall[dir_forward])
              << std::endl;
    if (!(maze[x][y] & mouseWall[dir_forward]) && flood[nx][ny] < flood[x][y])
    {
        std::cerr << "moving front" << std::endl;
        if (api.wallFront())
        {
            maze[x][y] |= mouseWall[dir_forward];
            maze[nx][ny] |= neighborWall[dir_forward];
        }
        else
        {
            api.moveForward();
            x = nx;
            y = ny;

            return true;
        }
    }

    nx = x + dx[dir_right];
    ny = y + dy[dir_right];
    if (!(maze[x][y] & mouseWall[dir_right]) && flood[nx][ny] < flood[x][y])
    {
        std::cerr << "moving right" << std::endl;
        if (api.wallRight())
        {
            maze[x][y] |= mouseWall[dir_right];
            maze[nx][ny] |= neighborWall[dir_right];
        }
        else
        {
            api.turnRight();
            api.moveForward();
            x = nx;
            y = ny;
            rotation = dir_right;
            return true;
        }
    }

    nx = x + dx[dir_left];
    ny = y + dy[dir_left];
    if (!(maze[x][y] & mouseWall[dir_left]) && flood[nx][ny] < flood[x][y])
    {
        std::cerr << "moving left" << std::endl;
        if (api.wallLeft())
        {
            maze[x][y] |= mouseWall[dir_left];
            maze[nx][ny] |= neighborWall[dir_left];
        }
        else
        {
            api.turnLeft();
            api.moveForward();
            x = nx;
            y = ny;
            rotation = dir_left;
            return true;
        }
    }

    if (lowest_neighbor_val != 255)
    {
        flood[x][y] = lowest_neighbor_val + 1;
    }

    std::cerr << "moving back" << std::endl;
    uint8_t dir_back = (rotation + 2) % 4;
    nx = x + dx[dir_back];
    ny = y + dy[dir_back];

    api.turnRight();
    api.turnRight();
    api.moveForward();

    x = nx;
    y = ny;
    rotation = dir_back;

    return false;
}

void start()
{
    for (int i = 0; i < MAZE_DIMENSION; i++)
    {
        for (int j = 0; j < MAZE_DIMENSION; j++)
        {
            api.setText(i, j, std::to_string(static_cast<int>(flood[i][j])));
        }
    }
}

void tick()
{
    if (!move())
        reflood();
}
