#pragma once

namespace crab
{

//===================================================
// Application Window
//===================================================

struct AppWindowSetting
{
    std::string windowTitle  = "Crab Renderer";
    uint32      windowWidth  = 1280;
    uint32      windowHeight = 720;
    uint32      windowPosX   = CENTER_ALIGNMENT;
    uint32      windowPosY   = CENTER_ALIGNMENT;

    enum : uint32
    {
        CENTER_ALIGNMENT = CRAB_UINT32_MAX
    };
};

//===================================================
// Log
//===================================================

enum class eLogLevel
{
    Trace,
    Debug,
    Info,
    Warn,
    Error,
    Critical,
    Off
};

struct LogSetting
{
    eLogLevel             logLevel     = eLogLevel::Trace;
    std::string           logPattern   = "[%Y-%m-%d %H:%M:%S.%e] [%l] %v";
    std::filesystem::path logDirectory = std::filesystem::current_path() / "Logs";

    // - Adv Settings
    uint64 threadPoolSize = 8192;   // Thread pool size for async logging
    uint32 threadCount    = 1;      // Thread count for async logging
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
    eFormat depthBufferFormat = eFormat::Depth_Float32_Stencil_UInt8;

    bool enableVSync        = true;
    bool enableHDRRendering = false;

    // MSAA
    bool  enableMSAA      = false;
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

    // - Log
    LogSetting logSetting = {};

    // - Renderer
    RendererSetting rendererSetting = {};
};

}   // namespace crab