#include "pch.h"

#include "DemoApp.h"

#include "30. DemoScene.h"

DemoApp::DemoApp(const ApplicationSetting& in_setting)
    : Application(in_setting)
{
}

DemoApp::~DemoApp()
{
}

void DemoApp::OnInit()
{
    SceneManager& sm = GetSceneManager();

    sm.CreateScene<DemoScene>();
    sm.ChangeScene<DemoScene>();
}

void DemoApp::OnShutdown()
{
}


