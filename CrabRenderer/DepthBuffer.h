#pragma once
#include "CrabEnums.h"
#include "D11Utils.h"

namespace crab
{

class Texture;

class DepthBuffer
{
public:
    // Factory
    void Init(
        uint32  in_width,
        uint32  in_height,
        eFormat in_depthBufferFormat,
        uint32  in_MSAASampleCount,
        uint32  in_MSAAQuality);

    void Init(
        ID3D11Texture2D* in_texture,
        eFormat          in_depthBufferFormat = eFormat::Unknown);

    void Clear(
        bool  in_clearDepth,
        float in_clearDepthFactor,
        bool  in_clearStencil,
        int32 in_clearStencilFactor) const;

    ID3D11DepthStencilView* Get() const
    {
        return m_dsv.Get();
    }

    Int2 GetResolution() const
    {
        return m_resolution;
    }

    float GetAspect() const
    {
        return static_cast<float>(m_resolution.x) / static_cast<float>(m_resolution.y);
    }

    eFormat GetFormat() const
    {
        return m_format;
    }

private:
    ComPtr<ID3D11DepthStencilView> m_dsv;
    eFormat                        m_format = eFormat::Unknown;
    Int2                           m_resolution;
};

}   // namespace crab