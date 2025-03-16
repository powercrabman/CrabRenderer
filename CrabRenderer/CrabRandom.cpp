#include "CrabPch.h"

#include "CrabRandom.h"

#include <random>

namespace crab
{

struct Random::Impl
{
    std::random_device rd;
    std::mt19937       gen;

    Impl()
        : gen(rd())
    {
    }
};

Scope<Random::Impl> Random::m_impl = CreateScope<Random::Impl>();

int32 Random::GenerateInt32(int32 in_min, int32 in_max)
{
    CRAB_ASSERT(in_min < in_max, "Invalid range.");
    std::uniform_int_distribution<int32> dist(in_min, in_max - 1);
    return dist(m_impl->gen);
}

float Random::GenerateFloat(float in_min, float in_max)
{
    CRAB_ASSERT(in_min < in_max, "Invalid range.");
    std::uniform_real_distribution<float> dist(in_min, in_max);
    return dist(m_impl->gen);
}

}   // namespace crab