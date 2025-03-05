#include "pch.h"

#include "DemoApp.h"

#include "GlobalRenderPass.h"
#include "GlobalShader.h"

#include "28. AnimationDemo.h"

DemoApp::DemoApp(const ApplicationSetting& in_setting)
    : Application(in_setting)
{
}

DemoApp::~DemoApp()
{
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
