#include "CrabPch.h"

#include "D11_FrameBuffer.h"

#include "D11_DepthStencil.h"
#include "D11_RenderTarget.h"
#include "D11_Renderer.h"

namespace crab
{

Ref<D11_FrameBuffer> D11_FrameBuffer::Create(const Ref<D11_RenderTarget>& in_renderTarget,
                                             const Viewport&              in_viewport,
                                             const Ref<D11_DepthStencil>& in_depthStencil)
{
    Ref<D11_FrameBuffer> frameBuffer = CreateRef<D11_FrameBuffer>();
    frameBuffer->m_renderTarget      = in_renderTarget;
    frameBuffer->m_viewport          = in_viewport;
    frameBuffer->m_depthStencil      = in_depthStencil;

    return frameBuffer;
}

Ref<D11_FrameBuffer> D11_FrameBuffer::Create(const Ref<D11_RenderTarget>& in_renderTarget, const Viewport& in_viewport)
{
    return Create(in_renderTarget, in_viewport, nullptr);
}

void D11_FrameBuffer::Bind()
{
    auto* dx = D11_API;
    dx->SetRenderTarget(m_renderTarget->Get(), m_depthStencil ? m_depthStencil->Get() : nullptr);
    dx->SetViewport(m_viewport);
}

void D11_FrameBuffer::Clear(const Color& in_color, bool in_clearDepth, bool in_clearStencil)
{
    m_renderTarget->Clear(in_color);

    if (m_depthStencil)
        m_depthStencil->Clear(in_clearDepth, in_clearStencil);
}

void D11_FrameBufferList::Clear(const std::vector<Color>& in_colors, bool in_clearDepth, bool in_clearStencil)
{
    auto* dx = D11_API;

    for (uint32 i = 0; i < m_renderTargets.size(); ++i)
        m_renderTargets[i]->Clear(in_colors[i]);

    if (m_depthStencil)
        m_depthStencil->Clear(in_clearDepth, in_clearStencil);
}

void D11_FrameBufferList::Bind()
{
    auto* dx = D11_API;

    dx->SetRenderTargets(m_rtvs, m_depthStencil ? m_depthStencil->Get() : nullptr);
    dx->SetViewport(m_viewport);
}

void D11_FrameBufferList::AddRenderTarget(const Ref<D11_RenderTarget>& in_rt)
{
    m_renderTargets.push_back(in_rt);
    m_rtvs.push_back(in_rt->Get());
}

void D11_FrameBufferList::SetDepthStencil(const Ref<D11_DepthStencil>& in_ds)
{
    m_depthStencil = in_ds;
}

void D11_FrameBufferList::SetViewport(const Viewport& in_vp)
{
    m_viewport = in_vp;
}

Ref<D11_RenderTarget> D11_FrameBufferList::GetRenderTarget(uint32 in_index) const
{
    CRAB_ASSERT(in_index < m_renderTargets.size(), "Index out of range.");
    return m_renderTargets[in_index];
}

}   // namespace crab