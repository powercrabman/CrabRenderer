#include "pch.h"

#include "28. AnimationDemo.h"

#include "CameraScript.h"
#include "GUIs.h"
#include "ImageFilterFactory.h"
#include "RenderUtil.h"
#include "Tags.h"

#include <entt/core/hashed_string.hpp>

AnimationDemo::~AnimationDemo()
{
}

void AnimationDemo::Init()
{
    // Skybox - day
    {
        auto  e  = CreateEntity("Skybox");
        auto& sr = e.GetOrCreateComponent<SkyboxRenderer>();

        auto geo = GeometryFactory::CreateSphere(1000.f, 32.f, 32.f);
        std::ranges::reverse(geo.indices);
        sr.mesh = Mesh::Create(
            Vertex3D::CreateVertices(geo),
            geo.indices,
            eTopology::TriangleList);

        std::filesystem::path skyboxPath = "Resources\\AnimationDemo\\DaySkybox";

        sr.brdfImage   = Texture2D::CreateFromFile(skyboxPath / "skyboxBrdf.dds", false);
        sr.envCubemap  = TextureCube::CreateFromFile(skyboxPath / "skyboxEnvHDR.dds");
        sr.irrCubemap  = TextureCube::CreateFromFile(skyboxPath / "skyboxDiffuseHDR.dds");
        sr.specCubemap = TextureCube::CreateFromFile(skyboxPath / "skyboxSpecularHDR.dds");

        e.CreateComponent<TAG("SkyboxGroup")>();

        m_skyboxDayEntity = e;
    }

    // Skybox - night
    {
        auto e = CloneEntity(m_skyboxDayEntity);

        auto& sr                              = e.GetComponent<SkyboxRenderer>();
        e.GetComponent<LabelComponent>().name = "Skybox Night";

        std::filesystem::path skyboxPath = "Resources\\AnimationDemo\\NightSkybox";

        sr.brdfImage   = Texture2D::CreateFromFile(skyboxPath / "SkyboxBrdf.dds", false);
        sr.envCubemap  = TextureCube::CreateFromFile(skyboxPath / "SkyboxEnvHDR.dds");
        sr.irrCubemap  = TextureCube::CreateFromFile(skyboxPath / "SkyboxDiffuseHDR.dds");
        sr.specCubemap = TextureCube::CreateFromFile(skyboxPath / "SkyboxSpecularHDR.dds");

        m_skyboxNightEntity = e;
    }

    // Camera
    {
        auto  e      = CreateEntity("Camera");
        auto& cc     = e.GetOrCreateComponent<CameraControlComponent>();
        cc.moveSpeed = 3.f;
        cc.rotSpeed  = 15.f * DEG2RAD;

        auto& cam          = e.GetOrCreateComponent<CameraComponent>();
        cam.projectionType = eProjectionType::Perspective;
        cam.aspect         = GetAppWindow().GetAspect();
        cam.farZ           = 5000.f;
        cam.nearZ          = 0.1f;
        cam.fov            = 45.f * DEG2RAD;

        auto& t    = e.GetTransform();
        t.position = Vec3 { 0.f, 0.f, -5.f };

        e.CreateComponent<ScriptComponent>(CreateScope<CameraScript>(e));

        m_cameraEntity = e;
    }

    // Floor (Mirror)
    {
        auto  e      = CreateEntity("Floor");
        auto& mirror = e.GetOrCreateComponent<PlanarMirrorComponent>();
        auto  geo    = GeometryFactory::CreateQuad(100.f, 100.f);
        geo.texCoords.clear();
        geo.texCoords.resize(24, {});
        for (uint32 i = 0; i < 6; i++)
        {
            geo.texCoords[i * 4 + 0] = Vec2 { 0.f, 0.f };
            geo.texCoords[i * 4 + 1] = Vec2 { 30.f, 0.f };
            geo.texCoords[i * 4 + 2] = Vec2 { 30.f, 30.f };
            geo.texCoords[i * 4 + 3] = Vec2 { 0.f, 30.f };
        }

        MaterialData matData = {};
        matData.baseColor    = color3::WHITE;
        matData.metallic     = 1.f;
        matData.roughness    = 1.f;

        std::filesystem::path floorPath = "Resources\\AnimationDemo\\floor";
        matData.baseColorImage          = Texture2D::CreateFromFile(floorPath / "angled-tiled-floor_albedo.png", true, true);
        matData.aoImage                 = Texture2D::CreateFromFile(floorPath / "angled-tiled-floor_ao.png");
        matData.metallicImage           = Texture2D::CreateFromFile(floorPath / "angled-tiled-floor_metallic.png");
        matData.normalImage             = Texture2D::CreateFromFile(floorPath / "angled-tiled-floor_normal-dx.png");
        matData.roughnessImage          = Texture2D::CreateFromFile(floorPath / "angled-tiled-floor_roughness.png");
        matData.normalMapType           = eNormalMapType::DirectX;
        matData.alpha                   = 0.8f;

        mirror.mirrorMesh = Mesh::Create(
            Vertex3D::CreateVertices(geo),
            geo.indices,
            eTopology::TriangleList);

        mirror.mirrorMaterial = Material::Create(matData);

        auto& t    = e.GetTransform();
        t.position = Vec3 { 0.f, -1.f, 0.f };
        t.SetRotateYaw(180.f * DEG2RAD);
        t.SetRotatePitch(-90.f * DEG2RAD);
        e.CreateComponent<TAG("MirrorGroup")>();
    }

    // Cube
    {
        auto  e     = CreateEntity("Cube");
        auto& model = e.GetOrCreateComponent<ModelRenderer>();
        auto  geo   = GeometryFactory::CreateCube(1.f, 1.f, 1.f);

        MaterialData matData = {};
        matData.metallic     = 0.f;
        matData.roughness    = 1.f;
        matData.baseColor    = Vec3 { 0.9f, 0.2f, 0.2f };
        //
        //  std::filesystem::path cubePath = "Resources\\AnimationDemo\\Leather";
        //  matData.baseColorImage         = Texture2D::CreateFromFile(cubePath / "Leather037_2K-JPG_Color.jpg", true, true);
        //  matData.roughnessImage         = Texture2D::CreateFromFile(cubePath / "Leather037_2K-JPG_Roughness.jpg");
        //  matData.normalImage            = Texture2D::CreateFromFile(cubePath / "Leather037_2K-JPG_NormalGL.jpg");
        //
        model.model = Model::Create({ { RenderUtil::CreateMesh(geo), Material::Create(matData) } });

        auto& t    = e.GetTransform();
        t.position = Vec3 { -2.f, 0.f, 3.f };

        e.CreateComponent<TAG("PBRGroup")>();
    }

    // Sphere
    {
        auto  e     = CreateEntity("Sphere");
        auto& model = e.GetOrCreateComponent<ModelRenderer>();
        auto  geo   = GeometryFactory::CreateSphere(0.5f, 32, 32);

        MaterialData matData = {};
        matData.baseColor    = Vec3 { 0.06f, 0.06f, 0.06f };
        matData.metallic     = 1.f;
        matData.roughness    = 0.f;
        //
        // std::filesystem::path spherePath = "Resources\\AnimationDemo\\MetalBall";
        // matData.baseColorImage           = Texture2D::CreateFromFile(spherePath / "Metal049A_2K-JPG_Color.jpg", true, true);
        // matData.metallicImage            = Texture2D::CreateFromFile(spherePath / "Metal049A_2K-JPG_Metalness.jpg");
        // matData.normalImage              = Texture2D::CreateFromFile(spherePath / "Metal049A_2K-JPG_NormalGL.jpg");
        // matData.roughnessImage           = Texture2D::CreateFromFile(spherePath / "Metal049A_2K-JPG_Roughness.jpg");
        //
        model.model = Model::Create({ { RenderUtil::CreateMesh(geo), Material::Create(matData) } });

        auto& t    = e.GetTransform();
        t.position = Vec3 { 0.f, 0.f, 3.f };

        e.CreateComponent<TAG("PBRGroup")>();
    }

    // model
    //{
    //    auto  e   = CreateEntity("Model");
    //    auto& msh = e.GetOrCreateComponent<ModelRenderer>();
    //
    //    msh.model = CreateModel(R"(Resources\AnimationDemo\Model\source\Kokona.obj)");
    //
    //    auto& t    = e.GetTransform();
    //    t.position = Vec3 { 2.f, 0.f, 3.f };
    //    t.scale    = Vec3 { 3.f, 3.f, 3.f };
    //
    //    e.CreateComponent<TAG("PBRGroup")>();
    //
    //    std::filesystem::path path  = R"(Resources\AnimationDemo\Model\textures)";
    //    auto&                 model = msh.model;
    //
    //    auto LoadFunc = [](Ref<Model>& model, const std::filesystem::path& path, std::string_view in_string)
    //    {
    //        auto* node = model->FindNode(in_string);
    //        node->material->SetBaseColorImage(Texture2D::CreateFromFile(path / (std::string(in_string) + ".png"), true, true));
    //        node->material->SetMetallic(0.f);
    //    };
    //
    //    LoadFunc(model, path, "Eyes");
    //    LoadFunc(model, path, "HALO");
    //    LoadFunc(model, path, "Hair");
    //    LoadFunc(model, path, "Head");
    //    LoadFunc(model, path, "Hoodie");
    //    LoadFunc(model, path, "Shoes");
    //    LoadFunc(model, path, "Cloth");
    //    LoadFunc(model, path, "Panties");
    //    LoadFunc(model, path, "Skin");
    //    LoadFunc(model, path, "exp");
    //}

    // Light (Directional Light)
    {
        auto e = CreateEntity("Light");

        auto& lc         = e.GetOrCreateComponent<LightComponent>();
        lc.fallOffEnd    = 100.f;
        lc.fallOffStart  = 5.f;
        lc.lightRadiance = color3::WHITE;
        lc.lightStrength = 1.f;
        lc.lightType     = eLightType::Point;

        lc.shadowMap = DepthMap::CreateDepthMapCube(1024, 1024);
        lc.useShadow = true;

        e.GetTransform().position = Vec3 { 0.f, 5.f, 0.f };
    }

    // Resources
    {
        m_sceneHierarchy = CreateScope<SceneHierarchy>();
        m_depthMap       = DepthMap::Create(1024, 1024);
    }
}

void AnimationDemo::OnEnter()
{
}

void AnimationDemo::OnExit()
{
}

void AnimationDemo::OnUpdate(TimeStamp& in_ts)
{
    if (Input::IsKeyPressed(eKey::Escape))
        GetApplication().Quit();
}

void AnimationDemo::_DrawSkybox(bool in_bindTexture)
{
    // this is common skybox
    auto DrawCall = [](SkyboxRenderer& s, TransformComponent& t, bool in_bineTexture)
    {
        SkyboxPSConstant c = {};
        c.textureCubeType  = s.mappingType;
        GetGlobalConstants().UpdateSkyboxPS(c);

        TransformConstant tc = {};
        tc.world             = t.GetWorld();
        tc.worldInvTranspose = tc.world.Invert().Transpose();
        GetGlobalConstants().UpdateTransform(tc);

        if (in_bineTexture)
        {
            if (s.envCubemap)
                s.envCubemap->Bind(10, eShaderFlags_PixelShader);

            if (s.irrCubemap)
                s.irrCubemap->Bind(11, eShaderFlags_PixelShader);

            if (s.specCubemap)
                s.specCubemap->Bind(12, eShaderFlags_PixelShader);

            if (s.brdfImage)
                s.brdfImage->Bind(13, eShaderFlags_PixelShader);
        }

        s.mesh->Draw();
    };

    if (m_skyboxType == eSkyboxType::Day)
    {
        DrawCall(m_skyboxDayEntity.GetComponent<SkyboxRenderer>(),
                 m_skyboxDayEntity.GetTransform(),
                 in_bindTexture);
    }
    else
    {
        DrawCall(m_skyboxNightEntity.GetComponent<SkyboxRenderer>(),
                 m_skyboxNightEntity.GetTransform(),
                 in_bindTexture);
    }
}

void AnimationDemo::_DrawBasic(bool in_bindTexture)
{
    GetView<TransformComponent, ModelRenderer, TAG("PBRGroup")>().each(
        [&](const TransformComponent& t, const ModelRenderer& m)
        {
            for (const auto& node: m.model->GetNodes())
            {
                _DrawPBRMesh(t, node.mesh, in_bindTexture ? node.material : nullptr);
            }
        });
}

void AnimationDemo::_DrawMirror(bool in_bindTexture)
{
    GetView<TransformComponent, PlanarMirrorComponent, TAG("MirrorGroup")>().each(
        [&](const TransformComponent& t, const PlanarMirrorComponent& m)
        {
            TransformConstant tc = {};
            tc.world             = t.GetWorld();
            tc.worldInvTranspose = tc.world.Invert().Transpose();
            GetGlobalConstants().UpdateTransform(tc);

            _DrawPBRMesh(t, m.mirrorMesh, in_bindTexture ? m.mirrorMaterial : nullptr);
        });
}

void AnimationDemo::_DrawPBRMesh(
    const TransformComponent& t,
    const Ref<Mesh>&          mesh,
    const Ref<Material>&      mat)
{
    CRAB_ASSERT(mesh, "Mesh is nullptr");

    TransformConstant tc = {};
    tc.world             = t.GetWorld();
    tc.worldInvTranspose = tc.world.Invert().Transpose();
    GetGlobalConstants().UpdateTransform(tc);

    if (mat)
    {
        MaterialConstant mc     = {};
        mc.baseColor            = mat->GetBaseColor();
        mc.metallic             = mat->GetMetallic();
        mc.emissive             = mat->GetEmissive();
        mc.roughness            = mat->GetRoughness();
        mc.diffuse              = mat->GetDiffuse();
        mc.displacementStrength = mat->GetDisplacementStrength();
        mc.specular             = mat->GetSpecular();
        mc.sharpness            = mat->GetSharpness();
        mc.ambient              = mat->GetAmbient();
        mc.alpha                = mat->GetAlpha();

        const MaterialData& md = mat->GetMaterialData();

        mc.usingTextureFlags = eMaterialTextureUsingFlags_None;

        if (md.baseColorImage)
        {
            md.baseColorImage->Bind(0, eShaderFlags_PixelShader);
            mc.usingTextureFlags |= eMaterialTextureUsingFlags_BaseColor;
        }

        if (md.normalImage)
        {
            md.normalImage->Bind(1, eShaderFlags_PixelShader);

            if (md.normalMapType == eNormalMapType::DirectX)
                mc.usingTextureFlags |= eMaterialTextureUsingFlags_Normal_DX;
            else
                mc.usingTextureFlags |= eMaterialTextureUsingFlags_Normal_GL;
        }

        if (md.metallicImage)
        {
            md.metallicImage->Bind(3, eShaderFlags_PixelShader);
            mc.usingTextureFlags |= eMaterialTextureUsingFlags_Metallic;
        }

        if (md.roughnessImage)
        {
            md.roughnessImage->Bind(4, eShaderFlags_PixelShader);
            mc.usingTextureFlags |= eMaterialTextureUsingFlags_Roughness;
        }
        if (md.aoImage)
        {
            md.aoImage->Bind(2, eShaderFlags_PixelShader);
            mc.usingTextureFlags |= eMaterialTextureUsingFlags_AO;
        }
        if (md.emissiveImage)
        {
            md.emissiveImage->Bind(5, eShaderFlags_PixelShader);
            mc.usingTextureFlags |= eMaterialTextureUsingFlags_Emissive;
        }
        if (md.displacementImage)
        {
            md.displacementImage->Bind(0, eShaderFlags_VertexShader);
            mc.usingTextureFlags |= eMaterialTextureUsingFlags_Displacement;
        }

        GetGlobalConstants().UpdateMaterial(mc);
    }

    mesh->Draw();
}

void AnimationDemo::OnRender(TimeStamp& in_ts)
{
    auto& r = GetRenderer();

    //===================================================
    // Depth Only Path
    //===================================================

    m_depthMap->BindDepthBuffer();
    m_depthMap->Clear(1.f);
    GetGlobalRenderPass().BeginDepthOnlyPass();
    {
        CameraComponent& cmr = m_cameraEntity.GetComponent<CameraComponent>();

        CameraConstant cc;
        cc.eyePosition = m_cameraEntity.GetTransform().position;
        cc.viewProj    = cmr.GetViewProj(cc.eyePosition, m_cameraEntity.GetTransform().rotate);
        cc.viewProjInv = cc.viewProj.Invert();

        auto [width, height] = m_depthMap->GetResolution();

        Viewport vp = {};
        vp.x = vp.y = 0;
        vp.width    = width;
        vp.height   = height;
        r.SetViewport(vp);

        GetGlobalConstants().UpdateCamera(cc);
    }

    _DrawSkybox(false);
    _DrawBasic(false);
    _DrawMirror(false);

    //===================================================
    // Pass Light
    //===================================================

    {
        uint32 index = 0;

        LightTransformConstant ltc = {};
        ZeroMemory(&ltc, sizeof(ltc));

        LightAttributeConstant lac = {};
        ZeroMemory(&lac, sizeof(lac));

        GetView<TransformComponent, LightComponent>().each(
            [&](const TransformComponent& t, const LightComponent& l)
            {
                // Lighting
                LightTransform lt = {};
                lt.lightDirection = t.Forward();
                lt.lightPosition  = t.position;

                LightAttribute la = {};
                la.fallOffEnd     = l.fallOffEnd;
                la.fallOffStart   = l.fallOffStart;
                la.lightType      = l.lightType;
                la.lightRadiance  = l.lightRadiance;
                la.lightStrength  = l.lightStrength;
                la.innerConeAngle = l.innerConeAngle;
                la.outerConeAngle = l.outerConeAngle;

                // Shadow
                if (l.useShadow)
                {
                    la.useShadow        = true;
                    la.shadowBias       = l.shadowBias;
                    la.shadowKernelSize = l.shadowKernelSize;
                    lt.lightViewProj    = CreateView(t.position, t.rotate);

                    auto& shadowMap = l.shadowMap;

                    switch (l.lightType)
                    {
                        case eLightType::Directional:
                        {
                            lt.lightViewProj *= CreateOrthographic(
                                10.f * GetAppWindow().GetAspect(),
                                10.f,
                                0.1f,
                                100.f);

                            lt.lightPosition = m_cameraEntity.GetTransform().position - lt.lightDirection * 100.f;
                        }
                        break;

                        case eLightType::Spot:
                        {
                            lt.lightViewProj *= CreatePerspective(
                                l.outerConeAngle,
                                GetAppWindow().GetAspect(),
                                0.1f,
                                l.fallOffEnd);
                        }
                        break;

                        case eLightType::Point:
                        {
                            r.SetShaderResourceView(nullptr, OMNI_SHADOW_SLOT + index, eShaderFlags_PixelShader);
                            shadowMap->BindDepthBuffer();
                            shadowMap->Clear(1.f);

                            GetGlobalRenderPass().BeginOmniShadowCasterPass();

                            OmniShadowConstant osc  = {};
                            Mat4               proj = CreatePerspective(90.f * DEG2RAD, 1.f, 0.1f, l.fallOffEnd);
                            osc.shadowViewProj[0]   = CreateViewFromLookVector(t.position, Vec3 { 1.f, 0.f, 0.f }) * proj;
                            osc.shadowViewProj[1]   = CreateViewFromLookVector(t.position, Vec3 { -1.f, 0.f, 0.f }) * proj;
                            osc.shadowViewProj[2]   = CreateViewFromLookVector(t.position, Vec3 { 0.f, 1.f, 0.f }) * proj;
                            osc.shadowViewProj[3]   = CreateViewFromLookVector(t.position, Vec3 { 0.f, -1.f, 0.f }) * proj;
                            osc.shadowViewProj[4]   = CreateViewFromLookVector(t.position, Vec3 { 0.f, 0.f, 1.f }) * proj;
                            osc.shadowViewProj[5]   = CreateViewFromLookVector(t.position, Vec3 { 0.f, 0.f, -1.f }) * proj;

                            GetGlobalConstants().UpdateOmniShadow(osc);

                            auto [width, height] = shadowMap->GetResolution();
                            r.SetViewport(0, 0, width, height);

                            _DrawBasic(false);
                            _DrawMirror(false);

                            r.BindOnlyDepthStencilView(nullptr);
                            shadowMap->BindDepthMapTexture(OMNI_SHADOW_SLOT + index, eShaderFlags_PixelShader);
                        }
                        break;

                        default:
                        {
                            CRAB_DEBUG_BREAK_V(
                                "Not support light type. {0}",
                                magic_enum::enum_name(l.lightType));
                        }
                        break;
                    };
                }

                ltc.lightTransform[index] = lt;
                lac.lightAttribute[index] = la;

                ++index;
            });

        GetGlobalConstants().UpdateLightTransform(ltc);
        GetGlobalConstants().UpdateLightAttribute(lac);
    }

    //===================================================
    // Basic Pass
    //===================================================

    // pre render
    {
        auto [width, height] = GetAppWindow().GetWindowSize();

        r.BindBackBufferMS(r.GetDepthBuffer());
        r.ClearDepthBuffer(true, 1.f, true, 0);
        r.ClearBackBufferMS(color4::BLACK);
        r.SetViewport(0, 0, width, height);

        // update camera
        auto&          cmr = m_cameraEntity.GetComponent<CameraComponent>();
        CameraConstant cc;
        cc.eyePosition = m_cameraEntity.GetTransform().position;
        cc.viewProj    = cmr.GetViewProj(cc.eyePosition, m_cameraEntity.GetTransform().rotate);
        cc.viewProjInv = cc.viewProj.Invert();

        GetGlobalConstants().UpdateCamera(cc);
    }

    // main path
    {
        // Skybox
        GetGlobalRenderPass().BeginSkyboxPass(0);
        _DrawSkybox(true);

        // PBR
        if (m_wireframeMode)
            GetGlobalRenderPass().BeginPBRWireframePass(0);
        else
            GetGlobalRenderPass().BeginPBRPass(0);

        _DrawBasic(true);
    }

    //===================================================
    // Draw Normal (Debug)
    //===================================================

    if (m_renderNormalMode)
    {
        GetGlobalRenderPass().BeginDrawNormalPass();
        GetView<TransformComponent, ModelRenderer, TAG("PBRGroup")>().each(
            [&](const TransformComponent& t, const ModelRenderer& m)
            {
                for (const auto& node: m.model->GetNodes())
                {
                    _DrawPBRMesh(t, node.mesh, nullptr);
                }
            });
    }

    //===================================================
    // Mirror Pass
    //===================================================

    // 1. Masking Pass
    GetGlobalRenderPass().BeginMirrorMaskingPass(1);
    _DrawMirror(false);

    // 2. Clear Depth Buffer
    r.ClearDepthBuffer(true, 1.f, false, 0);

    // 3. Draw Reflection Pass
    GetView<TransformComponent, TAG("MirrorGroup")>().each(
        [&](const TransformComponent& t)
        {
            CameraComponent    cmr      = m_cameraEntity.GetComponent<CameraComponent>();
            TransformComponent cmrTrans = m_cameraEntity.GetTransform();

            Plane mirrorPlane {
                t.position,
                t.Forward()
            };

            CameraConstant cc = {};
            cc.eyePosition    = cmrTrans.position;
            cc.viewProj       = Mat4::CreateReflection(mirrorPlane) * cmr.GetViewProj(cc.eyePosition, cmrTrans.rotate);
            cc.viewProjInv    = Mat4::Identity;   // not used

            GetGlobalConstants().UpdateReflectCamera(cc);

            GetGlobalRenderPass().BeginSkyboxReflectPass(1);
            _DrawSkybox(true);

            GetGlobalRenderPass().BeginPBRReflectPass(1);
            _DrawBasic(true);
        });

    // 4. Draw Mirror Mesh Pass
    GetGlobalRenderPass().BeginPBRPass(1);
    _DrawMirror(true);

    //===================================================
    // Depth Visualize (Debug)
    //===================================================
    if (m_renderDepthVisualizeMode)
    {
        GetGlobalRenderPass().BeginDepthVisualizePass();

        DepthVisualizeConstant dvc = {};
        dvc.visualFactor           = m_depthVisualizeFactor;
        dvc.cameraPos              = m_cameraEntity.GetTransform().position;
        dvc.invViewProj            = m_cameraEntity.GetComponent<CameraComponent>().GetViewProj(dvc.cameraPos, m_cameraEntity.GetTransform().rotate).Invert();

        GetGlobalConstants().UpdateDepthVisualize(dvc);

        m_depthMap->BindDepthMapTexture(0, eShaderFlags_PixelShader);
        RenderUtil::DrawFullScreenQuad();
    }
}

void AnimationDemo::OnPostRender(TimeStamp& in_ts)
{
    auto& r         = GetRenderer();
    auto  swapChain = r.GetSwapChain();

    // build post process
    if (m_postProcessDirty)
    {
        m_postProcess.ClearFilterList();

        auto [width, height] = GetAppWindow().GetWindowSize();

        // »ùÇÃ¸µ ÇÊÅÍ
        {
            auto filter = ImageFilterFactory::CreateSampling(
                width,
                height,
                GetCommonState()->Sampler_LinearWrap(),
                swapChain->GetResolvedBackBufferImage());

            m_postProcess.AddFilter(filter);
        }

        // Åæ ¸ÅÇÎ
        {
            auto tone = ImageFilterFactory::CreateToneMapping(
                width,
                height,
                GetCommonState()->Sampler_LinearWrap(),
                m_postProcess.GetLastFilter()->GetOutputTexture(),
                ToneMapConstant { 1.f, 2.2f },
                &m_toneMapConstant);

            tone->SetRenderTarget(r.GetBackBuffer());
            m_postProcess.AddFilter(tone);
        }

        m_postProcessDirty = false;
    }

    swapChain->ResolveBackBuffer();
    m_postProcess.Render();
}

void AnimationDemo::OnRenderGUI(TimeStamp& in_ts)
{
    DrawDemoInspector(GetName(),
                      [&]()
                      {
                          ImGui::BulletText("Rasterizer State");
                          ImGui::Checkbox("Wireframe Mode", &m_wireframeMode);

                          if (ImGui::Checkbox("Render Normal Mode", &m_renderNormalMode))
                              GetGlobalConstants().UpdateDrawNormalFactor({ m_renderNormalMode ? m_normalStrength : 0.f });

                          if (ImGui::SliderFloat("Normal Strength", &m_normalStrength, 0.f, 3.f))
                              GetGlobalConstants().UpdateDrawNormalFactor({ m_normalStrength });

                          ImGui::Checkbox("Render Depth Visualize Mode", &m_renderDepthVisualizeMode);
                          ImGui::DragFloat("Depth Visualize Factor", &m_depthVisualizeFactor, 0.01f, 0.01f, 1.f);

                          ImGui::RadioButton("Day", reinterpret_cast<int*>(&m_skyboxType), static_cast<int>(eSkyboxType::Day));
                          ImGui::RadioButton("Night", reinterpret_cast<int*>(&m_skyboxType), static_cast<int>(eSkyboxType::Night));
                      });

    m_sceneHierarchy->DrawSceneHierarchy(this);
    DrawVoidContext(this);
}

void AnimationDemo::OnEvent(CrabEvent& in_event)
{
}
