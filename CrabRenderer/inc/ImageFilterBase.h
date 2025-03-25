#pragma once

namespace crab
{
class SamplerState;
class PixelShader;
class VertexShader;
class Texture2D;
class RenderTarget;

class ImageFilterBase
{
public:
    virtual ~ImageFilterBase();

    void           SetRenderTarget(const Ref<RenderTarget>& in_rt) { m_renderTarget = in_rt; }
    Ref<Texture2D> GetOutputTexture() const;

    virtual void Bind();

protected:
    ImageFilterBase();

    void Init_Internal(uint32                   in_width,
                       uint32                   in_height,
                       const Ref<VertexShader>& in_vs,
                       const Ref<PixelShader>&  in_ps,
                       const Ref<Texture2D>&    in_inputTexture);

    Ref<RenderTarget> m_renderTarget;
    Viewport          m_viewport;

    Ref<VertexShader> m_vertexShader;
    Ref<PixelShader>  m_pixelShader;

    Ref<Texture2D>    m_inputTexture;
    Ref<SamplerState> m_linearClampSampler;
};

}   // namespace crab