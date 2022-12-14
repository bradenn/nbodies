/* *
 * @project nbodies
 * @file simulation.cpp
 *
 * @author Braden Nicholson
 * @date 3/28/21, 4/28/22
 */

#ifndef SIMULATION_H
#define SIMULATION_H

#include "body.h"
#include "tester.h"
#include <utility>
#include <vector>

using std::vector;

struct Settings {
    int maxTicks = 0;
    bool useParallel = true;
    bool useCollisions = true;
    bool realtime = true;
    int targetFps = 60;
    int numThreads = 1;
    int maxThreads = 1;
    float radius = 1024;

    float scale = 1;
    float stepSize = 1;
    bool showControls = false;
    float rotP = 0, rotY = 0;
};


class Simulation {
private:


    vector<Body *> bodies;
    Graphics *graphics;
    Settings *settings = new Settings;
    TestManager *testManager;

    double tickBegin = 0.0;
    float currentStep = 0.0;
    float maxRSS = 0.0;
    bool running = true;
    float fps = 0.0;
    float ups = 0.0;
    int cycles = 0;

    // Core scaling
    double underflow{}, overflow{};

    void simulate();


    void showStats();

    void handleEvents();

    static float fmap(float value,
                      float istart,
                      float istop,
                      float ostart,
                      float ostop) {
        if (value < istart) return 0;
        if (value > istop) return istop / 2;
        return ostart +
               (ostop - ostart) * ((value - istart) / (istop - istart));
    }

public:
    Simulation();

    Simulation(Settings *s);

    void runHeadless();

    void run();

    void render();

    void recalculateCores();

};


#endif // SIMULATION_H
