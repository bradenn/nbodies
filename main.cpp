/* *
 * @project nbodies
 * @file main.cpp
 *
 * @author Braden Nicholson
 * @date 3/8/21, 5/18/22
 */

#include "simulation.h"
#include <algorithm>

int main(int argc, char **argv) {
    // Run in headless mode if arguments are specified
    if (argc >= 5) {
        char *c;
        int maxTicks = (int) strtol(argv[1], &c, 10);
        float step = strtof(argv[2], &c);
        int mode = (int) strtol(argv[3], &c, 10);
        int threads = (int) strtol(argv[4], &c, 10);

        // Define the simulation parameters
        auto s = Settings{
                .maxTicks = maxTicks,
                .useParallel = (mode == 1),
                .useCollisions = false,
                .realtime = false,
                .targetFps = 0,
                .numThreads = threads,
                .maxThreads = threads,
                .radius = 1024,
                .scale = 1,
                .stepSize = step,
                .showControls = false,
                .rotP = 0,
                .rotY = 0,
        };

        // Initialize a simulation with the provided settings
        auto simulation = new Simulation(&s);
        // Run the simulation
        simulation->runHeadless();
    } else {
        printf("./nbodies or ./nbodies <maxTicks> <tickSize> <useParallel (1 or 0)> <numThreads (-1 for all)>\n");
        // Initialize the simulation with the default values
        auto simulation = new Simulation();
        // Run windowed
        simulation->run();
    }


    return 0;
}


