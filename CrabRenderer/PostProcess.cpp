#include "CrabPch.h"

#include "PostProcess.h"

#include "AppWindow.h"
#include "D11_Buffers.h"
#include "D11_FrameBuffer.h"
#include "D11_Mesh.h"
#include "D11_RenderState.h"
#include "D11_RenderTarget.h"
#include "D11_Renderer.h"
#include "D11_Shaders.h"
#include "D11_Texture.h"

namespace crab
{

Ref<PostProcess> PostProcess::Create(const PostProcessSetting& in_setting)
{
    auto             d  = D11_API->GetDevice();
    Ref<PostProcess> pp = CreateRef<PostProcess>();

    CRAB_ASSERT(in_setting.vertexShader, "Invalid Vertex Shader.");
    CRAB_ASSERT(in_setting.pixelShader, "Invalid Pixel Shader.");

    auto [scrWidth, scrHeight] = GetApplication().GetAppWindow().GetWindowSize();

    uint32 width  = in_setting.renderTargetWidth;
    uint32 height = in_setting.renderTargetHeight;

    if (width == 0)
        width = scrWidth;

    if (height == 0)
        height = scrHeight;

    // - Constant Buffer
    pp->m_cbPostProcessing = D11_ConstantBuffer::Create(0, eShaderFlags_PixelShader);

    // - Frame Buffer
    ComPtr<ID3D11Texture2D> texture;
    D3D11_TEXTURE2D_DESC    texDesc = {};
    texDesc.Width                   = width;
    texDesc.Height                  = height;
    texDesc.MipLevels               = 1;
    texDesc.ArraySize               = 1;
    texDesc.Format                  = in_setting.renderTargetFormat;
    texDesc.SampleDesc.Count        = 1;
    texDesc.SampleDesc.Quality      = 0;
    texDesc.Usage                   = D3D11_USAGE_DEFAULT;
    texDesc.BindFlags               = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
    texDesc.CPUAccessFlags          = 0;
    texDesc.MiscFlags               = 0;

    D11_ASSERT(d->CreateTexture2D(&texDesc, nullptr, texture.GetAddressOf()), "CreateTexture2D Fail.");

    Ref<D11_RenderTarget> rt = D11_RenderTarget::Create(texture.Get());

    Viewport vp = {};
    vp.width    = (float)width;
    vp.height   = (float)height;
    vp.minDepth = 0.f;
    vp.maxDepth = 1.f;
    vp.x        = (float)in_setting.viewportTopLeftX;
    vp.y        = (float)in_setting.viewportTopLeftY;

    pp->m_frameBuffer = D11_FrameBuffer::Create(rt, vp);

    // - Shader
    pp->m_vertexShader = in_setting.vertexShader;
    pp->m_pixelShader  = in_setting.pixelShader;

    // - Mesh
    std::vector<Vertex> vertices = {
        Vertex { Vec2 { -1.f, 1.f }, Vec2 { 0.f, 0.f } },
        Vertex { Vec2 { 1.f, 1.f }, Vec2 { 1.f, 0.f } },
        Vertex { Vec2 { 1.f, -1.f }, Vec2 { 1.f, 1.f } },
        Vertex { Vec2 { -1.f, -1.f }, Vec2 { 0.f, 1.f } }
    };

    std::vector<UINT> indices = {
        0, 1, 2, 0, 2, 3
    };

    pp->m_quadMesh = D11_Mesh::Create(vertices, indices);

    pp->m_deltaX = 1.f / width;
    pp->m_deltaY = 1.f / height;

    // sampler state
    if (in_setting.samplerState)
        pp->m_samplerState = in_setting.samplerState;
    else
        pp->m_samplerState = D11_SamplerState::Create(eSamplerFilter::Linear,
                                                      eSamplerAddress::Clamp);

    // default render target
    pp->SetRenderTarget(pp->m_frameBuffer);
    pp->UpdateConstantData(0.f, 0.f, Vec4 { 0.f, 0.f, 0.f, 0.f });   // default value

    return pp;
}

void PostProcess::SetRenderTarget(const Ref<D11_FrameBuffer>& in_fb)
{
    m_rtvs.clear();
    m_rtvs.push_back(in_fb->GetRenderTarget()->Get());
}

void PostProcess::SetRenderTargets(const std::vector<Ref<D11_FrameBuffer>>& in_fbs)
{
    m_rtvs.clear();
    for (const auto& fb: in_fbs)
        m_rtvs.push_back(fb->GetRenderTarget()->Get());
}

void PostProcess::SetTexture(const Ref<D11_Texture>& in_tex, uint32 in_slot)
{
    if (m_textures.size() < in_slot + 1)
        m_textures.resize(in_slot + 1, nullptr);

    m_textures[in_slot] = in_tex;
}

void PostProcess::UpdateConstantData(float in_threshhold, float in_strength)
{
    CB_PostProcess cbData;
    cbData.dx        = m_deltaX;
    cbData.dy        = m_deltaY;
    cbData.threshold = in_threshhold;
    cbData.strength  = in_strength;
    m_cbPostProcessing->UpdateData(cbData);
}

void PostProcess::UpdateConstantData(float in_threshhold, float in_strength, const Vec4& in_userData)
{
    CB_PostProcess cbData;
    cbData.dx         = m_deltaX;
    cbData.dy         = m_deltaY;
    cbData.threshold  = in_threshhold;
    cbData.strength   = in_strength;
    cbData.userValues = in_userData;
    m_cbPostProcessing->UpdateData(cbData);
}

void PostProcess::UpdateConstantData(const CB_PostProcess& in_constatnData)
{
    m_deltaX = in_constatnData.dx;
    m_deltaY = in_constatnData.dy;

    m_cbPostProcessing->UpdateData(in_constatnData);
}

void PostProcess::Render() const
{
    auto* dx = D11_API;

    // unbind before render target and shader resource view
    // to avoid resource conflict
    // shader resource cannot be used input and output at the same time
    dx->ReleaseShaderResourceViews(eShaderFlags_PixelShader);

    dx->SetTopology(eTopology::TriangleList);
    dx->SetRenderTargets(m_rtvs, nullptr);
    dx->SetViewport(m_frameBuffer->GetViewport());
    dx->SetSamplerState(m_samplerState->Get(), 0, eShaderFlags_PixelShader);

    for (uint32 i = 0; i < m_textures.size(); ++i)
        m_textures[i]->Bind(i, eShaderFlags_PixelShader);

    if (m_geometryShader)
        m_geometryShader->Bind();
    else
        dx->SetGeometryShader(nullptr);

    if (m_computeShader)
        m_computeShader->Bind();
    else
        dx->SetComputeShader(nullptr);

    m_vertexShader->Bind();
    m_pixelShader->Bind();
    m_cbPostProcessing->Bind();
    m_quadMesh->Draw();
}

Ref<D11_Texture> PostProcess::GetTexture() const
{
    return m_frameBuffer->GetRenderTarget()->GetTexture();
}

}   // namespace crab
