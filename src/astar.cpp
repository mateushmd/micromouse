#include "micromouse.hpp"
#include "API.h"
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <iostream>
#include <ostream>

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

#define target_x1 7
#define target_y1 7
#define target_x2 8
#define target_y2 8

uint8_t maze[MAZE_SIZE] = {
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

// Euclidian Distance # Altered since diagonal moves are not allowed
uint8_t heuristic (uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2) {
	return ( abs(x2-x1) + abs(y2-y1) );
}

void updateWalls (uint8_t x, uint8_t y, uint8_t facing) {

	API api;

	bool front = api.wallFront();
	bool left = api.wallLeft();
	bool right = api.wallRight();
	bool back = api.wallBack();

	if (facing == RIGHT) {
		bool tmp = right;
		right = front;
		front = left;
		left = back;
		back = tmp;
	}

	if (facing == LEFT) {
		bool tmp = left;
		left = front;
		front = right;
		right = back;
		back = tmp;
	}

	if (facing == DOWN) {
		bool tmp = front;
		front = back;
		back = tmp;
		tmp = right;
		right = left;
		left = tmp;
	}

	std::cerr << "------------------------" << std::endl;
	std::cerr << "front: " << front << std::endl;
	std::cerr << "back: " << back << std::endl;
	std::cerr << "left: " << left << std::endl;
	std::cerr << "right: " << right << std::endl;
	std::cerr << "------------------------" << std::endl;

	if (front) {
		maze[IDX2D(x, y)] |= WALL_U;
		if (y < 15) {
			maze[IDX2D(x, y+1)] |= WALL_D;
		}
	}
	if (back) {
		maze[IDX2D(x, y)] |= WALL_D;
		if (y > 0) {
			maze[IDX2D(x, y-1)] |= WALL_U;
		}
	}
	if (left) {
		maze[IDX2D(x, y)] |= WALL_L;
		if (x > 0) {
			maze[IDX2D(x-1, y)] |= WALL_R;
		}
	}
	if (right) {
		maze[IDX2D(x, y)] |= WALL_R;
		if (x < 15) {
			maze[IDX2D(x+1, y)] |= WALL_L;
		}
	}
}

void paintWalls () {
	API api;
	for (int x = 0; x < MAZE_DIMENSIONS; x++) {

		for (int y = 0; y < MAZE_DIMENSIONS; y++) {
			uint8_t pos = maze[IDX2D(x, y)];
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
struct Node {
	uint8_t x;
	uint8_t y;
	uint8_t dist;

	Node () {
		this->x = 0x7f;
		this->y = 0x7f;
		this->dist = 0x7f;
	}

	Node (uint8_t x, uint8_t y) {
		this->x = x;
		this->y = y;

		uint8_t h1 = heuristic(x, y, target_x1, target_y1);
		uint8_t h2 = heuristic(x, y, target_x2, target_y1);
		uint8_t h3 = heuristic(x, y, target_x1, target_y2);
		uint8_t h4 = heuristic(x, y, target_x2, target_y2);

		if (h1 > h2) {
			h1 = h2;
		}
		if (h1 > h3) {
			h1 = h3;
		}
		if (h1 > h4) {
			h1 = h4;
		}

		this->dist = h1;
	}

	~Node() = default;
};



void sort (Node* prio_queue, int pi, Node pos, uint8_t facing) {

	int i = 1;
	uint8_t dir = 4;

	for (int i = 1; i < pi; i++) {
		Node tmp = prio_queue[i];
		int j = i-1;

		if (heuristic(pos.x, pos.y, tmp.x, tmp.y) == 1) {
			if (prio_queue[i].x > pos.x) {
				dir = RIGHT;
			}
			if (prio_queue[i].x < pos.x) {
				dir = LEFT;
			}
			if (prio_queue[i].y > pos.y) {
				dir = UP;
			}
			if (prio_queue[i].y < pos.y) {
				dir = DOWN;
			}
		}

		while ( j>=0 && ( (prio_queue[j].dist > tmp.dist) || (prio_queue[j].dist == tmp.dist && dir == facing) ) ) {
			prio_queue[j+1] = prio_queue[j];
			j--;
		}
		prio_queue[j+1] = tmp;
	}
}

bool wasVisited (Node* visited, int vi, Node u) {
	bool status = false;
	int i = 0;
	while (i < vi && status == false) {
		status = (visited[i].x == u.x && visited[i].y == u.y);
		i++;
	}
	return (status);
}

void move(uint8_t* x, uint8_t* y, uint8_t* facing, Node next) {

	API api;
	if (next.x > *x) {
		while (*facing != RIGHT) {
			api.turnRight();
			*facing += 1;
			if (*facing > 3) {
				*facing = 0;
			}
		}
	}
	if (next.x < *x) {
		while (*facing != LEFT) {
			api.turnRight();
			*facing += 1;
			if (*facing > 3) {
				*facing = 0;
			}
		}
	}
	if (next.y > *y) {
		while (*facing != UP) {
			api.turnRight();
			*facing += 1;
			if (*facing > 3) {
				*facing = 0;
			}
		}
	}
	if (next.y < *y) {
		while (*facing != DOWN) {
			api.turnRight();
			*facing += 1;
			if (*facing > 3) {
				*facing = 0;
			}
		}
	}
	api.moveForward();

	*x = next.x;
	*y = next.y;
}

bool shouldReturn(Node u, Node next) {

	bool ret = false;
	uint8_t h = heuristic(u.x, u.y, next.x, next.y);

	if (h > 1) {
		ret = true;
	}
	std::cerr << "ux: " << (int)u.x << std::endl;
	std::cerr << "uy: " << (int)u.y << std::endl;
	std::cerr << "shouldRet: " << ret << std::endl;
	return (ret);
}

void astar() {

	API api;

	uint8_t facing = UP;

	uint8_t x = 0;
	uint8_t y = 0;

	Node prio_queue [64] = {Node(x, y)};
	int prio_i = 1;

	Node visited [MAZE_SIZE];
	int vi = 0;

	Node path [128];
	int path_i = 0;

	while ( (x != target_x1 && x != target_x2) || (y != target_y1 && y != target_x2)) {

		updateWalls(x, y, facing);
		paintWalls();

		Node u = prio_queue[0];
		prio_i--;
		for (int i = 0; i < prio_i; i++) {
			prio_queue[i] = prio_queue[i+1];
		}

		visited[vi++] = u;

		// If there is no wall on top
		if ( (maze[IDX2D(u.x, u.y)] & WALL_U) == 0) {
			Node v = Node(u.x, u.y+1);
			if (!wasVisited(visited, vi, v)) {
				prio_queue[prio_i++] = v;
			}
		}

		// If there is no wall on the back
		if ((maze[IDX2D(u.x, u.y)] & WALL_D) == 0) {
			Node v = Node(u.x, u.y-1);
			if (!wasVisited(visited, vi, v)) {
				prio_queue[prio_i++] = v;
			}
		}

		// If there is no wall on the left
		if ((maze[IDX2D(u.x, u.y)] & WALL_L) == 0) {
			Node v = Node(u.x-1, u.y);
			if (!wasVisited(visited, vi, v)) {
				prio_queue[prio_i++] = v;
			}
		}

		// If there is no wall on the right 
		if ((maze[IDX2D(u.x, u.y)] & WALL_R) == 0) {
			Node v = Node(u.x+1, u.y);
			if (!wasVisited(visited, vi, v)) {
				prio_queue[prio_i++] = v;
			}
		}

		sort(prio_queue, prio_i, u, facing);

		Node next = prio_queue[0];

		while (path_i > 0 && shouldReturn(u, next)) {
			move(&u.x, &u.y, &facing, path[--path_i]);
		} 
		path[path_i++] = u;

		for (int i = 0; i < path_i; i++) {
			std::fprintf(stderr, "(%d, %d)", path[i].x, path[i].y);
		}
		std::cerr << std::endl;

		std::fprintf(stderr, "nx: %d\nny: %d\nfacing: %d\n", next.x, next.y, facing);
		std::cerr << "dist: " << (int)next.dist << std::endl;

		move(&u.x, &u.y, &facing, next);

		x = u.x;
		y = u.y;
	}
}
