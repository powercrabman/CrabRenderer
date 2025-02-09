#pragma once
#include "D11_Utiles.h"

namespace crab
{

class D11_RenderTarget;
class D11_DepthStencil;

class D11_FrameBuffer
{
public:
    // - Factory
    static Ref<D11_FrameBuffer> Create(const Ref<D11_RenderTarget>& in_renderTarget,
                                       const Viewport&              in_viewport,
                                       const Ref<D11_DepthStencil>& in_depthStencil);

    static Ref<D11_FrameBuffer> Create(const Ref<D11_RenderTarget>& in_renderTarget,
                                       const Viewport&              in_viewport);

    Ref<D11_DepthStencil> GetDepthStencil() const { return m_depthStencil; }
    Ref<D11_RenderTarget> GetRenderTarget() const { return m_renderTarget; }
    const Viewport&       GetViewport() const { return m_viewport; }

    void Bind();
    void Clear(const Color& in_color, bool in_clearDepth, bool in_clearStencil);

private:
    Ref<D11_RenderTarget> m_renderTarget;
    Ref<D11_DepthStencil> m_depthStencil;
    Viewport              m_viewport;
};

class D11_FrameBufferList
{
public:
    D11_FrameBufferList()  = default;
    ~D11_FrameBufferList() = default;

    void Bind();
    void Clear(const std::vector<Color>& in_colors, bool in_clearDepth, bool in_clearStencil);

    void AddRenderTarget(const Ref<D11_RenderTarget>& in_rt);
    void SetDepthStencil(const Ref<D11_DepthStencil>& in_ds);
    void SetViewport(const Viewport& in_vp);

    Ref<D11_RenderTarget> GetRenderTarget(uint32 in_index) const;

private:
    std::vector<Ref<D11_RenderTarget>> m_renderTargets;
    Ref<D11_DepthStencil>              m_depthStencil;
    Viewport                           m_viewport;

    std::vector<ID3D11RenderTargetView*> m_rtvs;
};

}   // namespace crab
