#pragma once
#include "PassCommon.h"

namespace crab
{

class PBRPassBase
{
protected:
    enum eShaderResourceSlot
    {
        DISPLACEMENT_TEXTURE_SLOT = 0,
        ALBEDO_TEXTURE_SLOT       = 0,
        NORMAL_TEXTURE_SLOT       = 1,
        AO_TEXTURE_SLOT           = 2,
        METALLIC_TEXTURE_SLOT     = 3,
        ROUGHNESS_TEXTURE_SLOT    = 4,
        EMISSIVE_TEXTURE_SLOT     = 5,
    };
};

}   // namespace crab