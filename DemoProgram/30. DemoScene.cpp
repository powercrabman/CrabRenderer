#include "pch.h"

#include "30. DemoScene.h"

#include "CameraScript.h"
#include "GUIs.h"

DemoScene::~DemoScene() = default;

void DemoScene::Init()
{
    Super::Init();

    auto&       r  = GetRenderer();
    const auto& sc = r.GetSwapChain();

    //===================================================
    // Values
    //===================================================

    {
        m_sceneHierarchy = CreateScope<SceneHierarchy>();

        GeometryData skyboxData = GeometryFactory::CreateSphere(500.f, 32, 32);
        std::ranges::reverse(skyboxData.indices);
        std::filesystem::path daySkyboxPath   = "Resources\\AnimationDemo\\DaySkybox";
        std::filesystem::path nightSkyboxPath = "Resources\\AnimationDemo\\NightSkybox";

        m_daySkybox = SkyboxRenderer {
            .mesh        = RenderFactory::CreateMesh(skyboxData),
            .envCubemap  = RenderFactory::CreateTextureCubeFromFile(daySkyboxPath / "skyboxEnvHDR.dds"),
            .irrCubemap  = RenderFactory::CreateTextureCubeFromFile(daySkyboxPath / "skyboxDiffuseHDR.dds"),
            .specCubemap = RenderFactory::CreateTextureCubeFromFile(daySkyboxPath / "skyboxSpecularHDR.dds"),
            .brdfImage   = RenderFactory::CreateTexture2DFromFile(daySkyboxPath / "skyboxBrdf.dds")
        };

        m_nightSkybox = SkyboxRenderer {
            .mesh        = RenderFactory::CreateMesh(skyboxData),
            .envCubemap  = RenderFactory::CreateTextureCubeFromFile(nightSkyboxPath / "skyboxEnvHDR.dds"),
            .irrCubemap  = RenderFactory::CreateTextureCubeFromFile(nightSkyboxPath / "skyboxDiffuseHDR.dds"),
            .specCubemap = RenderFactory::CreateTextureCubeFromFile(nightSkyboxPath / "skyboxSpecularHDR.dds"),
            .brdfImage   = RenderFactory::CreateTexture2DFromFile(nightSkyboxPath / "skyboxBrdf.dds")
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
        GetCameraEntity().CreateComponent<ScriptComponent>(CreateScope<CameraScript>(GetCameraEntity()));
        GetCameraTransform().position.z = -5.f;
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
                RenderFactory::CreateModel(
                    RenderFactory::CreateMesh(cubeData),
                    RenderFactory::CreateMaterial(materialData)) });

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
                RenderFactory::CreateModel(
                    RenderFactory::CreateMesh(sphereData),
                    RenderFactory::CreateMaterial(materialData)) });

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
        materialData.baseColorTex  = RenderFactory::CreateTexture2DFromFile(path / "floor_tiles_06_diff_2k.jpg", eTextureLoadFlags_ReverseTone);
        materialData.normalTex     = RenderFactory::CreateTexture2DFromFile(path / "floor_tiles_06_nor_gl_2k.exr");
        materialData.roughnessTex  = RenderFactory::CreateTexture2DFromFile(path / "floor_tiles_06_rough_2k.jpg");
        materialData.normalMapType = eNormalMapType::OpenGL;
        materialData.metallic      = 0.5f;
        materialData.roughness     = 0.5f;
        materialData.alpha         = 0.98f;

        e.CreateComponent<ModelRenderer>(
            ModelRenderer {
                RenderFactory::CreateModel(
                    RenderFactory::CreateMesh(planeData),
                    RenderFactory::CreateMaterial(materialData)) });

        e.CreateComponent<RenderGroup<"Mirror">>();
        e.GetTransform().position.y = -1.5f;
        e.GetTransform().SetRotatePitch(-90.f * DEG2RAD);
    }
}

void DemoScene::OnEnter()
{
    Super::OnEnter();
}

void DemoScene::OnExit()
{
    Super::OnExit();
}

void DemoScene::OnUpdate(TimeStamp& in_ts)
{
    Super::OnUpdate(in_ts);
}

void DemoScene::OnRender(TimeStamp& in_ts)
{
    Super::OnRender(in_ts);
}

void DemoScene::OnPostRender(TimeStamp& in_ts)
{
    Super::OnPostRender(in_ts);
}

void DemoScene::OnRenderGUI(TimeStamp& in_ts)
{
    DrawDemoInspector(GetName(),
                      [&]()
                      {
                          ImGui::Text("Hello, World!");

                          bool  wireframeMode = IsWireframeModeEnabled();
                          bool  drawNormal    = IsDrawNormalEnabled();
                          bool  useBloom      = IsPostProcessBloomEnabled();
                          float combineFactor = GetBloomCombineFactor();
                          float bloomRadius   = GetBloomBlurRadius();

                          if (ImGui::Checkbox("Wireframe Mode", &wireframeMode))
                              EnableWireframeMode(wireframeMode);

                          if (ImGui::Checkbox("Draw Normal", &drawNormal))
                              EnableDrawNormal(drawNormal);

                          ImGui::SeparatorText("Post Process Bloom");

                          if (ImGui::Checkbox("Use Bloom", &useBloom))
                              EnablePostProcessBloom(useBloom);

                          if (ImGui::SliderFloat("Bloom Combine Factor", &combineFactor, 0.f, 1.f))
                              SetBloomCombineFactor(combineFactor);

                          if (ImGui::DragFloat("Bloom Radius", &bloomRadius, 0.0001f, 0.0001f, 1.f))
                              SetBloomBlurRadius(bloomRadius);

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