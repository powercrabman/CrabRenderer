#pragma once
#include "D11_Utiles.h"

namespace crab
{

struct D11_RenderTarget;
struct D11_DepthStencil;

struct D11_FrameBuffer
{
    // - Factory
    static Ref<D11_FrameBuffer> Create(const Ref<D11_RenderTarget>& in_renderTarget,
                                       const Viewport&              in_viewport,
                                       const Ref<D11_DepthStencil>& in_depthStencil /* opt */);

    Ref<D11_RenderTarget> renderTarget;
    Viewport              viewport;
    Ref<D11_DepthStencil> depthStencil;
};

}   // namespace crab