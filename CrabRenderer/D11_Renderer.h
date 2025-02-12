#pragma once
#include "D11_Utils.h"

namespace crab
{

class D11_Imgui;
class D11_VertexShader;
class D11_PixelShader;
class D11_VertexBuffer;
class D11_IndexBuffer;
class D11_Texture2D;
class D11_RasterizerState;
class D11_DepthStencilState;
class D11_BlendState;
class D11_ConstantBufferBase;
class D11_SamplerState;
class D11_Swapchain;
class D11_RenderTarget;
class D11_DepthBuffer;

class D11_Renderer
{
public:
    D11_Renderer();
    ~D11_Renderer();

    // - Core
    void Init(const RendererSetting& in_setting);
    void Shutdown();

    void BeginGUI();
    void EndGUI();

    void OnEvent(CrabEvent& in_event);

    // - Draw
    void Draw(uint32 in_vertexCount, uint32 in_startVertexLocation = 0);
    void DrawIndexed(uint32 in_indexCount, uint32 in_startIndexLocation = 0, int32 in_baseVertexLocation = 0);

    // - Render State
    void SetTopology(eTopology in_topology);

    void SetVertexBuffer(ID3D11Buffer* in_vertexBuffer, uint32 in_stride, uint32 in_offset);
    void SetIndexBuffer(ID3D11Buffer* in_indexBuffer);

    void SetVertexShader(ID3D11VertexShader* in_shader, ID3D11InputLayout* in_layout);
    void SetPixelShader(ID3D11PixelShader* in_shader);
    void SetGeometryShader(ID3D11GeometryShader* in_shader);
    void SetComputeShader(ID3D11ComputeShader* in_shader);
    void SetHullShader(ID3D11HullShader* in_shader);
    void SetDomainShader(ID3D11DomainShader* in_shader);

    void SetSamplerState(ID3D11SamplerState* in_samplerState, uint32 in_slot, eShaderFlags in_flag);
    void SetRasterizerState(ID3D11RasterizerState* in_rasterizerState);
    void SetDepthStencilState(ID3D11DepthStencilState* in_depthStencilState);
    void SetBlendState(ID3D11BlendState* in_blendState);

    void SetShaderResourceView(ID3D11ShaderResourceView* in_srv, uint32 in_slot, eShaderFlags in_flag);
    void ReleaseShaderResourceViews(eShaderFlags in_flag);
    void SetConstantBuffer(ID3D11Buffer* in_constantBuffer, uint32 in_slot, eShaderFlags in_flag);

    void SetRenderTarget(ID3D11RenderTargetView* in_renderTargetView, ID3D11DepthStencilView* in_depthStencilView);
    void SetRenderTargets(const std::vector<ID3D11RenderTargetView*>& in_renderTargetViews, ID3D11DepthStencilView* in_depthStencilView);
    void ReleaseRenderTargets();

    void SetViewport(const Viewport& in_viewport);

    // back buffer helper
    void                  BindBackBuffer();
    void                  ClearBackBuffer(const Color& in_color);
    Ref<D11_RenderTarget> GetBackBuffer();

    void                  BindFloatBackBuffer();
    void                  ClearFloatBackBuffer(const Color& in_color);
    Ref<D11_RenderTarget> GetFloatBackBuffer();

    void                 ClearDepthBuffer(float in_depth = 1.f, uint8 in_stencil = 0) const;
    Ref<D11_DepthBuffer> GetDepthBuffer() const;

    // - Swap Chain
    void                  Present();
    Ref<D11_Swapchain>    GetSwapChain() const;
    Ref<D11_RenderTarget> GetBackBuffer() const;
    Ref<D11_RenderTarget> GetFloatBackBuffer() const;

    // - Getter (Native)
    ComPtr<ID3D11Device>        GetDevice() const;
    ComPtr<ID3D11DeviceContext> GetContext() const;

private:
    // Device
    ComPtr<ID3D11Device>        m_device;
    ComPtr<ID3D11DeviceContext> m_deviceContext;
    ComPtr<ID3D11Debug>         m_debug;       // only debug
    ComPtr<ID3D11InfoQueue>     m_infoQueue;   // only debug

    // Swap Chain
    Ref<D11_Swapchain> m_swapChain;

    // Feature Level
    D3D_FEATURE_LEVEL m_featureLevel;

    // imgui
    Scope<D11_Imgui> m_imgui;

    //===================================================
    // Pipeline State
    //===================================================

    using PipelineConstantBufferArray = std::array<ID3D11Buffer*, CONSTANT_BUFFER_SLOT_MAX>;
    using PipelineTextureArray        = std::array<ID3D11ShaderResourceView*, TEXTURE_SLOT_MAX>;
    using PipelineSamplerStateArray   = std::array<ID3D11SamplerState*, SAMPLER_SLOT_MAX>;

    // Input Assembler
    D3D11_PRIMITIVE_TOPOLOGY m_topology     = D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED;
    ID3D11Buffer*            m_vertexBuffer = nullptr;
    ID3D11Buffer*            m_indexBuffer  = nullptr;

    // Vertex Shader
    ID3D11VertexShader*         m_vertexShader = nullptr;
    PipelineConstantBufferArray m_vsConstantBuffers;
    PipelineTextureArray        m_vsTextures;
    uint32                      m_vsLargestBindedTextureSlot = 0;
    PipelineSamplerStateArray   m_vsSamplerStates;

    // Hull Shader
    ID3D11HullShader*           m_hullShader = nullptr;
    PipelineConstantBufferArray m_hsConstantBuffers;
    PipelineTextureArray        m_hsTextures;
    uint32                      m_hsLargestBindedTextureSlot = 0;
    PipelineSamplerStateArray   m_hsSamplerStates;

    // Domain Shader
    ID3D11DomainShader*         m_domainShader = nullptr;
    PipelineConstantBufferArray m_dsConstantBuffers;
    PipelineTextureArray        m_dsTextures;
    uint32                      m_dsLargestBindedTextureSlot = 0;
    PipelineSamplerStateArray   m_dsSamplerStates;

    // Geometry Shader
    ID3D11GeometryShader*       m_geometryShader = nullptr;
    PipelineConstantBufferArray m_gsConstantBuffers;
    PipelineTextureArray        m_gsTextures;
    uint32                      m_gsLargestBindedTextureSlot = 0;
    PipelineSamplerStateArray   m_gsSamplerStates;

    // Pixel Shader
    ID3D11PixelShader*          m_pixelShader = nullptr;
    PipelineConstantBufferArray m_psConstantBuffers;
    PipelineTextureArray        m_psTextures;
    uint32                      m_psLargestBindedTextureSlot = 0;
    PipelineSamplerStateArray   m_psSamplerStates;

    // Compute Shader
    ID3D11ComputeShader*        m_computeShader = nullptr;
    PipelineConstantBufferArray m_csConstantBuffers;
    PipelineTextureArray        m_csTextures;
    uint32                      m_csLargestBindedTextureSlot = 0;
    PipelineSamplerStateArray   m_csSamplerStates;

    // RenderState
    ID3D11RasterizerState*   m_rsState = nullptr;
    ID3D11DepthStencilState* m_dsState = nullptr;
    ID3D11BlendState*        m_bState  = nullptr;
    Viewport                 m_viewport;
};

}   // namespace crab