#pragma once

namespace crab
{

struct MSAA
{
    MSAA() = default;
    MSAA(uint32 in_sampleCount, uint32 in_quality)
        : sampleCount(in_sampleCount)
        , quality(in_quality)
    {
        enableMSAA = sampleCount > 1;
    }

    static MSAA ComputeBestMSAA(bool          enableMSAA,
                                ID3D11Device* in_device,
                                DXGI_FORMAT   in_format,
                                uint32        in_sampleCount)
    {
        if (enableMSAA)
        {
            uint32 qualityLevels = 0;
            if (d3d11::CheckOK(
                    in_device->CheckMultisampleQualityLevels(
                        in_format,
                        in_sampleCount,
                        &qualityLevels),
                    "CheckMultisampleQualityLevels Fail."))
            {
                return DisableMSAA();
            }
        }

        return DisableMSAA();
    }

    static MSAA DisableMSAA()
    {
        return MSAA { 1, 0 };
    }

    bool   enableMSAA  = false;
    uint32 sampleCount = 1;
    uint32 quality     = 0;
};

struct HDR
{
    bool enableHDR = false;
};

struct RendererConfig
{
    MSAA msaa;
    HDR  hdr;

    D3D_FEATURE_LEVEL featureLevel;

    bool enableVSync;
};

struct RendererInitInfo
{
    eFormat backBufferFormat    = eFormat::UNorm8_4;
    eFormat depthBufferFormat   = eFormat::Depth_UNorm24_Stencil_UInt8;
    eFormat HDRBackBufferFormat = eFormat::Float16_4;

    uint32 width;
    uint32 height;

    bool createHDR         = false;
    bool enableMSAA        = false;
    bool createDepthBuffer = false;
    bool vSync             = true;
    HWND nativeHandle      = nullptr;
};

}   // namespace crab