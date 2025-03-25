#include "InternalPCH.h"

#include "PipelineBase.h"

#include "DepthBuffer.h"
#include "FrameBuffer.h"
#include "Mesh.h"
#include "RenderStates.h"
#include "RenderTarget.h"
#include "Renderer.h"
#include "Shaders.h"

namespace crab
{

void PipelineBase::BindPipeline(const PipelineBindArgs& in_args)
{
    auto& r = GetRenderer();

    // IA
    r.SetTopology(static_cast<D3D11_PRIMITIVE_TOPOLOGY>(m_topology));

    // Shaders
    m_vertexShader->Bind();
    m_pixelShader->Bind();

    if (m_geometryShader)
        m_geometryShader->Bind();
    else
        r.ResetGeometryShader();

    if (m_hullShader)
        m_hullShader->Bind();
    else
        r.ResetHullShader();

    if (m_domainShader)
        m_domainShader->Bind();
    else
        r.ResetDomainShader();

    // RS
    if (m_depthStencilState)
        m_depthStencilState->Bind(in_args.in_stencilRef);

    if (m_blendState)
        m_blendState->Bind(in_args.in_blendFactor);

    if (m_rasterizerState)
        m_rasterizerState->Bind();

    // Frame
    m_frameBuffer->Bind();
    m_viewport.Bind();
}

void PipelineBase::InitPipelineBase_Internal(
    eTopology                     in_topology,
    const Ref<VertexShader>&      in_vs,
    const Ref<PixelShader>&       in_ps,
    const Ref<GeometryShader>&    in_gs,
    const Ref<HullShader>&        in_hs,
    const Ref<DomainShader>&      in_ds,
    const Ref<RasterizerState>&   in_rasterizerState,
    const Ref<DepthStencilState>& in_depthStencilState,
    const Ref<BlendState>&        in_blendState,
    const Ref<FrameBuffer>&       in_frameBuffer,
    const Viewport&               in_viewport)
{
    ASSERT(in_vs, "PipelineBase::InitPipelineBase_Internal: VertexShader is nullptr.");
    ASSERT(in_ps, "PipelineBase::InitPipelineBase_Internal: PixelShader is nullptr.");
    ASSERT(in_frameBuffer, "PipelineBase::InitPipelineBase_Internal: FrameBuffer is nullptr.");

    m_topology          = in_topology;
    m_vertexShader      = in_vs;
    m_pixelShader       = in_ps;
    m_geometryShader    = in_gs;
    m_hullShader        = in_hs;
    m_domainShader      = in_ds;
    m_rasterizerState   = in_rasterizerState;
    m_depthStencilState = in_depthStencilState;
    m_blendState        = in_blendState;
    m_frameBuffer       = in_frameBuffer;
    m_viewport          = in_viewport;
}

void PipelineBase::ClearRenderTargets(const Color4& in_color) const
{
    m_frameBuffer->ClearRenderTargets(in_color);
}

void PipelineBase::ClearDepthBuffer(bool   in_clearDepth,
                                    float  in_clearDepthFactor,
                                    bool   in_clearStencil,
                                    uint32 in_clearStencilFactor) const
{
    m_frameBuffer->ClearDepthBuffer(in_clearDepth,
                                    in_clearDepthFactor,
                                    in_clearStencil,
                                    in_clearStencilFactor);
}

void PipelineBase::Draw(const Ref<Mesh>& in_mesh) const
{
    in_mesh->Draw();
}

}   // namespace crab