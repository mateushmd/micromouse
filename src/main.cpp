#include "micromouse.hpp"
#include "API.h"
#include <iostream>

int main()
{
	API api;
	api.setColor(0, 0, 'G');
	api.setColor(7, 7, 'R');
	api.setColor(7, 8, 'R');
	api.setColor(8, 7, 'R');
	api.setColor(8, 8, 'R');

	astar();
/*	api.turnRight();
	api.turnRight();
	std::cerr << "Front: " << api.wallFront() << std::endl;
	std::cerr << "Back: " << api.wallBack() << std::endl;
	std::cerr << "Left: " << api.wallLeft() << std::endl;
	std::cerr << "Right: " << api.wallRight() << std::endl;*/

    return 0;
}
