#include "pch.h"

#include "DemoApp.h"

#include "1. TriangleDemo.h"
#include "2. ColorDemo.h"
#include "3. ConstantBufferDemo.h"
#include "4. CameraDemo.h"
#include "5. Texture2DDemo.h"
#include "6. LightDemo.h"
#include "7. ModelDemo.h"
#include "8. CubeMapDemo.h"
#include "9. ImageBasedLightningDemo.h"

ApplicationSetting DemoApp::ConfigureApplication()
{
    ApplicationSetting setting = {};
    setting.applicationName    = "Demo App";

    setting.windowSetting.windowTitle  = "Demo App";
    setting.windowSetting.windowWidth  = 1920;
    setting.windowSetting.windowHeight = 1080;

    setting.rendererSetting.useVSync = true;
    return setting;
}

void DemoApp::OnInit()
{
    SceneManager& sm = GetSceneManager();
    sm.CreateScene<TriangleDemo>();
    sm.CreateScene<ColorDemo>();
    sm.CreateScene<ConstantBufferDemo>();
    sm.CreateScene<CameraDemo>();
    sm.CreateScene<Texture2DDemo>();
    sm.CreateScene<LightDemo>();
    sm.CreateScene<ModelDemo>();
    sm.CreateScene<CubeMapDemo>();
    sm.CreateScene<ImageBasedLightningDemo>();

    sm.ChangeScene<ImageBasedLightningDemo>();
}

void DemoApp::OnShutdown()
{
}
