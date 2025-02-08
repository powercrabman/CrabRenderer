#pragma once
#include "D11_Utiles.h"

namespace crab
{

class D11_Imgui;
class D11_VertexShader;
class D11_PixelShader;
class D11_VertexBuffer;
class D11_IndexBuffer;
class D11_Texture;
class D11_FrameBuffer;
class D11_RasterizerState;
class D11_DepthStencilState;
class D11_BlendState;
class D11_ConstantBufferBase;
class D11_SamplerState;
class D11_Swapchain;

class D11_Renderer
{
public:
    D11_Renderer();
    ~D11_Renderer();

    // - Core
    void Init(const RendererSetting& in_setting);
    void Shutdown();

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

    void SetSamplerState(ID3D11SamplerState* in_samplerState, uint32 in_slot, eShaderFlags in_flag);
    void SetSamplerStates(const  std::vector<ID3D11SamplerState*>& in_samplerStates, uint32 in_startSlot, eShaderFlags in_flag);
    void SetRasterizerState(ID3D11RasterizerState* in_rasterizerState);
    void SetDepthStencilState(ID3D11DepthStencilState* in_depthStencilState);
    void SetBlendState(ID3D11BlendState* in_blendState);

    void SetShaderResourceView(ID3D11ShaderResourceView* in_srv, uint32 in_slot, eShaderFlags in_flag);
    void SetShaderResourceViews(const std::vector<ID3D11ShaderResourceView*>& in_srvs, uint32 in_startSlot, eShaderFlags in_flag);

    void SetConstantBuffer(ID3D11Buffer* in_constantBuffer, uint32 in_slot, eShaderFlags in_flag);

    void SetRenderTarget(ID3D11RenderTargetView* in_renderTargetView, ID3D11DepthStencilView* in_depthStencilView);
    void SetRenderTargets(const std::vector<ID3D11RenderTargetView*>& in_renderTargetViews, ID3D11DepthStencilView* in_depthStencilView);

    void SetViewport(const Viewport& in_viewport);
    void SetBackBufferToRenderTarget();

    // - Clear
    void ClearBackBuffer(const Color& in_color, bool in_clearDepth, bool in_clearStencil);
    void ClearBackBuffer(const Color& in_color);

    // - Swap Chain
    void BeginRender();
    void EndRender();
    void Present();

    // - Getter (Native)
    ComPtr<ID3D11Device>        GetDevice() const;
    ComPtr<ID3D11DeviceContext> GetContext() const;
    Ref<D11_Swapchain>          GetSwapChain() const;


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

    // Default Resources
    Ref<D11_SamplerState> m_defaultSampler;

    //===================================================
    // Pipeline State
    //===================================================

    using PipelineConstantBufferArray = std::array<ID3D11Buffer*, CONSTANT_BUFFER_MAX>;

    // Input Assembler
    D3D11_PRIMITIVE_TOPOLOGY m_topology     = D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED;
    ID3D11Buffer*            m_vertexBuffer = nullptr;
    ID3D11Buffer*            m_indexBuffer  = nullptr;

    // Vertex Shader
    ID3D11VertexShader*         m_vertexShader = nullptr;
    PipelineConstantBufferArray m_vsConstantBuffers;

    // Pixel Shader
    ID3D11PixelShader*          m_pixelShader = nullptr;
    PipelineConstantBufferArray m_psConstantBuffers;

    // RenderState (sampler state exist in texture)
    ID3D11RasterizerState*   m_rsState = nullptr;
    ID3D11DepthStencilState* m_dsState = nullptr;
    ID3D11BlendState*        m_bState  = nullptr;
};

}   // namespace crab