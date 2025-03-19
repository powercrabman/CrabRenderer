#pragma once

#include "Types.h"

namespace crab
{

namespace color3
{

    constexpr Color3 BLACK      = { 0.f, 0.f, 0.f };
    constexpr Color3 WHITE      = { 1.f, 1.f, 1.f };
    constexpr Color3 RED        = { 1.f, 0.f, 0.f };
    constexpr Color3 GREEN      = { 0.f, 1.f, 0.f };
    constexpr Color3 BLUE       = { 0.f, 0.f, 1.f };
    constexpr Color3 YELLOW     = { 1.f, 1.f, 0.f };
    constexpr Color3 CYAN       = { 0.f, 1.f, 1.f };
    constexpr Color3 MAGENTA    = { 1.f, 0.f, 1.f };
    constexpr Color3 GRAY       = { 0.5f, 0.5f, 0.5f };
    constexpr Color3 LIGHT_GRAY = { 0.75f, 0.75f, 0.75f };
    constexpr Color3 DARK_GRAY  = { 0.25f, 0.25f, 0.25f };

}   // namespace color3

namespace color4
{
    constexpr Color4 BLACK      = { 0.f, 0.f, 0.f, 1.f };
    constexpr Color4 WHITE      = { 1.f, 1.f, 1.f, 1.f };
    constexpr Color4 RED        = { 1.f, 0.f, 0.f, 1.f };
    constexpr Color4 GREEN      = { 0.f, 1.f, 0.f, 1.f };
    constexpr Color4 BLUE       = { 0.f, 0.f, 1.f, 1.f };
    constexpr Color4 YELLOW     = { 1.f, 1.f, 0.f, 1.f };
    constexpr Color4 CYAN       = { 0.f, 1.f, 1.f, 1.f };
    constexpr Color4 MAGENTA    = { 1.f, 0.f, 1.f, 1.f };
    constexpr Color4 GRAY       = { 0.5f, 0.5f, 0.5f, 1.f };
    constexpr Color4 LIGHT_GRAY = { 0.75f, 0.75f, 0.75f, 1.f };
    constexpr Color4 DARK_GRAY  = { 0.25f, 0.25f, 0.25f, 1.f };

}   // namespace color4

inline Color3 LerpColor3(const Color3& a, const Color3& b, float t)
{
    return a * (1.f - t) + b * t;
}

inline Color4 LerpColor4(const Color4& a, const Color4& b, float t)
{
    return a * (1.f - t) + b * t;
}

inline Color4 ToColor4(const Color3& c, float a = 1.f)
{
    return { c.x, c.y, c.z, a };
}

inline Color3 ToColor3(const Color4& c)
{
    return { c.x, c.y, c.z };
}

}   // namespace crab