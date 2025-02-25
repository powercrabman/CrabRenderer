#pragma once

namespace crab
{

class Image2D;
class DepthBuffer;

//===================================================
// RenderTarget
//===================================================

class RenderTarget
{
public:
    // - Factory
    static Ref<RenderTarget> Create(
        uint32 in_width,
        uint32 in_height,
        eFormat in_format,
        uint32  in_MSAASampleCount,
        uint32  in_MSAAQuality);

    static Ref<RenderTarget> Create(ID3D11Texture2D* in_texture);

    void Bind() const;
    void Bind(const Ref<DepthBuffer>& in_depthBuffer) const;

    void Clear(const Color& in_color) const;

    ID3D11RenderTargetView* Get() const { return m_renderTargetView.Get(); }
    eFormat                 GetFormat() const { return m_format; }
    Ref<Image2D>            GetImage() const { return m_image; }

private:
    ComPtr<ID3D11RenderTargetView> m_renderTargetView;

    Ref<Image2D> m_image;
    eFormat      m_format = eFormat::Unknown;
};

}   // namespace crab