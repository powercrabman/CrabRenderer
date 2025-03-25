#include "InternalPCH.h"

#include "FrameBuffer.h"

#include "DepthBuffer.h"
#include "RenderTarget.h"
#include "Renderer.h"

namespace crab
{

FrameBuffer::FrameBuffer()
{
}

FrameBuffer::~FrameBuffer()
{
}

void FrameBuffer::Init(const RenderTargetArray& in_renderTargets,
                       const Ref<DepthBuffer>&  in_depthBuffer)
{
    Init_Internal(in_renderTargets, in_depthBuffer);
}

void FrameBuffer::InitDepthOnly(const Ref<DepthBuffer>& in_depthBuffer)
{
    RenderTargetArray renderTargets;
    renderTargets.fill(nullptr);

    Init_Internal(renderTargets, in_depthBuffer);
}

void FrameBuffer::Bind() const
{
    GetRenderer().SetRenderTargets(
        m_d3d11RenderTargets.data(),
        m_numRenderTargets,
        m_depthBuffer ? m_depthBuffer->GetDSV() : nullptr,
        m_id);
}

void FrameBuffer::ClearRenderTargets(const Color4& in_color) const
{
    if (HasRenderTarget())
    {
        for (uint32 i = 0; i < m_numRenderTargets; ++i)
            m_renderTargets[i]->Clear(in_color);
    }
}

void FrameBuffer::ClearDepthBuffer(bool   in_clearDepth,
                                   float  in_clearDepthFactor,
                                   bool   in_clearStencil,
                                   uint32 in_clearStencilFactor) const
{
    if (HasDepthBuffer())
    {
        m_depthBuffer->Clear(in_clearDepth,
                             in_clearDepthFactor,
                             in_clearStencil,
                             in_clearStencilFactor);
    }
}

RenderTargetArray FrameBuffer::GetRenderTargets(uint32* out_arraySize) const
{
    if (out_arraySize)
        *out_arraySize = m_numRenderTargets;

    return m_renderTargets;
}

void FrameBuffer::Init_Internal(const RenderTargetArray& in_renderTargets,
                                const Ref<DepthBuffer>&  in_depthBuffer)
{
    ASSERT(in_renderTargets.size() <= SHADER_RENDER_TARGET_SLOT_COUNT,
           "FrameBuffer::Init: RenderTargetArray size is too big.");

    m_renderTargets = in_renderTargets;
    m_depthBuffer   = in_depthBuffer;

    m_d3d11RenderTargets.fill(nullptr);
    for (uint32 i = 0; i < m_renderTargets.size(); ++i)
    {
        if (m_renderTargets[i])
        {
            m_d3d11RenderTargets[i] = m_renderTargets[i]->GetRTV();
        }
        else
        {
            m_numRenderTargets = i;
            break;   
        }
    }

    m_id = s_idGenerator.GenerateID();
}

}   // namespace crab