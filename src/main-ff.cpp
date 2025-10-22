#include "API.h"
#include "floodfill.hpp"

void run()
{
    bool run = true;
    start();
    while (run)
        run = tick();
}

int main(int argc, char *argv[])
{
    API api;
    for (int i = 0; i < 3; i++)
    {
        run();
        reset();
        api.ackReset();
    }
}
