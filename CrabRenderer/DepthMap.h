#pragma once
#include "RenderCommon.h"
#include "Textures.h"

namespace crab
{

class ShaderResource;
class DepthBuffer;

//===================================================
// Depth Only Buffer With ShaderResource
//===================================================

class DepthMap
{
public:
    void Clear(float in_clearValue) const;

    void BindDepthBuffer() const;
    void BindDepthMapTexture(uint32 in_slot, eShaderFlags in_bindFlags) const;

    Int2             GetResolution() const;
    Ref<DepthBuffer> GetDepthBuffer() const { return m_depthBuffer; }
    Ref<ShaderResource>     GetTexture() const { return m_texture; }
    eResourceType     GetTextureType() const { return m_texture->GetResourceType(); }

protected:
    DepthMap() = default;

    Ref<ShaderResource>     m_texture;
    Ref<DepthBuffer> m_depthBuffer;
};

//===================================================
// DepthMap ShaderResource
//===================================================
class DepthMapTexture : public DepthMap
{
public:
    void Init(uint32  in_width,
              uint32  in_height,
              eFormat in_depthFormat,
              eFormat in_textureFormat);
};

//===================================================
// DepthMap Array
//===================================================
class DepthMapArray : public DepthMap
{
public:
    void Init(uint32  in_width,
              uint32  in_height,
              uint32  in_arraySize,
              eFormat in_depthFormat,
              eFormat in_textureFormat);
};

//===================================================
// DepthMap Cube
//===================================================
class DepthMapCube : public DepthMap
{
public:
    void Init(uint32  in_width,
              uint32  in_height,
              eFormat in_depthFormat,
              eFormat in_textureFormat);
};

}   // namespace crab