#include "pch.h"

#include "30. DemoScene.h"

#include "CameraScript.h"
#include "GUIs.h"

DemoScene::~DemoScene() = default;

void DemoScene::Init()
{
    auto&       r  = GetRenderer();
    const auto& sc = r.GetSwapChain();

    //===================================================
    // Values
    //===================================================

    {
        m_backBufferHDR         = sc->GetBackBufferHDR();
        m_backBufferDepthBuffer = sc->GetDepthBuffer();
        m_sceneHierarchy        = CreateScope<SceneHierarchy>();

        GeometryData skyboxData = GeometryFactory::CreateSphere(500.f, 32, 32);
        std::ranges::reverse(skyboxData.indices);
        std::filesystem::path daySkyboxPath   = "Resources\\AnimationDemo\\DaySkybox";
        std::filesystem::path nightSkyboxPath = "Resources\\AnimationDemo\\NightSkybox";

        m_daySkybox = SkyboxRenderer {
            .mesh        = CreateMesh(skyboxData),
            .envCubemap  = CreateTextureCubeFromFile(daySkyboxPath / "skyboxEnvHDR.dds"),
            .irrCubemap  = CreateTextureCubeFromFile(daySkyboxPath / "skyboxDiffuseHDR.dds"),
            .specCubemap = CreateTextureCubeFromFile(daySkyboxPath / "skyboxSpecularHDR.dds"),
            .brdfImage   = CreateTexture2DFromFile(daySkyboxPath / "skyboxBrdf.dds")
        };

        m_nightSkybox = SkyboxRenderer {
            .mesh        = CreateMesh(skyboxData),
            .envCubemap  = CreateTextureCubeFromFile(nightSkyboxPath / "skyboxEnvHDR.dds"),
            .irrCubemap  = CreateTextureCubeFromFile(nightSkyboxPath / "skyboxDiffuseHDR.dds"),
            .specCubemap = CreateTextureCubeFromFile(nightSkyboxPath / "skyboxSpecularHDR.dds"),
            .brdfImage   = CreateTexture2DFromFile(nightSkyboxPath / "skyboxBrdf.dds")
        };
    }

    //===================================================
    // Entity
    //===================================================

    {
        Entity e = CreateEntity("Skybox");
        e.CreateComponent<RenderGroup<"Skybox">>();
        e.CreateComponent<SkyboxRenderer>(m_skyboxType == eSkyboxType::Day ? m_daySkybox : m_nightSkybox);
    }

    {
        Entity e = CreateEntity("Camera");
        e.CreateComponent<CameraComponent>(
            CameraComponent {
                .projectionType = eProjectionType::Perspective,
                .nearZ          = 0.1f,
                .farZ           = 1000.0f,
                .fov            = 45.f * DEG2RAD,
                .aspect         = GetAppWindow().GetAspect() });

        e.CreateComponent<ScriptComponent>(CreateScope<CameraScript>(e));
        e.GetTransform().position.z = -5.f;
        m_camera                    = e;
    }

    for (uint32 i = 0; i < 100; i++)
    {
        Entity       e            = CreateEntity(fmt::format("Cube {0}", i));
        GeometryData cubeData     = GeometryFactory::CreateCube(1.f, 1.f, 1.f);
        MaterialData materialData = {
            .baseColor = Color4(Random::GenerateFloat(0.f, 1.f), Random::GenerateFloat(0.f, 1.f), Random::GenerateFloat(0.f, 1.f), 1.f),
            .metallic  = Random::GenerateFloat(0.f, 1.f),
            .roughness = Random::GenerateFloat(0.f, 1.f)
        };

        e.CreateComponent<ModelRenderer>(
            ModelRenderer {
                CreateModel(
                    CreateMesh(cubeData),
                    CreateMaterial(materialData)) });

        e.CreateComponent<RenderGroup<"PBR">>();

        e.GetTransform().position = Vec3(Random::GenerateFloat(-50.f, 50.f), Random::GenerateFloat(0.f, 10.f), Random::GenerateFloat(0.f, 100.f));
        e.GetTransform().AddRotateYaw(DEG2RAD * Random::GenerateFloat(0.f, 360.f));
        e.GetTransform().AddRotatePitch(DEG2RAD * Random::GenerateFloat(0.f, 360.f));
        e.GetTransform().AddRotateRoll(DEG2RAD * Random::GenerateFloat(0.f, 360.f));
    }

    {
        Entity       e          = CreateEntity("Sphere");
        GeometryData sphereData = GeometryFactory::CreateSphere(1.f, 32, 32);

        MaterialData materialData = {
            .baseColor = color4::RED,
            .metallic  = 0.5f,
            .roughness = 0.25f
        };

        e.CreateComponent<ModelRenderer>(
            ModelRenderer {
                CreateModel(
                    CreateMesh(sphereData),
                    CreateMaterial(materialData)) });

        e.CreateComponent<RenderGroup<"PBR">>();
        e.GetTransform().position.x = 2.f;
    }

    {
        Entity e = CreateEntity("Model");

        ModelLoader loader;
        loader.Load(R"(C:\Users\Ahnjiwoo\Desktop\Projects\CrabRenderer\DemoProgram\Resources\AnimationDemo\Model\source\Kokona.obj)");

        e.CreateComponent<ModelRenderer>(
            ModelRenderer { loader.CreateModel<Vertex3D>(Vertex3D::CreateVertex) });

        e.CreateComponent<RenderGroup<"PBR">>();

        e.GetTransform().position.z = 5.f;
    }

    {
        Entity e = CreateEntity("Light");
        e.CreateComponent<LightComponent>(
            LightComponent {
                .lightRadiance  = color3::WHITE,
                .fallOffStart   = 5.f,
                .fallOffEnd     = 100.f,
                .lightStrength  = 1.f,
                .innerConeAngle = 30.f * DEG2RAD,
                .outerConeAngle = 45.f * DEG2RAD,
                .lightType      = eLightType::Directional,
                .useShadow      = true });

        e.GetTransform().position.y = 5.f;
        e.GetTransform().SetRotatePitch(45.f * DEG2RAD);
    }

    {
        Entity       e         = CreateEntity("Plane");
        GeometryData planeData = GeometryFactory::CreateGrid(250.f, 250.f, 25, 25);

        std::filesystem::path path = "Resources\\AnimationDemo\\floor2";
        MaterialData          materialData;
        materialData.baseColorTex  = CreateTexture2DFromFile(path / "floor_tiles_06_diff_2k.jpg", true, true);
        materialData.normalTex     = CreateTexture2DFromFile(path / "floor_tiles_06_nor_gl_2k.exr", true);
        materialData.roughnessTex  = CreateTexture2DFromFile(path / "floor_tiles_06_rough_2k.jpg", true);
        materialData.normalMapType = eNormalMapType::OpenGL;
        materialData.metallic      = 0.5f;
        materialData.roughness     = 0.5f;
        materialData.alpha         = 0.98f;

        e.CreateComponent<ModelRenderer>(
            ModelRenderer {
                CreateModel(
                    CreateMesh(planeData),
                    CreateMaterial(materialData)) });

        e.CreateComponent<RenderGroup<"Mirror">>();
        e.GetTransform().position.y = -1.5f;
        e.GetTransform().SetRotatePitch(-90.f * DEG2RAD);
    }
}

void DemoScene::OnEnter()
{
}

void DemoScene::OnExit()
{
}

void DemoScene::OnUpdate(TimeStamp& in_ts)
{
    if (Input::IsKeyPressed(eKey::Escape))
        GetApplication().Quit();
}

void DemoScene::OnRender(TimeStamp& in_ts)
{
    auto [screenWidth, screenHeight] = GetAppWindow().GetResolution();
    auto&       r                    = GetRenderer();
    const auto* pipe                 = GetGlobalPipeline();

    //===================================================
    // Update LightData
    //===================================================

    CrabPass::LightShadowPass<"PBR">(this, m_camera);

    //===================================================
    // Begin Render
    //===================================================

    {
        m_backBufferHDR->Bind(m_backBufferDepthBuffer);
        m_backBufferHDR->Clear(color4::BLACK);
        m_backBufferDepthBuffer->Clear(true, 1.f, false, 0);
        r.SetViewport(0, 0, screenWidth, screenHeight);

        // Update Camera
        auto& cmr = m_camera.GetComponent<CameraComponent>();

        GetGlobalConstants()->UpdateCamera(
            CameraConstant {
                .view           = cmr.GetView(m_camera.GetTransform()),
                .viewProj       = cmr.GetViewProj(m_camera.GetTransform()),
                .invViewProj    = cmr.GetViewProj(m_camera.GetTransform()).Invert(),
                .cameraPosition = m_camera.GetTransform().position,
            });
    }

    //===================================================
    // Main Render
    //===================================================

    CrabPass::SkyboxPass<"Skybox">(this);
    CrabPass::PBRPass<"PBR">(this, {}, m_wireframeMode ? pipe->GetPBRWireframe() : nullptr);

    if (m_drawNormal)
    {
        CrabPass::DrawNormalPass<"PBR">(this);
    }

    //===================================================
    // Mirror Reflection
    //===================================================

    CrabPass::MirrorPass<"Mirror">(
        this,
        m_backBufferDepthBuffer,
        m_camera,
        1,
        [&](Scene* in_scene)
        {
            CrabPass::SkyboxPass<"Skybox">(this, PipelineBindArgument { 1 }, pipe->GetSkyboxReflection());
            CrabPass::PBRPass<"PBR">(this, PipelineBindArgument { 1 }, m_wireframeMode ? pipe->GetPBRRefractionWireframe() : pipe->GetPBRReflection());
            CrabPass::PBRPass<"Mirror">(this, PipelineBindArgument { 1 }, m_wireframeMode ? pipe->GetPBRWireframeOnMask() : pipe->GetPBROnMask());
        });
}

void DemoScene::OnPostRender(TimeStamp& in_ts)
{
    if (m_postProcessDirty)
    {
        //===================================================
        // Post Process
        //===================================================

        {
            auto [screenWidth, screenHeight] = GetAppWindow().GetResolution();
            const auto& sc                   = GetRenderer().GetSwapChain();

            m_postProcess.ClearFilterList();

            m_postProcess.AddFilter(
                ImageFilterFactory::CreateSampling(
                    screenWidth,
                    screenHeight,
                    sc->GetResolvedBackBufferHDRTexture()));

            if (m_useBloom)
            {
                constexpr uint32 BLUR_COUNT = 3;

                for (uint32 i = 0; i < BLUR_COUNT; ++i)
                {
                    uint32 width  = screenWidth >> (i + 1);
                    uint32 height = screenHeight >> (i + 1);

                    m_postProcess.AddFilter(
                        ImageFilterFactory::CreateBlurDown(
                            width,
                            height,
                            m_postProcess.GetLastFilter()->GetOutputTexture()));
                }

                for (uint32 i = 0; i < BLUR_COUNT; ++i)
                {
                    uint32 width  = screenWidth >> (BLUR_COUNT - i);
                    uint32 height = screenHeight >> (BLUR_COUNT - i);

                    m_postProcess.AddFilter(
                        ImageFilterFactory::CreateBlurUp(
                            width,
                            height,
                            m_postProcess.GetLastFilter()->GetOutputTexture(),
                            BlurUpConstant { m_postprocessBlurRadius }));
                }

                m_postProcess.AddFilter(
                    ImageFilterFactory::CreateCombine(
                        screenWidth,
                        screenHeight,
                        m_postProcess.GetLastFilter()->GetOutputTexture(),
                        sc->GetResolvedBackBufferHDRTexture(),
                        CombineConstant { m_postprocessCombineFactor },
                        &m_postprocessCombineConst));
            }

            Ref<ImageFilter> toneMap = ImageFilterFactory::CreateToneMapping(
                screenWidth,
                screenHeight,
                m_postProcess.GetLastFilter()->GetOutputTexture(),
                ToneMappingConstant { .exposure = 1.f, .gamma = 2.2f });

            toneMap->SetRenderTarget(sc->GetBackBuffer());

            m_postProcess.AddFilter(toneMap);
        }
        m_postProcessDirty = false;
    }

    GetRenderer().GetSwapChain()->ResolveBackBufferHDR();
    m_postProcess.Render();
}

void DemoScene::OnRenderGUI(TimeStamp& in_ts)
{
    DrawDemoInspector(GetName(),
                      [&]()
                      {
                          ImGui::Text("Hello, World!");

                          ImGui::Checkbox("Wireframe Mode", &m_wireframeMode);
                          ImGui::Checkbox("Draw Normal", &m_drawNormal);

                          ImGui::SeparatorText("Post Process Bloom");

                          if (ImGui::Checkbox("Use Bloom", &m_useBloom))
                              m_postProcessDirty = true;

                          if (ImGui::SliderFloat("Bloom Combine Factor", &m_postprocessCombineFactor, 0.f, 1.f))
                              m_postprocessCombineConst->WriteToBuffer(CombineConstant { m_postprocessCombineFactor });

                          if (ImGui::DragFloat("Bloom Radius", &m_postprocessBlurRadius, 0.0001f, 0.0001f, 1.f))
                              m_postProcessDirty = true;

                          ImGui::Separator();
                          if (ImGui::RadioButton("Day", reinterpret_cast<int*>(&m_skyboxType), 0))
                              FindEntityByTag("Skybox").ReplaceComponent<SkyboxRenderer>(m_daySkybox);

                          ImGui::SameLine();

                          if (ImGui::RadioButton("Night", reinterpret_cast<int*>(&m_skyboxType), 1))
                              FindEntityByTag("Skybox").ReplaceComponent<SkyboxRenderer>(m_nightSkybox);
                      });

    m_sceneHierarchy->DrawSceneHierarchy(this);
}

void DemoScene::OnEvent(CrabEvent& in_event)
{
}