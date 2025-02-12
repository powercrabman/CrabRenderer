#include "pch.h"

#include "DemoApp.h"

// #include "1. TriangleDemo.h"
// #include "2. ColorDemo.h"
// #include "3. ConstantBufferDemo.h"
// #include "4. CameraDemo.h"
// #include "5. Texture2DDemo.h"
// #include "6. LightDemo.h"
// #include "7. ModelDemo.h"
// #include "8. CubeMapDemo.h"
// #include "9. ImageBasedLightningDemo.h"
// #include "10. PostProcessingDemo.h"
// #include "11. PostProcessingDemo2.h"
// #include "12. MousePickingDemo.h"
// #include "13. TrackballDemo.h"
// #include "14. BasicGSDemo.h"
// #include "15. TessellationSubdivision.h"
// #include "16. MipmapDemo.h"
// #include "17. NormalMapDemo.h"
//#include "18. HDRIDemo.h"
#include "19. PBRDemo.h"

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

    setting.rendererSetting.swapChainSetting.enableDepthBuffer       = true;
    setting.rendererSetting.swapChainSetting.enableVSync             = true;
    setting.rendererSetting.swapChainSetting.enableFloatRenderTarget = true;
    setting.rendererSetting.swapChainSetting.enableMSAA              = true;
    setting.rendererSetting.swapChainSetting.MSAAQuality             = MSAA_MAX_QUALITY;
    setting.rendererSetting.swapChainSetting.MSAASampleCount         = 4;

    return setting;
}

void DemoApp::OnInit()
{
    SceneManager& sm = GetSceneManager();
    // sm.CreateScene<TriangleDemo>();
    // sm.CreateScene<ColorDemo>();
    // sm.CreateScene<ConstantBufferDemo>();
    // sm.CreateScene<CameraDemo>();
    // sm.CreateScene<Texture2DDemo>();
    // sm.CreateScene<LightDemo>();
    // sm.CreateScene<ModelDemo>();
    // sm.CreateScene<CubeMapDemo>();
    // sm.CreateScene<ImageBasedLightningDemo>();
    // sm.CreateScene<PostProcessingDemo>();
    // sm.CreateScene<PostProcessingDemo2>();
    // sm.CreateScene<MousePickingDemo>();
    // sm.CreateScene<TrackballDemo>();
    // sm.CreateScene<BasicGSDemo>();
    // sm.CreateScene<TessellationSubdivision>();
    // sm.CreateScene<MipmapDemo>();
    // sm.CreateScene<NormalMapDemo>();
    //sm.CreateScene<HDRIDemo>();
    sm.CreateScene<PBRDemo>();

    sm.ChangeScene<PBRDemo>();
}

void DemoApp::OnShutdown()
{
}
