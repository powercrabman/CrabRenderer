#pragma once
#include "CrabEnums.h"

namespace crab
{

//===================================================
// Application Window
//===================================================

struct AppWindowSetting
{
    std::string windowTitle = "Crab Renderer";
    Int2        windowSize  = { 1280, 720 };
    Int2        windowPos   = { CENTER_ALIGNMENT, CENTER_ALIGNMENT };

    // clang-format off
    enum : int32 { CENTER_ALIGNMENT = CRAB_INT32_MAX };
    // clang-format on
};

//===================================================
// Renderer
//===================================================

enum class eRendererAPI
{
    DirectX11
};

struct SwapChainSetting
{
    eFormat swapChainFormat   = eFormat::UNorm8x4;
    eFormat depthBufferFormat = eFormat::Depth_UNorm24_Stencil_UInt8;

    bool enableVSync        = true;
    bool enableHDRRendering = false;

    // MSAA
    bool enableMSAA = false;
};

struct RendererSetting
{
    eRendererAPI rendererAPI = eRendererAPI::DirectX11;

    SwapChainSetting swapChainSetting = {};
};

//===================================================
// Application
//===================================================

struct ApplicationSetting
{
    // - Application
    std::string applicationName = "Crab Renderer";

    // - Window
    AppWindowSetting windowSetting = {};

    // - Renderer
    RendererSetting rendererSetting = {};

    std::string engineDirectory = "";
};

}   // namespace crab