#include "node.hpp"

struct Path {

	Node* arr;
	int i;
	int n;

	Path (int n) {
		this->n = n;
		if (n > 0) {
			this->arr = new Node[n];
		}
		this->i = 0;
	}

	void insert (Node u) {
		this->arr[i++] = u;
	}

	void invert (void) {
		int max = (int)(this->i/2.0);
		for (int i = 0; i < max; i++) {
			Node tmp = this->arr[i];
			this->arr[i] = this->arr[this->i-i-1];
			this->arr[this->i-i-1] = tmp;
		}
	}

	void update (Path new_path, int new_start) {
		for (int i = 0; i < new_path.i; i++) {
			this->arr[i-1+new_start] = new_path.arr[i];
		}
		this->i = new_start + new_path.i-1;
	}

	void print (void) {
		for (int i = 0; i < this->i; i++) {
			std::fprintf(stderr, "[%d, %d]\n", arr[i].x, arr[i].y);
		}
	}
};

