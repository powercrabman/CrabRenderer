#pragma once
#include "D11Utils.h"

namespace crab
{

class Image2D;

class DepthBuffer
{
    D11_RESOURCE_CTOR(DepthBuffer);

public:
    // Factory
    static Ref<DepthBuffer> Create(
        uint32  in_width,
        uint32  in_height,
        eFormat in_depthBufferFormat,
        uint32  in_MSAASampleCount,
        uint32  in_MSAAQuality);

    static Ref<DepthBuffer> CreateWithImage2D(
        uint32  in_width,
        uint32  in_height,
        eFormat in_depthBufferFormat,
        uint32  in_MSAASampleCount,
        uint32  in_MSAAQuality,
        eFormat in_imageFormat);

    void Clear(
        bool  in_clearDepth,
        float in_clearDepthFactor,
        bool  in_clearStencil,
        int32 in_clearStencilFactor) const;

    ID3D11DepthStencilView* Get() const
    {
        return m_dsv.Get();
    }

    Ref<Image2D> GetImage() const
    {
        return m_image;
    }

    eFormat GetFormat() const
    {
        return m_format;
    }

private:
    ComPtr<ID3D11DepthStencilView> m_dsv;
    Ref<Image2D>                   m_image;
    eFormat                        m_format = eFormat::Unknown;
};

}   // namespace crab