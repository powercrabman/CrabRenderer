#pragma once

namespace crab
{

class D11_FrameBuffer;
class D11_Texture;

class D11_Swapchain
{
public:
    static Ref<D11_Swapchain> Create(const SwapChainSetting& in_setting);

    void BindBackBuffer() const;
    void Present();
    void OnResize(uint32 in_width, uint32 in_height);

    void SetVSync(bool in_vsync) { m_vsync = in_vsync; }
    void Clear(const Color& in_color, bool in_clearDepth, bool in_clearStencil);

    Ref<D11_FrameBuffer> GetBackBuffer() const { return m_backbuffer; }
    Ref<D11_Texture>     GetBackBufferTexture() const;

private:
    void _CreateResources(uint32 in_width, uint32 in_height);

    ComPtr<IDXGISwapChain1> m_swapChain;
    Ref<D11_FrameBuffer>    m_backbuffer;

    DXGI_FORMAT m_backbufferFormat;
    DXGI_FORMAT m_depthStencilFormat;
    bool        m_useDepthStencil;
    bool        m_vsync;
};

}   // namespace crab