#include "CrabPch.h"

#include "D11_RenderTarget.h"

#include "D11_DepthBuffer.h"
#include "D11_RenderState.h"
#include "D11_Renderer.h"
#include "D11_Texture.h"

namespace crab
{

Ref<D11_RenderTarget> D11_RenderTarget::Create(ID3D11Texture2D* in_texture)
{
    auto d = D11_API->GetDevice();

    Ref<D11_RenderTarget> rt = CreateRef<D11_RenderTarget>();

    D3D11_TEXTURE2D_DESC texDesc = {};
    in_texture->GetDesc(&texDesc);

    // - Create Render Target View
    D3D11_RENDER_TARGET_VIEW_DESC desc = {};
    desc.Format                        = texDesc.Format;
    desc.Texture2D.MipSlice            = 0;

    if (texDesc.SampleDesc.Count > 1)
        desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DMS;
    else
        desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

    D11_ASSERT(d->CreateRenderTargetView(in_texture,
                                         &desc,
                                         rt->m_renderTargetView.GetAddressOf()),
               "CreateRenderTargetView Fail.");

    if (BIT_AND(texDesc.BindFlags, D3D11_BIND_SHADER_RESOURCE))
    {
        // - Create Shader Resource View
        D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
        srvDesc.Format                          = texDesc.Format;
        srvDesc.Texture2D.MostDetailedMip       = 0;
        srvDesc.Texture2D.MipLevels             = 1;

        if (texDesc.SampleDesc.Count > 1)
            srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DMS;
        else
            srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;

        rt->m_texture = D11_Texture2D::Create(in_texture, srvDesc);
    }
    rt->m_format = static_cast<eD11_Format>(texDesc.Format);

    return rt;
}

void D11_RenderTarget::Bind() const
{
    D11_API->SetRenderTarget(m_renderTargetView.Get(), nullptr);
}

void D11_RenderTarget::Bind(const Ref<D11_DepthBuffer>& in_depthBuffer) const
{
    D11_API->SetRenderTarget(m_renderTargetView.Get(), in_depthBuffer->Get());
}

void D11_RenderTarget::Clear(const Color& in_color) const
{
    D11_API->GetContext()->ClearRenderTargetView(m_renderTargetView.Get(), (FLOAT*)&in_color);
}

void D11_RenderTargetList::ClearList()
{
    m_rtvs.clear();
    m_renderTargetViews.clear();
}

D11_RenderTargetList& D11_RenderTargetList::Add(Ref<D11_RenderTarget> in_renderTarget)
{
    m_rtvs.push_back(in_renderTarget);
    m_renderTargetViews.push_back(in_renderTarget->Get());

    return *this;
}

Ref<D11_RenderTarget> D11_RenderTargetList::GetRenderTarget(uint32 in_index) const
{
    return m_rtvs[in_index];
}

void D11_RenderTargetList::Bind() const
{
    D11_API->SetRenderTargets(m_renderTargetViews, nullptr);
}

void D11_RenderTargetList::Bind(const Ref<D11_DepthBuffer>& in_depthBuffer) const
{
    D11_API->SetRenderTargets(m_renderTargetViews, in_depthBuffer->Get());
}

}   // namespace crab