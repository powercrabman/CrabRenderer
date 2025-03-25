#pragma once
#include "RenderDefinition.h"
#include "SwapChain.h"

namespace crab
{

class VertexShader;
class PixelShader;
class VertexBuffer;
class IndexBuffer;
class Texture2D;
class RasterizerState;
class DepthStencilState;
class BlendState;
class ConstantBufferBase;
class RenderStates;
class SwapChain;
class RenderTarget;
class DepthBuffer;
class GlobalState;

//===================================================
// Renderer
//===================================================

class Renderer : public Singleton<Renderer>
{
    friend class Singleton<Renderer>;

public:
    ~Renderer();

    // - Core
    void Init(const RendererInitInfo& in_initInfo);

    void InitGlobalResources(const std::filesystem::path& in_rendererDefaultShaderDirectory);
    void Shutdown();

    void OnResize(uint32 in_width, uint32 in_height) const;

    // - Draw
    void Draw(uint32 in_vertexCount, uint32 in_startVertexLocation = 0) const;
    void DrawIndexed(uint32 in_indexCount, uint32 in_startIndexLocation = 0, int32 in_baseVertexLocation = 0) const;

    // - Render State
    void SetTopology(D3D11_PRIMITIVE_TOPOLOGY in_topology);
    void SetTopology(eTopology in_topology) { SetTopology(static_cast<D3D11_PRIMITIVE_TOPOLOGY>(in_topology)); }

    void SetVertexBuffer(ID3D11Buffer*  in_vertexBuffer,
                         uint32         in_stride,
                         uint32         in_offset,
                         VertexBufferID in_vertexBufferID);

    void SetIndexBuffer(ID3D11Buffer* in_indexBuffer,
                        IndexBufferID in_indexBufferID);

    void SetVertexShader(ID3D11VertexShader* in_shader,
                         VertexShaderID      in_vertexShaderID);

    void SetInputLayout(ID3D11InputLayout* in_inputLayout,
                        InputLayoutID      in_inputLayoutID);

    void SetPixelShader(ID3D11PixelShader* in_shader,
                        PixelShaderID      in_pixelShaderID);

    void SetGeometryShader(ID3D11GeometryShader* in_shader,
                           GeometryShaderID      in_geometryShaderID);

    void SetHullShader(ID3D11HullShader* in_shader,
                       HullShaderID      in_hullShaderID);

    void SetDomainShader(ID3D11DomainShader* in_shader,
                         DomainShaderID      in_domainShaderID);

    void SetRasterizerState(ID3D11RasterizerState* in_rasterizerState,
                            RasterizerStateID      in_rasterizerStateID);

    void SetDepthStencilState(ID3D11DepthStencilState* in_depthStencilState,
                              uint32                   in_stencilRef,
                              DepthStencilStateID      in_depthStencilStateID);

    void SetBlendState(ID3D11BlendState*           in_blendState,
                       const std::array<float, 4>& in_blendFactor,
                       BlendStateID                in_blendStateID);

    void SetSamplerState(ID3D11SamplerState* in_srv,
                         uint32              in_slot,
                         eShaderFlags        in_bindFlags);

    void SetShaderResourceView(ID3D11ShaderResourceView* in_srv,
                               uint32                    in_slot,
                               eShaderFlags              in_bindFlags);

    void SetConstantBuffer(ID3D11Buffer* in_constantBuffer,
                           uint32        in_slot,
                           eShaderFlags  in_bindFlags);

    void SetUnorderedAccessView(ID3D11UnorderedAccessView* in_uav,
                                uint32                     in_slot) const;

    void DispatchComputeShader(ID3D11ComputeShader* in_cs,
                               uint32               in_threadGroupCountX,
                               uint32               in_threadGroupCountY,
                               uint32               in_threadGroupCountZ) const;

    void SetRenderTargets(ID3D11RenderTargetView* const* in_renderTargetViews,
                          uint32                         in_renderTargetCount,
                          ID3D11DepthStencilView*        in_depthStencilView,
                          FrameBufferID                  in_frameBufferID = RENDERER_RESOURCE_NULL_ID);

    void SetViewport(const Viewport& in_viewport);

    void ResetVertexBuffer();
    void ResetIndexBuffer();
    void ResetFrameBuffer();
    void ResetVertexShader();
    void ResetPixelShader();
    void ResetGeometryShader();
    void ResetHullShader();
    void ResetDomainShader();

    // - Swap Chain
    void            Present();
    NODISCARD const Ref<SwapChain>& GetSwapChain() const;

    // - Getter
    NODISCARD ID3D11Device*        GetDevice() const;
    NODISCARD ID3D11DeviceContext* GetDeviceContext() const;

    NODISCARD D3D11_PRIMITIVE_TOPOLOGY GetTopology() const { return m_topology; }

    NODISCARD MSAA GetMSAA() const { return m_config.msaa; }
    NODISCARD HDR  GetHDR() const { return m_config.hdr; }
    NODISCARD bool IsEnableMSAA() const { return m_config.msaa.enableMSAA; }
    NODISCARD bool IsEnableHDR() const { return m_config.hdr.enableHDR; }

    NODISCARD D3D_FEATURE_LEVEL GetFeatureLevel() const { return m_device->GetFeatureLevel(); }

    NODISCARD Ref<FrameBuffer> GetBackBufferFB() const;
    NODISCARD Ref<FrameBuffer> GetHDRBackBufferFB() const;
    NODISCARD Ref<DepthBuffer> GetDepthBuffer() const;

private:
    Renderer();

    // Device
    ComPtr<ID3D11Device>        m_device;
    ComPtr<ID3D11DeviceContext> m_deviceContext;
    ComPtr<ID3D11Debug>         m_debug;       // only debug
    ComPtr<ID3D11InfoQueue>     m_infoQueue;   // only debug

    // Swap Chain
    Ref<SwapChain> m_swapChain;

    // Config
    RendererConfig m_config;

    using ConstantBufferArray = std::array<ID3D11Buffer*, SHADER_CONSTANTS_SLOT_COUNT>;
    using TextureArray        = std::array<ID3D11ShaderResourceView*, SHADER_SRV_SLOT_COUNT>;
    using SamplerStateArray   = std::array<ID3D11SamplerState*, SHADER_SAMPLER_SLOT_COUNT>;
    using RenderTargetArray   = std::array<ID3D11RenderTargetView*, SHADER_RENDER_TARGET_SLOT_COUNT>;

    // Input Assembler
    D3D11_PRIMITIVE_TOPOLOGY m_topology       = D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED;
    VertexBufferID           m_vertexBufferID = RENDERER_RESOURCE_NULL_ID;
    IndexBufferID            m_indexBufferID  = RENDERER_RESOURCE_NULL_ID;

    // Shaders
    InputLayoutID       m_inputLayoutID  = RENDERER_RESOURCE_NULL_ID;
    VertexShaderID      m_vertexShaderID = RENDERER_RESOURCE_NULL_ID;
    ConstantBufferArray m_vsBuffer       = {};
    TextureArray        m_vsTexture      = {};
    SamplerStateArray   m_vsSampler      = {};

    HullShaderID        m_hullShaderID = RENDERER_RESOURCE_NULL_ID;
    ConstantBufferArray m_hsBuffer     = {};
    TextureArray        m_hsTexture    = {};
    SamplerStateArray   m_hsSampler    = {};

    DomainShaderID      m_domainShaderID = RENDERER_RESOURCE_NULL_ID;
    ConstantBufferArray m_dsBuffer       = {};
    TextureArray        m_dsTexture      = {};
    SamplerStateArray   m_dsSampler      = {};

    GeometryShaderID    m_geometryShaderID = RENDERER_RESOURCE_NULL_ID;
    ConstantBufferArray m_gsBuffer         = {};
    TextureArray        m_gsTexture        = {};
    SamplerStateArray   m_gsSampler        = {};

    PixelShaderID       m_pixelShaderID = RENDERER_RESOURCE_NULL_ID;
    ConstantBufferArray m_psBuffer      = {};
    TextureArray        m_psTexture     = {};
    SamplerStateArray   m_psSampler     = {};

    ConstantBufferArray m_csBuffer                  = {};
    uint32              m_csBigestTextureBindedSlot = 0;
    SamplerStateArray   m_csSampler                 = {};

    // RenderState
    RasterizerStateID    m_rasterizerStateID   = RENDERER_RESOURCE_NULL_ID;
    DepthStencilStateID  m_depthStencilStateID = RENDERER_RESOURCE_NULL_ID;
    BlendStateID         m_blendStateID        = RENDERER_RESOURCE_NULL_ID;
    std::array<float, 4> m_blendFactor         = { 1.0f, 1.0f, 1.0f, 1.0f };
    uint32               m_stencilRef          = 0;

    // Framebuffer
    Viewport      m_viewport;
    FrameBufferID m_frameBufferID = RENDERER_RESOURCE_NULL_ID;
};

//===================================================
// Inline
//===================================================

inline Renderer& GetRenderer()
{
    return Renderer::GetInstance();
}

}   // namespace crab