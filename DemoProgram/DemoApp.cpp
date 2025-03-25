#include "pch.h"

#include "DemoApp.h"

#include "TestScene.h"


DemoApp::DemoApp(const ApplicationCreateInfo& info)
    : Application(info)
{
}

DemoApp::~DemoApp()
{
}

void DemoApp::OnInit()
{
    SceneManager& sm = GetSceneManager();

    sm.CreateScene<TestScene>();
    sm.ChangeScene<TestScene>();
}

void DemoApp::OnShutdown()
{
}
