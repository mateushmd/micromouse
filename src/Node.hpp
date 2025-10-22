#pragma once

#include "API.h"
#include <cstdint>

// Euclidian Distance # Altered since diagonal moves are not allowed
uint8_t heuristic (uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);
uint8_t heuristic_center (uint8_t x, uint8_t y);

struct Node {
	uint8_t x;
	uint8_t y;
	uint8_t G;
	uint8_t H;
	uint8_t dist;
	Node* parent;

	Node () {
		this->x = 0x7f;
		this->y = 0x7f;
		this->dist = 0x7f;
		this->G = 0x7f;
		this->H = 0x7f;
		this->dist = 0x7f;
		this->parent = nullptr;
	}

	Node (uint8_t x, uint8_t y, uint8_t G, Node* parent = nullptr) {
		this->x = x;
		this->y = y;
		this->G = G;
		this->H = heuristic_center(x, y);
		this->dist = this->H + this->G;
		this->parent = parent;
		API api;
		api.setText(x, y, std::to_string(G) + ", " + std::to_string(H));
		if (H == 0) {
			api.setColor(7, 7, 'R');
			api.setColor(7, 8, 'R');
			api.setColor(8, 7, 'R');
			api.setColor(8, 8, 'R');
			api.setColor(x, y, 'B');
		}
	}

	Node (uint8_t x, uint8_t y, uint8_t G, uint8_t H, Node* parent = nullptr) {
		this->x = x;
		this->y = y;
		this->G = G;
		this->H = H;
		this->dist = this->H + this->G;
		this->parent = parent;
	}

	~Node() = default;
};
