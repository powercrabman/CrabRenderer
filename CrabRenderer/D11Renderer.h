#pragma once
#include "D11Utils.h"
#include "Singleton.h"

namespace crab
{

class VertexShader;
class PixelShader;
class VertexBuffer;
class IndexBuffer;
class Image2D;
class RasterizerState;
class DepthStencilState;
class BlendState;
class ConstantBufferBase;
class RenderStates;
class Swapchain;
class RenderTarget;
class DepthBuffer;
class CommonState;

class D11Renderer : public Singleton<D11Renderer>
{
    friend class Singleton<D11Renderer>;

public:
    ~D11Renderer();

    // - Core
    void Init(const RendererSetting& in_setting, const Int2& in_screenSize, HWND in_hWnd);
    void Shutdown();

    void OnEvent(CrabEvent& in_event) const;

    // - Draw
    void Draw(uint32 in_vertexCount, uint32 in_startVertexLocation = 0) const;
    void DrawIndexed(uint32 in_indexCount, uint32 in_startIndexLocation = 0, int32 in_baseVertexLocation = 0) const;

    // - Render State
    void SetTopology(eTopology in_topology);

    void SetVertexBuffer(ID3D11Buffer* in_vertexBuffer, uint32 in_stride, uint32 in_offset);
    void SetIndexBuffer(ID3D11Buffer* in_indexBuffer);

    void SetVertexShader(ID3D11VertexShader* in_shader, ID3D11InputLayout* in_layout);
    void SetPixelShader(ID3D11PixelShader* in_shader);
    void SetGeometryShader(ID3D11GeometryShader* in_shader);
    void SetHullShader(ID3D11HullShader* in_shader);
    void SetDomainShader(ID3D11DomainShader* in_shader);

    void SetRasterizerState(ID3D11RasterizerState* in_rasterizerState);
    void SetDepthStencilState(ID3D11DepthStencilState* in_depthStencilState, uint32 in_stencilRef);
    void SetBlendState(ID3D11BlendState* in_blendState, const std::array<float, 4>& in_blendFactor);

    void SetSamplerState(ID3D11SamplerState* in_srv, uint32 in_slot, eShaderFlags in_bindFlags);
    void SetShaderResourceView(ID3D11ShaderResourceView* in_srv, uint32 in_slot, eShaderFlags in_bindFlags);
    void SetConstantBuffer(ID3D11Buffer* in_constantBuffer, uint32 in_slot, eShaderFlags in_bindFlags);
    void SetUnorderedAccessView(ID3D11UnorderedAccessView* in_uav, uint32 in_slot) const;
    void DispatchComputeShader(ID3D11ComputeShader* in_cs, uint32 in_threadGroupCountX, uint32 in_threadGroupCountY, uint32 in_threadGroupCountZ) const;

    void SetRenderTarget(ID3D11RenderTargetView* in_renderTargetView, ID3D11DepthStencilView* in_depthStencilView) const;
    void SetRenderTargets(const std::vector<ID3D11RenderTargetView*>& in_renderTargetViews, ID3D11DepthStencilView* in_depthStencilView) const;
    void ReleaseRenderTargets() const;

    void SetViewport(const Viewport& in_viewport);

    // back texture helper
    void              BindBackBuffer(const Ref<DepthBuffer>& in_depthBuffer_or_null);
    void              ClearBackBuffer(const Color& in_color) const;
    Ref<RenderTarget> GetBackBuffer();

    void              BindBackBufferMS(const Ref<DepthBuffer>& in_depthBuffer_or_null);
    void              ClearBackBufferMS(const Color& in_color) const;
    Ref<RenderTarget> GetBackBufferMS() const;

    void ClearDepthBuffer(bool  in_clearDepth,
                          float in_clearDepthFactor,
                          bool  in_clearStencil,
                          int32 in_clearStencilFactor) const;

    void ClearDepthOnlyBuffer(bool  in_clearDepth,
                              float in_clearDepthFactor,
                              bool  in_clearStencil,
                              int32 in_clearStencilFactor) const;

    Ref<DepthBuffer> GetDepthBuffer() const;
    Ref<DepthBuffer> GetDepthOnlyBuffer() const;

    // - Swap Chain
    void              Present() const;
    Ref<Swapchain>    GetSwapChain() const;
    Ref<RenderTarget> GetBackBuffer() const;

    // - Getter (Native)
    ID3D11Device*        GetDevice() const;
    ID3D11DeviceContext* GetContext() const;

    CommonState* GetRenderStateStorage() const;

private:
    D11Renderer();

    // Device
    ComPtr<ID3D11Device>        m_device;
    ComPtr<ID3D11DeviceContext> m_deviceContext;
    ComPtr<ID3D11Debug>         m_debug;       // only debug
    ComPtr<ID3D11InfoQueue>     m_infoQueue;   // only debug

    // Swap Chain
    Ref<Swapchain> m_swapChain;

    // Feature Level
    D3D_FEATURE_LEVEL m_featureLevel;

    // Storage
    Scope<CommonState> m_renderStateStorage;

    using ConstantBufferArray = std::array<ID3D11Buffer*, CONSTANT_BUFFER_SLOT_MAX>;
    using TextureArray        = std::array<ID3D11ShaderResourceView*, TEXTURE_SLOT_MAX>;
    using SamplerStateArray   = std::array<ID3D11SamplerState*, SAMPLER_SLOT_MAX>;

    // Input Assembler
    D3D11_PRIMITIVE_TOPOLOGY m_topology     = D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED;
    ID3D11Buffer*            m_vertexBuffer = nullptr;
    ID3D11Buffer*            m_indexBuffer  = nullptr;

    // Shaders
    ID3D11VertexShader* m_vertexShader = nullptr;
    ConstantBufferArray m_vsBuffer     = {};
    TextureArray        m_vsTexture    = {};
    SamplerStateArray   m_vsSampler    = {};

    ID3D11HullShader*   m_hullShader = nullptr;
    ConstantBufferArray m_hsBuffer   = {};
    TextureArray        m_hsTexture  = {};
    SamplerStateArray   m_hsSampler  = {};

    ID3D11DomainShader* m_domainShader = nullptr;
    ConstantBufferArray m_dsBuffer     = {};
    TextureArray        m_dsTexture    = {};
    SamplerStateArray   m_dsSampler    = {};

    ID3D11GeometryShader* m_geometryShader = nullptr;
    ConstantBufferArray   m_gsBuffer       = {};
    TextureArray          m_gsTexture      = {};
    SamplerStateArray     m_gsSampler      = {};

    ID3D11PixelShader*  m_pixelShader = nullptr;
    ConstantBufferArray m_psBuffer    = {};
    TextureArray        m_psTexture   = {};
    SamplerStateArray   m_psSampler   = {};

    ConstantBufferArray m_csBuffer                  = {};
    uint32              m_csBigestTextureBindedSlot = 0;
    SamplerStateArray   m_csSampler                 = {};

    // RenderState
    ID3D11RasterizerState*   m_rsState     = nullptr;
    ID3D11DepthStencilState* m_dsState     = nullptr;
    ID3D11BlendState*        m_bState      = nullptr;
    std::array<float, 4>     m_blendFactor = { 1.0f, 1.0f, 1.0f, 1.0f };
    Viewport                 m_viewport;
    uint32                   m_stencilRef = 0;
};

//===================================================
// Inline
//===================================================

inline D11Renderer& GetRenderer()
{
    return D11Renderer::GetInstance();
}

inline CommonState* GetCommonState()
{
    return GetRenderer().GetRenderStateStorage();
}

}   // namespace crab