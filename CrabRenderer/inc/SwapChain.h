#pragma once

namespace crab
{
class FrameBuffer;

class Texture2D;
class RenderTarget;
class DepthBuffer;

class SwapChain
{
public:
    void Init(const RendererInitInfo& in_initInfo);

    void Present(bool in_vsync) const;
    void OnResize(uint32 in_width, uint32 in_height);

    Ref<FrameBuffer> GetBackBufferFB() const;
    Ref<FrameBuffer> GetHDRBackBufferFB() const;
    Ref<DepthBuffer> GetDepthBuffer() const;

private:
    void CreateResources_Internal(uint32  in_width,
                                  uint32  in_height,
                                  bool    in_createDepthBuffer,
                                  eFormat in_depthBufferFormat,
                                  bool    in_createHDR,
                                  eFormat in_HDRFormat);

    void CreateHDRRenderTarget_Internal(uint32  in_width,
                                        uint32  in_height,
                                        eFormat in_HDRFormat);

    ComPtr<IDXGISwapChain1> m_swapChain;

    // Main Back Buffer
    Ref<FrameBuffer> m_backBufferFO;
    Ref<FrameBuffer> m_mainFrameBuffer;
    Ref<DepthBuffer> m_depthBuffer;
};

}   // namespace crab