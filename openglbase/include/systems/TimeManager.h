#ifndef TIME_MANAGER_H
#define TIME_MANAGER_H

#include "core/ISystem.h"
#include <chrono>
#include <thread>
#include <atomic>
#include <cstdio>

class TimeManager : public ISystem {
public:
    TimeManager() : m_DeltaTime(0.0), m_LastTime(GetTime()) {}
    ~TimeManager() = default;

    // Singleton instance
    static TimeManager& GetInstance() {
        static TimeManager instance; // No need for a unique_ptr
        return instance;
    }

    // Deleted copy and move constructors and assignment operators
    TimeManager(const TimeManager&) = delete;
    TimeManager(TimeManager&&) = delete;
    TimeManager& operator=(const TimeManager&) = delete;
    TimeManager& operator=(TimeManager&&) = delete;

    void Update(float deltaTime) override { UpdateDeltaTime(); }

    // Calculates our current scene's frames per second and displays it in the console
    double CalculateFrameRate(bool writeToConsole);

    // Returns the current time in seconds since epoch
    double GetTime() const;

    // Get the delta time for the current frame
    double GetDeltaTime() const { return m_DeltaTime.load(); }

    

    // Pauses the current thread for an amount of time in milliseconds
    void Sleep(int milliseconds) const {
        std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
    }
private:
    // Updates delta time based on the current time
    void UpdateDeltaTime() {
        double currentTime = GetTime();
        double lastTime = m_LastTime.exchange(currentTime); // Atomically update last time
        m_DeltaTime.store(currentTime - lastTime);
    }
private:
    std::atomic<double> m_DeltaTime;
    std::atomic<double> m_LastTime;
};

#endif // TIME_MANAGER_H