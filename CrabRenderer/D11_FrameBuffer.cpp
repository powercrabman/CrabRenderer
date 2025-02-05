#include "CrabPch.h"

#include "D11_FrameBuffer.h"

#include "D11_DepthStencil.h"
#include "D11_RenderTarget.h"
#include "D11_Renderer.h"

namespace crab
{

Ref<D11_FrameBuffer> crab::D11_FrameBuffer::Create(const Ref<D11_RenderTarget>& in_renderTarget,
                                                   const Viewport&              in_viewport,
                                                   const Ref<D11_DepthStencil>& in_depthStencil)
{
    Ref<D11_FrameBuffer> frameBuffer = CreateRef<D11_FrameBuffer>();

    frameBuffer->renderTarget = in_renderTarget;
    frameBuffer->viewport     = in_viewport;
    frameBuffer->depthStencil = in_depthStencil;
    return frameBuffer;
}

}   // namespace crab