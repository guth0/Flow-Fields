// TODO:
//  use GLM (openGL math functions)
//  lines that are stright vertical/horizontal are't drawn?
//  maybe store the lines instead of creating them all each frame

#include "simulation.h"

int main()
{
    Simulation sim(124124124); // Enter seed up to 4,294,967,296 - 10
    sim.run();
    return 0;
}