#pragma once
#include "RenderCommon.h"

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
    enum : int32 { CENTER_ALIGNMENT = -1 };
    // clang-format on
};

//===================================================
// Renderer
//===================================================

enum class eRendererAPI
{
    DirectX11
};

struct RendererSetting
{
    eRendererAPI rendererAPI = eRendererAPI::DirectX11;

    eFormat backBufferFormat      = eFormat::UNorm8_4;
    eFormat backBufferDepthFormat = eFormat::Depth_UNorm24_Stencil_UInt8;

    bool enableVSync = true;
    bool enableHDR   = false;
    bool enableMSAA  = false;
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

    std::string engineDirectory = {};
};

}   // namespace crab