#pragma once
namespace crab
{

class D11_Texture2D;
class D11_DepthBuffer;

class D11_RenderTarget
{
public:
    // - Factory
    static Ref<D11_RenderTarget> Create(ID3D11Texture2D* in_texture);

    void Bind();
    void Clear(const Color& in_color);

    ID3D11RenderTargetView* Get() const { return m_renderTargetView.Get(); }
    Ref<D11_Texture2D>      GetTexture() const { return m_texture; }
    DXGI_FORMAT             GetFormat() const { return m_format; }

private:
    ComPtr<ID3D11RenderTargetView> m_renderTargetView;
    Ref<D11_Texture2D>             m_texture;
    DXGI_FORMAT                    m_format;
};

}   // namespace crab