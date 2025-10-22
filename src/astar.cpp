#include <cmath>
#include <cstdio>
#include "micromouse.hpp"

#define MAZE_DIMENSIONS 16
#define IDX2D(x, y) ((x) + (y) * MAZE_DIMENSIONS)

#define WALL_U 0b0001
#define WALL_D 0b0010
#define WALL_L 0b0100
#define WALL_R 0b1000

// Manhatam distance
byte heuristic (byte x1, byte y1, byte x2, byte y2) {
	return ( abs(x2-x1) + abs(y2-y1) );
}

byte heuristic_center (byte x, byte y) {
	byte dx = 0;
	if (x < target_x1) dx = target_x1-x;
	else if (x > target_x2) dx = x-target_x2;

	byte dy = 0;
	if (y < target_y1) dy = target_y1-y;
	else if (y > target_y2) dy = y-target_y2;

	return (dx+dy);
}

void sort (Node** prio_queue, int pn) {
	for (int i = 1; i < pn; i++) {
		Node* tmp = prio_queue[i];
		int j = i-1;
		while ( j>=0 && ( (prio_queue[j]->dist > tmp->dist)
						  || (prio_queue[j]->dist == tmp->dist && prio_queue[j]->H > tmp->H)
					    ) 
		) {
			prio_queue[j+1] = prio_queue[j];
			j--;
		}
		prio_queue[j+1] = tmp;
	}
}

Path astar(byte* maze, byte start_x, byte start_y) {

	Node* prio_queue [64] = {nullptr};
	int prio_n = 0;
	bool visited[16][16] = {false};
	int vi = 0;

	Node* goal_node;
	Node node_pool[255];
	int node_n = 0;

	node_pool[node_n] = Node(start_x, start_y, 0, nullptr);
	prio_queue[prio_n++] = &node_pool[node_n++];

	Path path(255);

	while (prio_n > 0) {

		Node* u = prio_queue[0];
		prio_n--;
		for (int i = 0; i < prio_n; i++) {
			prio_queue[i] = prio_queue[i+1];
		}

		if( (u->x >= target_x1 && u->x <= target_x2) && (u->y >= target_y1 && u->y <= target_y2) ) {
			goal_node = u;
			prio_n = 0;
		} else {
			visited[u->x][u->y] = true;

			// No wall on top
			if ((maze[IDX2D(u->x, u->y)] & WALL_U) == 0 ) {
				if (!visited[u->x][u->y+1]) {
					node_pool[node_n] = Node(u->x, u->y+1, u->G+1, u);
					prio_queue[prio_n++] = &node_pool[node_n++];
				}
			}

			// No wall on the back
			if ((maze[IDX2D(u->x, u->y)] & WALL_D) == 0 ) {
				if (!visited[u->x][u->y-1]) {
					node_pool[node_n] = Node(u->x, u->y-1, u->G+1, u);
					prio_queue[prio_n++] = &node_pool[node_n++];
				}
			}

			// No wall on the right
			if ((maze[IDX2D(u->x, u->y)] & WALL_R) == 0 ) {
				if (!visited[u->x+1][u->y]) {
				 	node_pool[node_n] = Node(u->x+1, u->y, u->G+1, u);
					prio_queue[prio_n++] = &node_pool[node_n++];
				}
			}

			// No wall on the left
			if ((maze[IDX2D(u->x, u->y)] & WALL_L) == 0 ) {
				if (!visited[u->x-1][u->y]) {
					node_pool[node_n] = Node(u->x-1, u->y, u->G+1, u);
					prio_queue[prio_n++] = &node_pool[node_n++];
				}
			}

			sort(prio_queue, prio_n);
		}
	}

	if (goal_node) {
		Node* current = goal_node;
		while (current) {
			path.insert(*current);
			current = current->parent;
		}
		path.invert();
	}
	return (path);
}
