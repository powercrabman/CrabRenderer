#pragma once

namespace crab
{

class D11_Texture2D;

class D11_DepthStencil
{
public:
    // - Factory
    static Ref<D11_DepthStencil> Create(DXGI_FORMAT in_format);

    void Clear(bool in_clearDepth, bool in_clearStencil);

    ID3D11DepthStencilView* Get() const { return m_dsv.Get(); }
    DXGI_FORMAT             GetFormat() const { return m_format; }

private:
    ComPtr<ID3D11DepthStencilView> m_dsv;
    DXGI_FORMAT                    m_format;
};

}   // namespace crab