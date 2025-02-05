#pragma once
#include "D11_Utiles.h"

namespace crab
{

class D11_Imgui;
struct D11_VertexShader;
struct D11_PixelShader;
struct D11_VertexBuffer;
struct D11_IndexBuffer;
struct Texture;
struct D11_FrameBuffer;
struct D11_RasterizerState;
struct D11_DepthStencilState;
struct D11_BlendState;
struct D11_ConstantBufferBase;
struct D11_SamplerState;
struct TextureArray;

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

    void SetVertexBuffer(const Ref<D11_VertexBuffer>& in_vertexBuffer);
    void SetIndexBuffer(const Ref<D11_IndexBuffer>& in_indexBuffer);

    void SetVertexShader(const Ref<D11_VertexShader>& in_vertexShader);
    void SetPixelShader(const Ref<D11_PixelShader>& in_pixelShader);

    void SetRasterizerState(const Ref<D11_RasterizerState>& in_rasterizerState);
    void SetDepthStencilState(const Ref<D11_DepthStencilState>& in_depthStencilState);
    void SetBlendState(const Ref<D11_BlendState>& in_blendState);

    void SetTexture(const Ref<Texture>& in_texture, uint32 in_slot, eShaderFlags in_flag);
    void SetTextureArray(const TextureArray& in_textures, uint32 in_startSlot, eShaderFlags in_flag);
    void SetConstantBuffer(const Ref<D11_ConstantBufferBase>& in_constantBuffer, uint32 in_slot, eShaderFlags in_flag);

    void SetFrameBuffer(const Ref<D11_FrameBuffer>& in_frameBuffer);
    void SetBackBufferToFrameBuffer();

    // - Clear
    void ClearFrameBuffer(const Color& in_color, bool in_clearDepth, bool in_clearStencil);
    void ClearFrameBuffer(const Color& in_color);

    // - Swap Chain
    void BeginRender();
    void EndRender();
    void Present();

    // - Getter (Native)
    ComPtr<ID3D11Device>        GetDevice() const;
    ComPtr<ID3D11DeviceContext> GetContext() const;
    ComPtr<IDXGISwapChain>      GetSwapChain() const;
    Ref<D11_FrameBuffer>        GetSwapChainFrameBuffer() const;

private:
    // Device
    ComPtr<ID3D11Device>        m_device;
    ComPtr<ID3D11DeviceContext> m_deviceContext;
    ComPtr<ID3D11Debug>         m_debug;       // only debug
    ComPtr<ID3D11InfoQueue>     m_infoQueue;   // only debug

    // Swap Chain
    ComPtr<IDXGISwapChain1> m_swapChain;
    bool                    m_vsync;

    // Feature Level
    D3D_FEATURE_LEVEL m_featureLevel;

    // imgui
    Scope<D11_Imgui> m_imgui;

    // Default Resources
    Ref<D11_FrameBuffer>  m_swapChainBackbuffer;
    Ref<D11_SamplerState> m_defaultSamplerState;

    // Pipeline State
    using PipelineConstantBufferArray = std::array<Ref<D11_ConstantBufferBase>, CONSTANT_BUFFER_MAX>;

    // FrameBuffer
    Ref<D11_FrameBuffer> m_frameBuffer;

    // Input Assembler
    eTopology             m_topology     = eTopology::None;
    Ref<D11_VertexBuffer> m_vertexBuffer = nullptr;
    Ref<D11_IndexBuffer>  m_indexBuffer  = nullptr;

    // Vertex Shader
    Ref<D11_VertexShader>       m_vertesShader = nullptr;
    PipelineConstantBufferArray m_vsConstantBuffers;

    // Pixel Shader
    Ref<D11_PixelShader>        m_pixelShader = nullptr;
    PipelineConstantBufferArray m_psConstantBuffers;

    // RenderState (sampler state exist in texture)
    Ref<D11_RasterizerState>   m_rsState = nullptr;
    Ref<D11_DepthStencilState> m_dsState = nullptr;
    Ref<D11_BlendState>        m_bState  = nullptr;
};

}   // namespace crab