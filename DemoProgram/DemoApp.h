#pragma once
#include "inc/EngineConfigs.h"

class DemoApp : public Application
{
public:
    DemoApp(const ApplicationCreateInfo& in_info);
    ~DemoApp() override;

    void OnInit() override;
    void OnShutdown() override;
};

inline Application* CreateCrabApplication(const CommandLineArgs& in_args)
{
    // Command Line Args
    CommandLineArgs args = in_args;
    for (const auto& arg: args.args)
        Log::Trace(arg.c_str());

    // Application Setting
    ApplicationCreateInfo info;

    info.applicationName = "Demo Application";

    using namespace std::chrono;
    auto           now   = system_clock::now();
    auto           today = floor<days>(now);
    year_month_day ymd   = year_month_day { today };
    info.windowTitle     = fmt::format("DemoApp - {0}/{1}/{2}",
                                   static_cast<int32>(ymd.year()),
                                   static_cast<uint32>(ymd.month()),
                                   static_cast<uint32>(ymd.day()));

    info.windowWidth  = 1920;
    info.windowHeight = 1080;

    info.windowPosX = 0;
    info.windowPosY = 0;

    info.flags = eApplicationCreateFlags_Window_CenterAlign |
                 eApplicationCreateFlags_Renderer_EnableDepthBuffer |
                 eApplicationCreateFlags_Renderer_EnableMSAA |
                 // eApplicationCreateFlags_Renderer_EnableHDR |
                 eApplicationCreateFlags_Renderer_EnableVSync;

    info.backBufferFormat            = eFormat::UNorm8_4;
    info.backBufferDepthBufferFormat = eFormat::Depth_UNorm24_Stencil_UInt8;
    info.HDRBackBufferFormat         = eFormat::Float16_4;

    info.projectDirectories.engineDirectory  = R"(C:\Users\Ahnjiwoo\Desktop\Projects\CrabRenderer\CrabEngine)";
    info.projectDirectories.shaderDirectory  = R"(C:\Users\Ahnjiwoo\Desktop\Projects\CrabRenderer\CrabRenderer\Shaders)";
    info.projectDirectories.projectDirectory = R"(C:\Users\Ahnjiwoo\Desktop\Projects\CrabRenderer\DemoProgram)";

    return new DemoApp(info);
}

/*
 *
 *  todo 1 : m_mainFrameBuffer
 *  todo 2 : post process 현재 프레임워크에 맞게 수정
 *
 *
 */