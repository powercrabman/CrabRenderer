#pragma once

#include "CrabTypes.h"

namespace crab
{
namespace color
{

    constexpr Color BLACK      = { 0.f, 0.f, 0.f, 1.f };
    constexpr Color WHITE      = { 1.f, 1.f, 1.f, 1.f };
    constexpr Color RED        = { 1.f, 0.f, 0.f, 1.f };
    constexpr Color GREEN      = { 0.f, 1.f, 0.f, 1.f };
    constexpr Color BLUE       = { 0.f, 0.f, 1.f, 1.f };
    constexpr Color YELLOW     = { 1.f, 1.f, 0.f, 1.f };
    constexpr Color CYAN       = { 0.f, 1.f, 1.f, 1.f };
    constexpr Color MAGENTA    = { 1.f, 0.f, 1.f, 1.f };
    constexpr Color GRAY       = { 0.5f, 0.5f, 0.5f, 1.f };
    constexpr Color LIGHT_GRAY = { 0.75f, 0.75f, 0.75f, 1.f };
    constexpr Color DARK_GRAY  = { 0.25f, 0.25f, 0.25f, 1.f };

}   // namespace color
}   // namespace crab