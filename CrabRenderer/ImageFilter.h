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
    static Ref<ImageFilter> Create(uint32                   in_filterWidth,
                                   uint32                   in_filterHeight,
                                   const Ref<VertexShader>& in_vertexShader,
                                   const Ref<PixelShader>&  in_pixelShader);

    Ref<ImageFilter> Clone() const;

    void Bind() const;
    void SetOnBindCallback(const std::function<void()>& in_func) { m_onBindFunc = in_func; }

    // Render Target
    void SetRenderTarget(const Ref<RenderTarget>& in_rt);

    // Image Filter Resources
    void ClearInputImages() { m_inputImages.ClearList(); }
    void ClearConstantBuffers() { m_constantBuffers.ClearList(); }
    void ClearSamplerStates() { m_samplerStates.ClearList(); }

    void AddInputImage(const Ref<Image2D>& in_textures, uint32 in_slot);
    void AddConstantBuffer(const Ref<ConstantBufferBase>& in_buffer, uint32 in_slot);
    void AddSamplerState(const Ref<SamplerState>& in_samplerState, uint32 in_slot);

    ConstantBufferList GetConstantBufferList() const { return m_constantBuffers; }

    // Getter
    auto         GetResolution() const;
    Ref<Image2D> GetOutputTexture() const;

private:
    Ref<RenderTarget> m_renderTarget;
    Viewport          m_viewport;

    Ref<VertexShader> m_vertexShader;
    Ref<PixelShader>  m_pixelShader;

    std::function<void()> m_onBindFunc;

    Image2DList        m_inputImages;
    SamplerStateList   m_samplerStates;
    ConstantBufferList m_constantBuffers;
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