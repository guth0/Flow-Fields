// TODO:
//  use GLM (openGL math functions)

#include "simulation.h"

int main()
{
    Simulation sim(5346); // Enter seed up to 4,294,967,296
    sim.run();
    return 0;
}