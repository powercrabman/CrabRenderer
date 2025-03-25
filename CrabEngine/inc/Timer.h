#pragma once

namespace crab
{

struct TimeStamp
{
    uint64 lastCount   = 0;
    float  deltaTime   = 0.f;
    float  totalTime   = 0.f;
    float  timeElapsed = 0.f;
    uint32 framePerSec = 0;

    // - frame count helper
    uint32 frameCounter     = 0;
    float  frameTimeElapsed = 0.f;
};

class Timer
{
public:
    Timer();
    ~Timer() = default;

    void             Reset();
    void             Tick();
    const TimeStamp& GetTimeStamp() const { return m_timeStamp; };

private:
    TimeStamp m_timeStamp;
};

}   // namespace crab
