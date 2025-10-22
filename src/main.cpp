#include "API.h"
#include "micromouse.hpp"

int main()
{
	API api;
	api.setColor(0, 0, 'G');
	api.setColor(7, 7, 'R');
	api.setColor(7, 8, 'R');
	api.setColor(8, 7, 'R');
	api.setColor(8, 8, 'R');
    return 0;
}
