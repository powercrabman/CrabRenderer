#pragma once

class DemoApp : public Application
{
public:
    DemoApp(const ApplicationSetting& in_setting);
    ~DemoApp() override;

    void OnInit() override;
    void OnShutdown() override;

    void PreLoadResources();
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
    setting.windowSetting.windowSize  = { 1600, 900 };

    setting.rendererSetting.swapChainSetting.enableVSync        = true;
    setting.rendererSetting.swapChainSetting.enableHDRRendering = true;
    setting.rendererSetting.swapChainSetting.enableMSAA         = true;

    return new DemoApp(setting);
}
