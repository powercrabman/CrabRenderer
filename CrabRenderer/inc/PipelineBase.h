#pragma once

namespace crab
{
class FrameBuffer;
class Mesh;
class VertexShader;
class PixelShader;
class GeometryShader;
class HullShader;
class DomainShader;
class BlendState;
class RasterizerState;
class DepthStencilState;
class RenderTarget;
class DepthBuffer;

struct PipelineBindArgs
{
    static PipelineBindArgs EmptyArgs()
    {
        return PipelineBindArgs();
    }

    uint32       in_stencilRef  = 0;
    BlendFactors in_blendFactor = { 1.f, 1.f, 1.f, 1.f };
};

class PipelineBase
{
public:
    virtual ~PipelineBase() = default;

    virtual void BindPipeline(const PipelineBindArgs& in_args);

    void ClearRenderTargets(const Color4& in_color) const;

    void ClearDepthBuffer(bool   in_clearDepth,
                          float  in_clearDepthFactor,
                          bool   in_clearStencil,
                          uint32 in_clearStencilFactor) const;

    void Draw(const Ref<Mesh>& in_mesh) const;

protected:
    void InitPipelineBase_Internal(eTopology                     in_topology,
                                   const Ref<VertexShader>&      in_vs,
                                   const Ref<PixelShader>&       in_ps,
                                   const Ref<GeometryShader>&    in_gs,
                                   const Ref<HullShader>&        in_hs,
                                   const Ref<DomainShader>&      in_ds,
                                   const Ref<RasterizerState>&   in_rasterizerState,
                                   const Ref<DepthStencilState>& in_depthStencilState,
                                   const Ref<BlendState>&        in_blendState,
                                   const Ref<FrameBuffer>&       in_frameBuffer,
                                   const Viewport&               in_viewport);

    // IA
    eTopology m_topology = eTopology::TriangleList;

    // Shaders
    Ref<VertexShader>   m_vertexShader   = nullptr;
    Ref<PixelShader>    m_pixelShader    = nullptr;
    Ref<GeometryShader> m_geometryShader = nullptr;
    Ref<HullShader>     m_hullShader     = nullptr;
    Ref<DomainShader>   m_domainShader   = nullptr;

    // State
    Ref<DepthStencilState> m_depthStencilState = nullptr;
    Ref<RasterizerState>   m_rasterizerState   = nullptr;
    Ref<BlendState>        m_blendState        = nullptr;

    // FrameBuffer
    Ref<FrameBuffer> m_frameBuffer = nullptr;
    Viewport         m_viewport    = Viewport();
};

}   // namespace crab