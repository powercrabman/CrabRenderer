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

struct SwapChainFrameBufferSetting
{
    // swap chain
    DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM;

    // depth stencil
    bool        useDepthStencil    = true;
    DXGI_FORMAT depthStencilFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

    // viewport
    Viewport viewport = { 0.f, 0.f, 0.f, 0.f, 0.f, 1.f };
};

struct RendererSetting
{
    eRendererAPI rendererAPI = eRendererAPI::DirectX11;

    SwapChainFrameBufferSetting swapChainFrameBufferSetting = {};
    bool                        useVSync                    = true;
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