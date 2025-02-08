#pragma once
namespace crab
{

class D11_Texture;

class D11_RenderTarget
{
public:
    // - Factory
    static Ref<D11_RenderTarget> Create(ID3D11Texture2D* in_texture);

    void SetClearColor(const Color& in_color) { m_clearColor = in_color; }

    void Clear(const Color& in_color);

    ID3D11RenderTargetView* Get() const { return m_renderTargetView.Get(); }
    Ref<D11_Texture>        GetTexture() const { return m_texture; }
    DXGI_FORMAT             GetFormat() const { return m_format; }

private:
    ComPtr<ID3D11RenderTargetView> m_renderTargetView;
    Ref<D11_Texture>               m_texture;
    DXGI_FORMAT                    m_format;
    Color                          m_clearColor = color::WHITE;
};

}   // namespace crab