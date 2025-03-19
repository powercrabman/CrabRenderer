#pragma once

namespace crab
{

class Texture2D;
class DepthBuffer;

//===================================================
// RenderTarget
//===================================================

class RenderTarget
{
public:
    // - Factory
    void Init(
        uint32     in_width,
        uint32     in_height,
        eFormat    in_format,
        eBindFlags in_bindFlags,
        MSAA       in_MSAA);

    void Init(ID3D11Texture2D* in_texture);

    void Bind() const;
    void Bind(const Ref<DepthBuffer>& in_depthBuffer) const;

    void Clear(const Color4& in_color) const;

    ID3D11RenderTargetView* GetRTV() const { return m_renderTargetView.Get(); }
    eFormat                 GetFormat() const { return m_format; }
    Ref<Texture2D>          GetTexture() const { return m_image; }

private:
    ComPtr<ID3D11RenderTargetView> m_renderTargetView;

    Ref<Texture2D> m_image;
    eFormat        m_format;
};

}   // namespace crab