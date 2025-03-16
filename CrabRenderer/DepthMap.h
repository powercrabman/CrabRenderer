#pragma once
#include "CrabEnums.h"
#include "Textures.h"

namespace crab
{

class Texture;
class DepthBuffer;

class DepthMap
{
public:
    void Clear(float in_clearValue) const;

    void BindDepthBuffer() const;
    void BindDepthMapTexture(uint32 in_slot, eShaderFlags in_bindFlags) const;

    Int2             GetResolution() const;
    Ref<DepthBuffer> GetDepthBuffer() const { return m_depthBuffer; }
    Ref<Texture>     GetTexture() const { return m_texture; }
    eTextureType     GetTextureType() const { return m_texture->GetTextureType(); }

protected:
    DepthMap() = default;

    Ref<Texture>     m_texture;
    Ref<DepthBuffer> m_depthBuffer;
};

//===================================================
// DepthMap Texture
//===================================================
class DepthMapTexture : public DepthMap
{
public:
    void Init(
        uint32  in_width,
        uint32  in_height,
        eFormat in_depthFormat   = eFormat::Depth_Float32,
        eFormat in_textureFormat = eFormat::Float32);
};

//===================================================
// DepthMap Array
//===================================================
class DepthMapArray : public DepthMap
{
public:
    void Init(
        uint32  in_width,
        uint32  in_height,
        uint32  in_arraySize,
        eFormat in_depthFormat   = eFormat::Depth_Float32,
        eFormat in_textureFormat = eFormat::Float32);
};

//===================================================
// DepthMap Cube
//===================================================
class DepthMapCube : public DepthMap
{
public:
    void Init(
        uint32  in_width,
        uint32  in_height,
        eFormat in_depthFormat   = eFormat::Depth_Float32,
        eFormat in_textureFormat = eFormat::Float32);
};

}   // namespace crab