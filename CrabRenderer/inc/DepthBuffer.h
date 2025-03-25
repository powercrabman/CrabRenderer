#pragma once

namespace crab
{
class TextureShaderResource;

class ShaderResource;

class DepthBuffer
{
public:
    //===================================================
    // Init
    //===================================================

    void Init(uint32                in_width,
              uint32                in_height,
              eFormat               in_depthBufferFormat,
              MSAA                  in_MSAA,
              uint32                in_textureArraySize,
              eTextureCreationFlags in_flags);

    void InitForDepthMap(uint32                in_width,
                         uint32                in_height,
                         eFormat               in_depthBufferFormat,
                         eFormat               in_shaderResourceFormat,
                         MSAA                  in_MSAA,
                         uint32                in_textureArraySize,
                         eTextureCreationFlags in_flags);

    //===================================================
    // Function
    //===================================================

    void Clear(bool  in_clearDepth,
               float in_clearDepthFactor,
               bool  in_clearStencil,
               int32 in_clearStencilFactor) const;

    ID3D11DepthStencilView* GetDSV() const
    {
        return m_dsv.Get();
    }

    IVec2 GetResolution() const
    {
        return m_resolution;
    }

    float GetAspect() const
    {
        return static_cast<float>(m_resolution.x) / static_cast<float>(m_resolution.y);
    }

    eFormat GetFormat() const
    {
        return m_format;
    }

    Ref<TextureShaderResource> GetShaderResource() const
    {
        return m_srv;
    }

private:
    void Init_Internal(ID3D11Texture2D* in_texture,
                       eFormat          in_depthBufferFormat,
                       bool             in_createShaderResource,
                       eFormat          in_shaderResourceFormat);

    ComPtr<ID3D11DepthStencilView> m_dsv;
    Ref<TextureShaderResource>     m_srv;

    IVec2   m_resolution;
    eFormat m_format;
};

}   // namespace crab