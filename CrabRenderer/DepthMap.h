#pragma once
#include "CrabEnums.h"

namespace crab
{

class Texture;
class DepthBuffer;

class DepthMap
{
    D11_RESOURCE_CTOR(DepthMap);

public:
    static Ref<DepthMap> Create(
        uint32  in_width,
        uint32  in_height,
        eFormat in_depthFormat   = eFormat::Depth_Float32,
        eFormat in_textureFormat = eFormat::Float32);

    static Ref<DepthMap> CreateDepthMapArray(
        uint32  in_width,
        uint32  in_height,
        uint32  in_arraySize,
        eFormat in_depthFormat   = eFormat::Depth_Float32,
        eFormat in_textureFormat = eFormat::Float32);

    static Ref<DepthMap> CreateDepthMapCube(
        uint32  in_width,
        uint32  in_height,
        eFormat in_depthFormat   = eFormat::Depth_Float32,
        eFormat in_textureFormat = eFormat::Float32);

    void Clear(float in_clearValue) const;

    void BindDepthBuffer() const;
    void BindDepthMapTexture(uint32 in_slot, eShaderFlags in_bindFlags) const;

    Int2 GetResolution() const;
    Ref<Texture> GetTexture() const { return m_texture; }
    Ref<DepthBuffer> GetDepthBuffer() const { return m_depthBuffer; }

private:
    Ref<Texture>     m_texture;
    Ref<DepthBuffer> m_depthBuffer;
};

}   // namespace crab