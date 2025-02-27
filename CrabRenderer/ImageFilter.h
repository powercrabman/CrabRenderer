#pragma once
#include "D11ResourceList.h"

namespace crab
{

class FrameBufferList;
class VertexShader;
class PixelShader;
class ComputeShader;
class GeometryShader;
class SamplerState;
class RenderTarget;
class Mesh;
class Image2D;
class ConstantBufferBase;
class SamplerState;

class ImageFilter
{
    D11_RESOURCE_CTOR(ImageFilter);

public:
    static Ref<ImageFilter> Create(uint32                    in_filterWidth,
                                   uint32                    in_filterHeight,
                                   const Ref<VertexShader>&  in_vertexShader,
                                   const Ref<PixelShader>&   in_pixelShader,
                                   const Image2DList&        in_inputTextures,
                                   const SamplerList&   in_samplerLists,
                                   const ConstantList& in_constantBuffers);

    Ref<ImageFilter> Clone() const;

    void Bind() const;

    // Render Target
    void SetRenderTarget(const Ref<RenderTarget>& in_rt);

    // Getter
    auto         GetResolution() const;
    Ref<Image2D> GetOutputTexture() const;

private:
    Ref<RenderTarget> m_renderTarget;
    Viewport          m_viewport;

    Ref<VertexShader> m_vertexShader;
    Ref<PixelShader>  m_pixelShader;

    Image2DList        m_inputImages;
    SamplerList   m_samplerStates;
    ConstantList m_constantBuffers;
};

//===================================================
// Inline
//===================================================

inline auto ImageFilter::GetResolution() const
{
    uint32 width  = static_cast<uint32>(m_viewport.width);
    uint32 height = static_cast<uint32>(m_viewport.height);
    return std::make_pair(width, height);
}

}   // namespace crab