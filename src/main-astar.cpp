#include "API.h"
#include "micromouse.hpp"
#include <iostream>

#define MAZE_DIMENSIONS 16
#define MAZE_SIZE 256
#define HALF_MAZE_SIZE 8
#define IDX2D(x, y) ((x) + (y) * MAZE_DIMENSIONS)

#define WALL_U 0b0001
#define WALL_D 0b0010
#define WALL_L 0b0100
#define WALL_R 0b1000

byte maze[MAZE_SIZE] = {
    WALL_D | WALL_L, WALL_D, WALL_D, WALL_D, WALL_D, WALL_D, WALL_D, WALL_D, WALL_D, WALL_D, WALL_D, WALL_D, WALL_D, WALL_D, WALL_D, WALL_D | WALL_R,
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
    WALL_U | WALL_L, WALL_U, WALL_U, WALL_U, WALL_U, WALL_U, WALL_U, WALL_U, WALL_U, WALL_U, WALL_U, WALL_U, WALL_U, WALL_U, WALL_U, WALL_U | WALL_R,
};

byte get_direction (byte cur_x, byte cur_y, byte target_x, byte target_y) {
	if (target_x > cur_x) return RIGHT;
	if (target_x < cur_x) return LEFT;
	if (target_y > cur_y) return UP;
	if (target_y < cur_y) return DOWN;
	return 4;
}

void face_direction (byte* facing, byte target) {
	API api;
	if (*facing != target) {
		if (target == UP) {
			if (*facing == RIGHT) api.turnLeft();
			else if (*facing == LEFT) api.turnRight();
			else {
				api.turnLeft();
				api.turnLeft();
			}
		} else if (target == RIGHT) {
			if (*facing == UP) api.turnRight();
			else if (*facing == DOWN) api.turnLeft();
			else {
				api.turnLeft();
				api.turnLeft();
			}
		} else if (target == LEFT) {
			if (*facing == UP) api.turnLeft();
			else if (*facing == DOWN) api.turnRight();
			else {
				api.turnLeft();
				api.turnLeft();
			}
		} else if (target == DOWN) {
			if (*facing == RIGHT) api.turnRight();
			else if (*facing == LEFT) api.turnLeft();
			else {
				api.turnLeft();
				api.turnLeft();
			}
		}
	}
	*facing = target;
}

void paintWalls () {
	API api;
	for (int x = 0; x < MAZE_DIMENSIONS; x++) {

		for (int y = 0; y < MAZE_DIMENSIONS; y++) {
			byte pos = maze[IDX2D(x, y)];
			if ( (pos & WALL_U) != 0) {
				api.setWall(x, y, 'n');
			}
			if ( (pos & WALL_R) != 0) {
				api.setWall(x, y, 'e');
			}
			if ( (pos & WALL_D) != 0) {
				api.setWall(x, y, 's');
			}
			if ( (pos & WALL_L) != 0) {
				api.setWall(x, y, 'w');
			}
		}
	}
}

void updateWalls (byte x, byte y, byte facing) {
	API api;
	if (api.wallFront()) {
		byte wall = 0;
		if (facing == UP) wall = WALL_U; 
		else if (facing == RIGHT) wall = WALL_R; 
		else if (facing == LEFT) wall = WALL_L; 
		else if (facing == DOWN) wall = WALL_D;

		maze[IDX2D(x, y)] |= wall;

		if (wall == WALL_U) maze[IDX2D(x, y+1)] |= WALL_D;
		else if (wall == WALL_D) maze[IDX2D(x, y-1)] |= WALL_U;
		else if (wall == WALL_R) maze[IDX2D(x+1, y)] |= WALL_L;
		else if (wall == WALL_L) maze[IDX2D(x-1, y)] |= WALL_R;
	}
	if (api.wallLeft()) {
		byte wall = 0;

		if (facing == UP) wall = WALL_L;
		else if (facing == RIGHT) wall = WALL_U;
		else if (facing == LEFT) wall = WALL_D;
		else if (facing == DOWN) wall = WALL_R;

		maze[IDX2D(x, y)] |= wall;

		if (wall == WALL_U) maze[IDX2D(x, y+1)] |= WALL_D; 
		else if (wall == WALL_D) maze[IDX2D(x, y-1)] |= WALL_U; 
		else if (wall == WALL_R) maze[IDX2D(x+1, y)] |= WALL_L;
		else if (wall == WALL_L) maze[IDX2D(x-1, y)] |= WALL_R;
	}
	if (api.wallRight()) {
		byte wall = 0;

		if (facing == UP) wall = WALL_R; 
		else if (facing == RIGHT) wall = WALL_D; 
		else if (facing == LEFT) wall = WALL_U;
		else if (facing == DOWN) wall = WALL_L;

		maze[IDX2D(x, y)] |= wall;

		if (wall == WALL_U) maze[IDX2D(x, y+1)] |= WALL_D; 
		else if (wall == WALL_D) maze[IDX2D(x, y-1)] |= WALL_U;
		else if (wall == WALL_R) maze[IDX2D(x+1, y)] |= WALL_L; 
		else if (wall == WALL_L) maze[IDX2D(x-1, y)] |= WALL_R; 
	}
}

bool checkWalls (byte x, byte y, byte dir) {

	bool res = false;
	if ((dir == UP && (maze[IDX2D(x, y)] & WALL_U) == 0)
	 	|| (dir == DOWN && (maze[IDX2D(x, y)] & WALL_D) == 0)
	 	|| (dir == RIGHT && (maze[IDX2D(x, y)] & WALL_R) == 0)
	 	|| (dir == LEFT && (maze[IDX2D(x, y)] & WALL_L) == 0)
	   ) res = true;

	return (res);
}

int main()
{
	API api;
	api.setColor(0, 0, 'G');
	api.setColor(7, 7, 'R');
	api.setColor(7, 8, 'R');
	api.setColor(8, 7, 'R');
	api.setColor(8, 8, 'R');

	byte I = 0;
	byte n = 90;
	Path path = astar(maze, 0, 0);

	while (I < n) {

		path = astar(maze, 0, 0);
		byte facing = UP;
		byte x = 0;
		byte y = 0;
		int i = 1;

		while (i < path.i) {

			Node v = path.arr[i++];
			std::cerr << "----------------------------------------\n";
			path.print();

			updateWalls(x, y, facing);
			byte dir = get_direction(x, y, v.x, v.y);
			bool can_move = checkWalls(x, y, dir);
			paintWalls();

			if (can_move) {
				face_direction(&facing, dir);
				api.moveForward();
				x = v.x;
				y = v.y;
			} else {
				api.clearAllText();
				path.update(astar(maze, x, y), --i);
			}
		}
		I++;
		api.clearAllText();
		api.ackReset();
	}
	path.print();
	return 0;
}
