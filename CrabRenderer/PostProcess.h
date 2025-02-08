#pragma once

#include "D11_ConstBuffer.h"
#include "D11_Texture.h"

namespace crab
{

class D11_FrameBuffer;
class D11_FrameBufferArray;
class D11_VertexShader;
class D11_PixelShader;
class D11_PixelShader;
class D11_RenderTarget;
class D11_Mesh;

struct PostProcessSetting
{
    // - Render Target
    uint32      renderTargetWidth  = 0;   // if 0 then use back buffer size
    uint32      renderTargetHeight = 0;   // if 0 then use back buffer size
    DXGI_FORMAT renderTargetFormat = DXGI_FORMAT_R32G32B32A32_FLOAT;

    // - Viewport
    uint32 viewportTopLeftX = 0;
    uint32 viewportTopLeftY = 0;

    // - Shader
    Ref<D11_VertexShader> vertexShader;
    Ref<D11_PixelShader>  pixelShader;

    // - Sampler
    Ref<D11_SamplerState> samplerState = nullptr;   // if nullptr then use default sampler
};

// Constant Buffer
struct alignas(16) CB_PostProcess
{
    float dx;
    float dy;
    float threshold;
    float strength;
    Vec4  userValues;
};

struct PostProcessingVertex
{
    Vec2 position;
    Vec2 texcoord;

    inline static D11_InputElements s_inputElements = []()
    {
        D11_InputElements elem;
        elem.Add("POSITION", 0, DXGI_FORMAT_R32G32_FLOAT)
            .Add("TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT);
        return elem;
    }();
};

class PostProcess
{
public:
    static Ref<PostProcess> Create(const PostProcessSetting& in_setting);

    void SetRenderTarget(const Ref<D11_FrameBuffer>& in_fb);
    void SetRenderTargets(const std::vector<Ref<D11_FrameBuffer>>& in_fbs);

    void SetTexture(const Ref<D11_Texture>& in_tex);
    void SetTextures(const std::vector<Ref<D11_Texture>>& in_texs);

    void UpdateConstantData(const CB_PostProcess& in_constatnData);
    void UpdateConstantData(float in_threshhold, float in_strength);
    void UpdateConstantData(float in_threshhold, float in_strength, const Vec4& in_userData);

    void Render() const;

    Ref<D11_Texture> GetTexture() const;

    using Vertex = PostProcessingVertex;

private:
    using D11_ConstantBuffer = D11_ConstantBuffer<CB_PostProcess>;

    Ref<D11_FrameBuffer>    m_frameBuffer;
    Ref<D11_VertexShader>   m_vertexShader;
    Ref<D11_PixelShader>    m_pixelShader;
    Ref<D11_ConstantBuffer> m_cbPostProcessing;
    Ref<D11_Mesh>           m_quadMesh;
    Ref<D11_SamplerState>   m_samplerState;

    float  m_deltaX;
    float  m_deltaY;

    std::vector<ID3D11RenderTargetView*> m_rtvs;
    std::vector<ID3D11ShaderResourceView*> m_dsvs;
};

}   // namespace crab