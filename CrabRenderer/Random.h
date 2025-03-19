#pragma once

namespace crab
{

//===================================================
// [in_min, in_max)
//===================================================

class Random
{
public:
    static int32 GenerateInt32(int32 in_min, int32 in_max);
    static float GenerateFloat(float in_min, float in_max);

private:
    struct Impl;
    static Scope<Impl> m_impl;
};

}
