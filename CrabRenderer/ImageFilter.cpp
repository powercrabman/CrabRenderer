#include "CrabPch.h"

#include "ImageFilter.h"

#include "AppWindow.h"
#include "Buffers.h"

#include "D11Renderer.h"
#include "Mesh.h"
#include "RenderStates.h"
#include "RenderTarget.h"
#include "Shaders.h"
#include "Textures.h"

namespace crab
{

void ImageFilter::Init(
    uint32                   in_filterWidth,
    uint32                   in_filterHeight,
    const Ref<VertexShader>& in_vertexShader,
    const Ref<PixelShader>&  in_pixelShader,
    const TextureList&       in_inputTextures,
    const SamplerList&       in_samplerLists,
    const ConstantList&      in_constantBuffers)
{
    auto d = GetRenderer().GetDevice();

    CRAB_ASSERT(in_vertexShader, "Invalid Vertex Shader.");
    CRAB_ASSERT(in_pixelShader, "Invalid Pixel Shader.");

    auto [scrWidth, scrHeight] = GetApplication().GetAppWindow().GetResolution();

    uint32 width  = in_filterWidth;
    uint32 height = in_filterHeight;

    if (width == 0)
        width = scrWidth;

    if (height == 0)
        height = scrHeight;

    // - Frame Buffer
    ComPtr<ID3D11Texture2D> texture;
    D3D11_TEXTURE2D_DESC    texDesc = {};
    texDesc.Width                   = width;
    texDesc.Height                  = height;
    texDesc.MipLevels               = 1;
    texDesc.ArraySize               = 1;
    texDesc.Format                  = DXGI_FORMAT_R16G16B16A16_FLOAT;
    texDesc.SampleDesc.Count        = 1;
    texDesc.SampleDesc.Quality      = 0;
    texDesc.Usage                   = D3D11_USAGE_DEFAULT;
    texDesc.BindFlags               = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
    texDesc.CPUAccessFlags          = 0;
    texDesc.MiscFlags               = 0;

    CheckD3D11Result(d->CreateTexture2D(&texDesc,
                                        nullptr,
                                        texture.GetAddressOf()),
                     "CreateTexture2D Fail.");

    Ref<RenderTarget> rt = CreateRef<RenderTarget>();
    rt->Init(texture.Get());

    Viewport vp = {};
    vp.width    = static_cast<float>(width);
    vp.height   = static_cast<float>(height);
    vp.minDepth = 0.f;
    vp.maxDepth = 1.f;
    vp.x        = 0.f;
    vp.y        = 0.f;

    m_renderTarget = rt;
    m_viewport     = vp;

    m_vertexShader = in_vertexShader;
    m_pixelShader  = in_pixelShader;

    m_constantBuffers = in_constantBuffers;
    m_samplerStates   = in_samplerLists;
    m_inputImages     = in_inputTextures;
}

Ref<ImageFilter> ImageFilter::Clone() const
{
    Ref<ImageFilter> filter = CreateRef<ImageFilter>();

    filter->m_renderTarget = m_renderTarget;
    filter->m_viewport     = m_viewport;

    filter->m_vertexShader = m_vertexShader;
    filter->m_pixelShader  = m_pixelShader;

    filter->m_inputImages     = m_inputImages;
    filter->m_samplerStates   = m_samplerStates;
    filter->m_constantBuffers = m_constantBuffers;

    return filter;
}

Ref<Texture2D> ImageFilter::GetOutputTexture() const
{
    return m_renderTarget->GetTexture();
}

void ImageFilter::Bind() const
{

    // unbind before render target and shader resource view
    // to avoid resource conflict
    // shader resource cannot be used input and output at the same time
    GetRenderer().ReleaseRenderTargets();
    GetRenderer().SetTopology(eTopology::TriangleList);

    m_inputImages.Bind();
    m_renderTarget->Bind();
    GetRenderer().SetViewport(m_viewport);

    m_constantBuffers.Bind();
    m_samplerStates.Bind();

    GetRenderer().SetGeometryShader(nullptr);
    GetRenderer().SetHullShader(nullptr);
    GetRenderer().SetDomainShader(nullptr);

    m_vertexShader->Bind();
    m_pixelShader->Bind();
}

void ImageFilter::SetRenderTarget(const Ref<RenderTarget>& in_rt)
{
    m_renderTarget = in_rt;
}

}   // namespace crab
