#include "systems/TimeManager.h"
#include <cstdio>

double TimeManager::CalculateFrameRate(bool writeToConsole) {
    static double fps = 0.0;
    static double startTime = GetTime();
    static double currentFPS = 0.0;

    ++fps;

    double currentTime = GetTime();
    if (currentTime - startTime > 1.0) {
        startTime = currentTime;
        if (writeToConsole) {
            std::fprintf(stderr, "Current Frames Per Second: %d\n", static_cast<int>(fps));
        }
        currentFPS = fps;
        fps = 0;
    }

    return currentFPS;
}

double TimeManager::GetTime() const {
    using namespace std::chrono;
    return duration_cast<duration<double>>(high_resolution_clock::now().time_since_epoch()).count();
}