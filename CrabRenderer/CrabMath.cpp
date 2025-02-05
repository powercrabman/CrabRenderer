#include "CrabPch.h"

#include "CrabMath.h"

#include <random>

namespace crab
{

namespace RandomGenerator
{
    std::random_device rd;
    std::mt19937       gen(rd());
}   // namespace RandomGenerator

int32 RandomInt(int32 in_min, int32 in_max)
{
    std::uniform_int_distribution<int32> dist(in_min, in_max - 1);
    return dist(RandomGenerator::gen);
}

float RandomFloat(float in_min, float in_max)
{
    std::uniform_real_distribution<float> dist(in_min, in_max);
    return dist(RandomGenerator::gen);
}

}   // namespace crab
