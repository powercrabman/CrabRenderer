#pragma once

#include "CrabTypes.h"

//===================================================
// Constants
//===================================================

constexpr uint32 MAX_LIGHTS = 3;

//===================================================
// Enums 
//===================================================

//   Light Attribute (24 bit)   |  Light (8 bit, 256)
// 0000 0000 0000 0000 0000 0000 0000 0000
enum class eLightType : uint32
{
    None        = 0,
    Directional = 1,
    Point       = 2,
    Spot        = 3,

    ShadowMapping = BIT(8),
};

