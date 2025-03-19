#pragma once

class DemoApp : public Application
{
public:
    DemoApp(const ApplicationSetting& in_setting);
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
    ApplicationSetting setting = {};
    setting.applicationName    = "Demo App";

    setting.windowSetting.windowTitle = "Demo App";
    setting.windowSetting.windowSize  = { 1920, 1080 };

    setting.rendererSetting.rendererAPI = eRendererAPI::DirectX11;

    setting.rendererSetting.backBufferDepthFormat = eFormat::Depth_UNorm24_Stencil_UInt8;
    setting.rendererSetting.backBufferFormat      = eFormat::UNorm8_4;

    setting.rendererSetting.enableMSAA  = true;
    setting.rendererSetting.enableHDR   = true;
    setting.rendererSetting.enableVSync = false;

    setting.engineDirectory = R"(C:\Users\Ahnjiwoo\Desktop\Projects\CrabRenderer\CrabRenderer)";

    return new DemoApp(setting);
}
