#include "CrabPch.h"

#include "Timer.h"

namespace crab
{

// - Timer

Timer::Timer()
    : m_timeStamp({})
{
    m_timeStamp.lastCount = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
}

void Timer::Reset()
{
    m_timeStamp = TimeStamp {};
    m_timeStamp.lastCount = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
}

void Timer::Tick()
{
    // - Update delta time
    uint64 currentCount   = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
    m_timeStamp.deltaTime = (currentCount - m_timeStamp.lastCount) / 1000000.0f;
    m_timeStamp.lastCount = currentCount;

    // - Update total time
    m_timeStamp.totalTime += m_timeStamp.deltaTime;

    // - Update frame count
    m_timeStamp.frameCounter++;
    m_timeStamp.frameTimeElapsed += m_timeStamp.deltaTime;
    if (m_timeStamp.frameTimeElapsed >= 1.0f)
    {
        m_timeStamp.framePerSec  = m_timeStamp.frameCounter;
        m_timeStamp.frameCounter = 0;
        m_timeStamp.frameTimeElapsed = std::fmodf(m_timeStamp.frameTimeElapsed, 1.0f);
    }

    // - Update elapsed time
    m_timeStamp.timeElapsed += m_timeStamp.deltaTime;
}

}   // namespace crab
