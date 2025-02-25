#include "pch.h"

#include "DemoApp.h"

#include "GlobalRenderPass.h"
#include "GlobalShader.h"

#include "28. AnimationDemo.h"

DemoApp::DemoApp()
{
}

DemoApp::~DemoApp()
{
}

ApplicationSetting DemoApp::ConfigureApplication()
{
    ApplicationSetting setting = {};
    setting.applicationName    = "Demo App";

    setting.windowSetting.windowTitle  = "Demo App";
    setting.windowSetting.windowWidth  = 1920;
    setting.windowSetting.windowHeight = 1080;

    setting.rendererSetting.swapChainSetting.enableVSync        = true;
    setting.rendererSetting.swapChainSetting.enableHDRRendering = true;
    setting.rendererSetting.swapChainSetting.enableMSAA         = true;

    return setting;
}

void DemoApp::OnInit()
{
    PreLoadResources();

    SceneManager& sm = GetSceneManager();

    sm.CreateScene<AnimationDemo>();

    sm.ChangeScene<AnimationDemo>();
}

void DemoApp::OnShutdown()
{
}

void DemoApp::PreLoadResources()
{
    GetGlobalConstants().Init();
    GetShaderLibrary().Init();
    GetGlobalRenderPass().Init();
}
