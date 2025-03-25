#pragma once

namespace crab
{

class DepthBuffer;
class RenderTarget;

class FrameBuffer
{
public:
    FrameBuffer();
    ~FrameBuffer();

    //===================================================
    // Init
    //===================================================

    void Init(const RenderTargetArray& in_renderTargets,
              const Ref<DepthBuffer>&  in_depthBuffer);

    void InitDepthOnly(const Ref<DepthBuffer>& in_depthBuffer);

    //===================================================
    // Function
    //===================================================

    void Bind() const;

    void ClearRenderTargets(const Color4& in_color) const;

    void ClearDepthBuffer(bool   in_clearDepth,
                          float  in_clearDepthFactor,
                          bool   in_clearStencil,
                          uint32 in_clearStencilFactor) const;

    Ref<DepthBuffer>  GetDepthBuffer() const { return m_depthBuffer; }
    Ref<RenderTarget> GetRenderTarget(uint32 in_index) const { return m_renderTargets[in_index]; }
    RenderTargetArray GetRenderTargets(uint32* out_arraySize) const;

    bool HasRenderTarget() const { return m_numRenderTargets > 0; }
    bool HasDepthBuffer() const { return m_depthBuffer != nullptr; }

private:
    void Init_Internal(const RenderTargetArray& in_renderTargets,
                       const Ref<DepthBuffer>&  in_depthBuffer);

    inline static RenderResourceIDGenerator s_idGenerator;
    FrameBufferID                           m_id = 0;

    RenderTargetArray      m_renderTargets;
    D3D11RenderTargetArray m_d3d11RenderTargets;
    uint32                 m_numRenderTargets = 0;

    Ref<DepthBuffer> m_depthBuffer;
};

}   // namespace crab