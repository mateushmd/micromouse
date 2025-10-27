#include "floodfill.hpp"
#include "API.h"
#include <cstdint>
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
//	uint8_t flood[16][16];
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
    {W_D | W_L, W_L, W_L, W_L, W_L, W_L, W_L, W_L, W_L, W_L, W_L, W_L, W_L, W_L, W_L, W_U | W_L},
    {W_D,         0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,       W_U},
    {W_D,         0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,       W_U},
    {W_D,         0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,       W_U},
    {W_D,         0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,       W_U},
    {W_D,         0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,       W_U},
    {W_D,         0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,       W_U},
    {W_D,         0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,       W_U},
    {W_D,         0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,       W_U},
    {W_D,         0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,       W_U},
    {W_D,         0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,       W_U},
    {W_D,         0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,       W_U},
    {W_D,         0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,       W_U},
    {W_D,         0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,       W_U},
    {W_D,         0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,       W_U},
    {W_D | W_R, W_R, W_R, W_R, W_R, W_R, W_R, W_R, W_R, W_R, W_R, W_R, W_R, W_R, W_R, W_U | W_R}
};
// clang-format on

const int8_t dx[] = {0, 1, 0, -1};
const int8_t dy[] = {1, 0, -1, 0};
const uint8_t mouseWall[] = {W_U, W_R, W_D, W_L};
const uint8_t apiWall[] = {'n', 'e', 's', 'w'};
const uint8_t neighborWall[] = {W_D, W_L, W_U, W_R};

uint8_t x = 0;
uint8_t y = 0;
uint8_t rotation = UP;
bool firstRun = true;
bool secondAttempt = false;

API api;

bool isInBounds(int x, int y)
{
    return x >= 0 && x < MAZE_DIMENSION && y >= 0 && y < MAZE_DIMENSION;
}

struct Node {
	uint8_t x;
	uint8_t y;

	Node () {
		x = 255;
		y = 255;
	}

	Node (uint8_t x, uint8_t y) {
		this->x = x;
		this->y = y;
	}

	~Node() = default;
};

void reflood()
{
	Node stack [255] = {Node(x, y)};
	int stack_n = 1;

    while (stack_n > 0) {
		Node u = stack[--stack_n];
		
		if (flood[u.x][u.y] == 0)
			u = stack[--stack_n];

		uint8_t lowest = UINT8_MAX;
		uint8_t neightbours = 0b0000;

		if (u.y + 1 < MAZE_DIMENSION && !(maze[u.x][u.y] & W_U))
		{
			lowest = (flood[u.x][u.y + 1] < lowest) ? flood[u.x][u.y + 1] : lowest;
			neightbours |= 0b0001;
		}
		if (u.y - 1 > -1 && !(maze[u.x][u.y] & W_D))
		{
			lowest = (flood[u.x][u.y - 1] < lowest) ? flood[u.x][u.y - 1] : lowest;
			neightbours |= 0b0010;
		}
		if (u.x + 1 < MAZE_DIMENSION && !(maze[u.x][u.y] & W_R))
		{
			lowest = (flood[u.x + 1][u.y] < lowest) ? flood[u.x + 1][u.y] : lowest;
			neightbours |= 0b1000;
		}
		if (u.x - 1 > -1 && !(maze[u.x][u.y] & W_L))
		{
			lowest = (flood[u.x - 1][u.y] < lowest) ? flood[u.x - 1][u.y] : lowest;
			neightbours |= 0b0100;
		}

		if (lowest == 255)
			u = stack[--stack_n];

		if (flood[u.x][u.y] != lowest + 1) {
			flood[u.x][u.y] = lowest + 1;
			api.setText(u.x, u.y, std::to_string(static_cast<int>(flood[u.x][u.y])));

			if ((neightbours & 0b0001) != 0) {
				stack[stack_n++] = Node(u.x, u.y+1);
			}
			if ((neightbours & 0b0010) != 0) {
				stack[stack_n++] = Node(u.x, u.y-1);
			}
			if ((neightbours & 0b1000) != 0) {
				stack[stack_n++] = Node(u.x+1, u.y);
			}
			if ((neightbours & 0b0100) != 0) {
				stack[stack_n++] = Node(u.x-1, u.y);
			}
		}
    }
}

bool move()
{
    uint8_t dirForward = rotation;
    uint8_t dirRight = (rotation + 1) % 4;
    uint8_t dirLeft = (rotation + 3) % 4;

    int nx = x + dx[dirForward];
    int ny = y + dy[dirForward];

    if (api.wallFront())
    {
        maze[x][y] |= mouseWall[dirForward];
        if (isInBounds(nx, ny))
            maze[nx][ny] |= neighborWall[dirForward];
        api.setWall(x, y, apiWall[dirForward]);
    }

    nx = x + dx[dirRight];
    ny = y + dy[dirRight];

    if (api.wallRight())
    {
        maze[x][y] |= mouseWall[dirRight];
        if (isInBounds(nx, ny))
            maze[nx][ny] |= neighborWall[dirRight];
        api.setWall(x, y, apiWall[dirRight]);
    }

    nx = x + dx[dirLeft];
    ny = y + dy[dirLeft];

    if (api.wallLeft())
    {
        maze[x][y] |= mouseWall[dirLeft];
        if (isInBounds(nx, ny))
            maze[nx][ny] |= neighborWall[dirLeft];
        api.setWall(x, y, apiWall[dirLeft]);
    }

    nx = x + dx[dirForward];
    ny = y + dy[dirForward];

    if (!(maze[x][y] & mouseWall[dirForward]) && flood[nx][ny] < flood[x][y])
    {
        api.moveForward();
        x = nx;
        y = ny;
        return true;
    }

    nx = x + dx[dirRight];
    ny = y + dy[dirRight];

    if (!(maze[x][y] & mouseWall[dirRight]) && flood[nx][ny] < flood[x][y])
    {
        api.turnRight();
        api.moveForward();
        x = nx;
        y = ny;
        rotation = dirRight;
        return true;
    }

    nx = x + dx[dirLeft];
    ny = y + dy[dirLeft];

    if (!(maze[x][y] & mouseWall[dirLeft]) && flood[nx][ny] < flood[x][y])
    {
        api.turnLeft();
        api.moveForward();
        x = nx;
        y = ny;
        rotation = dirLeft;
        return true;
    }

    uint8_t lowestNeighborVal = 255;
    for (int d = 0; d < 4; d++)
    {
        int nx = x + dx[d];
        int ny = y + dy[d];

        if (nx < 0 || nx >= MAZE_DIMENSION || ny < 0 || ny >= MAZE_DIMENSION)
            continue;
        if (maze[x][y] & mouseWall[d])
            continue;

        if (flood[nx][ny] < lowestNeighborVal)
        {
            lowestNeighborVal = flood[nx][ny];
        }
    }

    if (lowestNeighborVal != 255)
    {
        api.setText(x, y, std::to_string(static_cast<int>(flood[x][y])));
        uint8_t walls =
            mouseWall[dirForward] | mouseWall[dirRight] | mouseWall[dirLeft];
        if (walls != (maze[x][y] & walls) && !secondAttempt)
        {
            secondAttempt = true;
            return false;
        }
    }

    uint8_t dirBack = (rotation + 2) % 4;
    nx = x + dx[dirBack];
    ny = y + dy[dirBack];

    api.turnRight();
    api.turnRight();

    rotation = dirBack;

    return false;
}

void start(int endX, int endY)
{
/*    if (firstRun)
    {
        for (int i = 0; i < MAZE_DIMENSION; i++)
            for (int j = 0; j < MAZE_DIMENSION; j++)
                flood[i][j] = UINT8_MAX;
        firstRun = false;
    }

    flood[endX][endY] = 0;*/

    reflood();
    for (int i = 0; i < MAZE_DIMENSION; i++)
    {
        for (int j = 0; j < MAZE_DIMENSION; j++)
        {
            api.setText(i, j, std::to_string(static_cast<int>(flood[i][j])));
            if (maze[i][j] & W_U)
                api.setWall(i, j, 'n');
            if (maze[i][j] & W_R)
                api.setWall(i, j, 'e');
            if (maze[i][j] & W_D)
                api.setWall(i, j, 's');
            if (maze[i][j] & W_L)
                api.setWall(i, j, 'w');
        }
    }
}

bool tick()
{
    if (!move())
        reflood();
    else
    {
        secondAttempt = false;
        if (flood[x][y] == 0)
            return false;
    }

    return true;
}

void reset()
{
    x = 0;
    y = 0;
    rotation = UP;
}
