
#include "pch.h"

#include "23. ShadowMapDemo.h"

#include "CameraScript.h"
#include "ImageFilterFactory.h"
#include "MeshFactory.h"

#include "GUIs.h"

void ShadowMapDemo::Init()
{
    // Texture
    {
        std::filesystem::path path = "Resources\\PBRDemo";

        m_envMapTex = Texture2D::Create(path / "PBRDemoTextureEnvHDR.dds");
        m_spcMapTex = Texture2D::Create(path / "PBRDemoTextureSpecularHDR.dds");
        m_difMapTex = Texture2D::Create(path / "PBRDemoTextureDiffuseHDR.dds");
        m_brdfTex   = Texture2D::Create(path / "PBRDemoTextureBrdf.dds");
    }

    // Post Process
    {
        m_postProcess = PostProcess::Create();

        m_bloomStrength                  = 0.01f;
        m_postEffectData.fogColor        = Vec3::Zero;
        m_postEffectData.fogFallOffStart = 10.f;
        m_postEffectData.fogFallOffEnd   = 100.f;
        m_postEffectData.fogStrength     = 0.5f;

        _BuildPostProcess();
    }

    // Environment Map
    //{
    //    Entity e = CreateEntity();
    //
    //    auto& m       = e.GetOrCreateComponent<SkyboxRenderer>();
    //    m.mesh        = MeshFactory3D::CreateSphereTex(400.f, 16, 16, true);
    //    m.envTexture  = m_envMapTex;
    //    m.spcTexture  = m_spcMapTex;
    //    m.difTexture  = m_difMapTex;
    //    m.brdfTexture = m_brdfTex;
    //    m.transConst  = ConstantBuffer<TransformConstant>::Create();
    //}

    // Cube
    for (int i = 0; i < 10; i++)
    {
        Entity e = CreateEntity();

        auto& t      = e.GetTransform();
        t.position.z = 5.f;
        t.SetRotateYaw(45.f * RAD2DEG);
        t.SetRotatePitch(45.f * RAD2DEG);
        t.scale      = Vec3(1.f, 1.f, 1.f);
        t.position.x = i * 2.f;
        t.position.z = i * 2.f;

        auto& m      = e.GetOrCreateComponent<MeshRenderer>();
        m.mesh       = MeshFactory3D::CreateCubeTexTangent(1.f, 1.f, 1.f);
        m.transConst = ConstantBuffer<TransformConstant>::Create();

        auto& mat      = e.GetOrCreateComponent<PBRMaterialComponent>();
        mat.albedo     = Vec3(1.f, 1.f, 1.f);
        mat.metallic   = 0.5f;
        mat.roughness  = 0.5f;
        mat.emissive   = Vec3(0.f, 0.f, 0.f);
        mat.pbrConst   = ConstantBuffer<PBRMaterialConstant>::Create();
        mat.pbrVSConst = ConstantBuffer<PBRVSConstant>::Create();

        e.GetOrCreateComponent<TagComponent>().name = "Cube" + std::to_string(i);
    }

    // Floor
    {
        Entity e = CreateEntity();

        auto& t      = e.GetTransform();
        t.position.y = -2.f;
        t.SetRotatePitch(90.f * DEG2RAD);
        t.scale = Vec3(100.f, 100.f, 1.f);

        auto& m      = e.GetOrCreateComponent<MeshRenderer>();
        m.mesh       = MeshFactory3D::CreateCubeTexTangent(1.f, 1.f, 1.f);
        m.transConst = ConstantBuffer<TransformConstant>::Create();

        auto& mat      = e.GetOrCreateComponent<PBRMaterialComponent>();
        mat.albedo     = Vec3(1.f, 1.f, 1.f);
        mat.metallic   = 0.5f;
        mat.roughness  = 0.5f;
        mat.emissive   = Vec3(0.f, 0.f, 0.f);
        mat.pbrConst   = ConstantBuffer<PBRMaterialConstant>::Create();
        mat.pbrVSConst = ConstantBuffer<PBRVSConstant>::Create();

        e.GetOrCreateComponent<TagComponent>().name = "Floor";
    }

    // Model
    {
        Entity e = CreateEntity();
        auto&  t = e.GetTransform();

        t.position.x = -5.f;
        t.position.z = 5.f;
        t.scale      = Vec3(1.f, 1.f, 1.f);
        t.AddRotatePitch(90.f * DEG2RAD);

        auto&       m = e.GetOrCreateComponent<MeshRenderer>();
        ModelLoader loader;
        loader.Load(R"(Resources\status\marble_bust_01_2k.fbx)");
        m.mesh       = loader.CreateMergedMesh();
        m.transConst = ConstantBuffer<TransformConstant>::Create();

        auto& mat      = e.GetOrCreateComponent<PBRMaterialComponent>();
        mat.albedo     = Vec3(1.f, 1.f, 1.f);
        mat.metallic   = 0.5f;
        mat.roughness  = 0.5f;
        mat.emissive   = Vec3(0.f, 0.f, 0.f);
        mat.pbrConst   = ConstantBuffer<PBRMaterialConstant>::Create();
        mat.pbrVSConst = ConstantBuffer<PBRVSConstant>::Create();

        std::filesystem::path path = "Resources\\status\\textures";
        mat.albedoMap              = Texture2D::Create(path / "marble_bust_01_diff_2k.jpg");
        mat.roughnessMap           = Texture2D::Create(path / "marble_bust_01_rough_2k.jpg");
        mat.normalMap              = Texture2D::Create(path.parent_path() / "marble_bust_01_nor_gl_2k.exr");
        mat.normalMapType          = PBRMaterialComponent::eNormalMapType::OpenGL;

        e.GetOrCreateComponent<TagComponent>().name = "Model";
    }

    // Camera
    {
        Entity e = CreateEntity();

        auto& cmr          = e.GetOrCreateComponent<CameraComponent>();
        cmr.projectionType = eProjectionType::Perspective;
        cmr.aspect         = GetAppWindow().GetAspect();
        cmr.nearZ          = 0.1f;
        cmr.farZ           = 1000.f;
        cmr.fov            = 45.f * DEG2RAD;

        auto& script  = e.GetOrCreateComponent<ScriptComponent>();
        script.script = CreateScope<CameraScript>(e);

        e.CreateComponent<CameraControlComponent>();
        e.GetOrCreateComponent<TagComponent>().name = "Camera";
        m_camera                                    = e;
    }

    // Light
    {
        Entity e = CreateEntity();

        auto& t    = e.GetTransform();
        t.position = Vec3(3.2f, 1.7f, -8.5f);

        auto& m      = e.GetOrCreateComponent<MeshRenderer>();
        m.mesh       = MeshFactory3D::CreateCubeTexTangent(0.1f, 0.1f, 0.1f);
        m.transConst = ConstantBuffer<TransformConstant>::Create();

        auto& l         = e.GetOrCreateComponent<LightComponent>();
        l.type          = eLightType::Point;
        l.fallOffEnd    = 100.f;
        l.fallOffStart  = 99.f;
        l.lightRadiance = Vec3(1.f, 1.f, 1.f);
        l.spotPower     = 64.f;
        l.lightStrength = 2.f;

        auto [width, height] = GetAppWindow().GetWindowSize();
        auto& s              = e.GetOrCreateComponent<ShadowMapComponent>();
        s.shadowDepthBuffer  = DepthBuffer::Create(width, height, eFormat::Depth_Float32, 1, 0, true, eFormat::Float32);
        s.shadowSampler      = GetRenderStateStorage()->ShadowComparisonLinearClamp();

        e.GetOrCreateComponent<TagComponent>().name = "Light 1";
    }

    // Light (Moving)
    //{
    //    Entity e = CreateEntity();
    //
    //    auto& t    = e.GetTransform();
    //    t.position = Vec3(0.f, 3.f, -3.f);
    //
    //    auto& m      = e.GetOrCreateComponent<MeshRenderer>();
    //    m.mesh       = MeshFactory3D::CreateCubeTexTangent(0.1f, 0.1f, 0.1f);
    //    m.transConst = ConstantBuffer<TransformConstant>::Create();
    //
    //    auto& l         = e.GetOrCreateComponent<LightComponent>();
    //    l.type          = eLightType::Point;
    //    l.fallOffEnd    = 100.f;
    //    l.fallOffStart  = 1.f;
    //    l.lightRadiance = Vec3(1.f, 1.f, 1.f);
    //    l.spotPower     = 1.f;
    //
    //    class LightScript : public Script
    //    {
    //    public:
    //        explicit LightScript(Entity in_owner)
    //            : Script(in_owner)
    //        {
    //        }
    //
    //        void OnUpdate(TimeStamp& in_ts) override
    //        {
    //            m_time += in_ts.deltaTime;
    //            m_time = std::fmod(m_time, 2.f * PI);
    //
    //            auto& t      = m_ownerEntity.GetTransform();
    //            t.position.x = 10.f * std::sin(m_time);
    //            t.position.z = 10.f * std::cos(m_time);
    //        }
    //
    //    private:
    //        float m_time = 0.f;
    //    };
    //
    //    e.GetOrCreateComponent<TagComponent>().name      = "Light 2";
    //    e.GetOrCreateComponent<ScriptComponent>().script = CreateScope<LightScript>(e);
    //}
}

void ShadowMapDemo::OnEnter()
{
    // Panels
    m_sceneHierarchy = CreateScope<SceneHierarchy>();
}

void ShadowMapDemo::OnExit()
{
}

void ShadowMapDemo::OnUpdate(TimeStamp& in_ts)
{
    if (Input::IsKeyPressed(eKey::Escape))
    {
        AppClose_CoreEvent e;
        GetApplication().DispatchEvent(e);
    }
}

Mat4 ShadowMapDemo::_UpdateShadow(uint32 index, TransformComponent& t, ShadowMapComponent& s)
{
    CameraComponent& cmr = m_camera.GetComponent<CameraComponent>();

    Mat4 view = DirectX::XMMatrixLookToLH(
        t.position,
        t.Forward(),
        t.Up());

    Mat4 proj = DirectX::XMMatrixPerspectiveFovLH(
        cmr.fov,
        GetAppWindow().GetAspect(),
        cmr.nearZ,
        cmr.farZ);

    Mat4 viewProj = view * proj;

    CameraConstant cc;
    cc.eyePosition = t.position;
    cc.viewProj    = viewProj;
    GetGlobalConstants().UpdateCamera(cc);

    s.shadowDepthBuffer->Clear(true, 1.f, false, 0);

    D11_API->SetShaderResourceView(nullptr, 20 + index, eShaderFlags_PixelShader);
    D11_API->BindBackBuffer(s.shadowDepthBuffer);

    _RenderSkybox();
    _RenderBasicMesh();

    D11_API->ReleaseRenderTargets();
    s.shadowDepthBuffer->GetTexture()->Bind(20 + index, eShaderFlags_PixelShader);

    s.shadowSampler->Bind(2, eShaderFlags_PixelShader);

    return viewProj;
}

void ShadowMapDemo::OnRender(TimeStamp& in_ts)
{
    auto* dx = D11_API;

    // pre render

    if (m_postProcessDirty)
    {
        _BuildPostProcess();
        m_postProcessDirty = false;
    }

    if (m_postEffectDirty)
    {
        m_postEffectConstant->UpdateData(m_postEffectData);
        m_postEffectDirty = false;
    }

    // Light
    {
        GetPipelineLibrary().DepthOnlyState()->Bind();
        uint32        idx       = 0;
        LightConstant lightData = {};

        GetView<LightComponent, MeshRenderer, TransformComponent, IDComponent>().each(
            [&](LightComponent& l, MeshRenderer& m, TransformComponent& t, IDComponent& id)
            {
                LightConstantData lc;
                lc.lightRadiance  = l.lightRadiance;
                lc.fallOffStart   = l.fallOffStart;
                lc.fallOffEnd     = l.fallOffEnd;
                lc.lightPosition  = t.position;
                lc.lightDirection = t.Forward();
                lc.spotPower      = l.spotPower;
                lc.lightStrength  = l.lightStrength;
                lc.lightType      = l.type;

                if (FindEntity(id).HasComponent<ShadowMapComponent>())
                {
                    lc.useShadowMap  = true;
                    lc.lightViewProj = _UpdateShadow(idx, t, FindEntity(id).GetComponent<ShadowMapComponent>());
                }
                else
                {
                    lc.useShadowMap  = false;
                    lc.lightViewProj = Mat4::Identity;
                }

                lightData.lights[idx++] = lc;

                
            });

        GetGlobalConstants().UpdateLight(lightData);
    }

    dx->BindBackBufferMS(dx->GetDepthBuffer());
    dx->ClearBackBufferMS(Color { 0.f, 0.f, 0.f, 1.f });
    dx->ClearDepthBuffer(true, 1.f, true, 0);

    // Camera
    GetView<TransformComponent, CameraComponent>().each(
        [](TransformComponent& t, CameraComponent& cmr)
        {
            CameraConstant cc;
            cc.eyePosition = t.position;
            cc.viewProj    = cmr.GetViewProj(t.position, t.rotate);
            cc.viewProjInv = cc.viewProj.Invert();
            GetGlobalConstants().UpdateCamera(cc);
        });

    // Basic Pass
    {
        GetPipelineLibrary().SkyboxState()->Bind();
        _RenderSkybox();

        GetPipelineLibrary().PBRState()->Bind();
        _RenderBasicMesh();

        // Temp: depth only state is very simple (just white color)
        GetPipelineLibrary().DepthOnlyState()->Bind();
        _RenderLightMesh();
    }

    // Depth Only Pass
    {
        GetPipelineLibrary().DepthOnlyState()->Bind();
        dx->BindBackBuffer(dx->GetDepthOnlyBuffer());
        dx->ClearDepthOnlyBuffer(true, 1.f, false, 0);

        _RenderSkybox();
        _RenderBasicMesh();
        _RenderLightMesh();
    }

    m_postProcess->Render();
    dx->BindBackBuffer(nullptr);
}

void ShadowMapDemo::OnRenderGUI(TimeStamp& in_ts)
{
    DrawDemoInspector(GetName(),
                      [&]()
                      {
                          if (ImGui::TreeNode("Post Process"))
                          {
                              ImGui::SeparatorText("Bloom");
                              if (ImGui::Checkbox("Bloom Enable", &m_bloomEnable))
                                  m_postProcessDirty = true;

                              ImGui::BeginDisabled(!m_bloomEnable);
                              {
                                  if (ImGui::SliderInt("Bloom Sampling Count", &m_bloomMipChainCount, 1, 10))
                                      m_postProcessDirty = true;

                                  if (ImGui::DragFloat("Bloom Strength", &m_bloomStrength, 0.01f, 0.f, 1.f))
                                      m_combineConstant->UpdateData({ m_bloomStrength });
                              }
                              ImGui::EndDisabled();

                              ImGui::SeparatorText("Post Effect");
                              if (ImGui::ColorEdit3("Fog Color", &m_postEffectData.fogColor.x))
                                  m_postEffectDirty = true;

                              if (ImGui::DragFloat("Fog strength", &m_postEffectData.fogStrength, 0.01f, 0.f, 5.f))
                                  m_postEffectDirty = true;

                              if (ImGui::DragFloat("Fog fall off start", &m_postEffectData.fogFallOffStart, 0.1f, 0.f, 100.f))
                                  m_postEffectDirty = true;

                              if (ImGui::DragFloat("Fog fall off end", &m_postEffectData.fogFallOffEnd, 0.1f, 0.f, 100.f))
                                  m_postEffectDirty = true;

                              ImGui::TreePop();
                          }

                          if (ImGui::TreeNode("Skybox"))
                          {
                              static bool s_dirtyBit = true;
                              static int  s_texType  = 0;

                              if (ImGui::RadioButton("Environment", &s_texType, 0))
                                  s_dirtyBit = true;

                              if (ImGui::RadioButton("Diffuse", &s_texType, 1))
                                  s_dirtyBit = true;

                              if (ImGui::RadioButton("Irradiance", &s_texType, 2))
                                  s_dirtyBit = true;

                              if (s_dirtyBit)
                                  GetGlobalConstants().UpdateSkyboxPS({ s_texType });

                              ImGui::TreePop();
                          }
                      });

    m_sceneHierarchy->DrawSceneHierarchy(this);
    DrawVoidContext(this);

    ImGui::Begin("Mirror Test");

    ImGui::SliderFloat("Mirror Alpha", &m_mirrorAlpha, 0.f, 1.f);

    ImGui::End();
}

void ShadowMapDemo::OnEvent(CrabEvent& in_event)
{
}

void ShadowMapDemo::_RenderBasicMesh()
{
    // Common Mesh
    GetView<TransformComponent, MeshRenderer, PBRMaterialComponent>(entt::exclude<MirrorComponent>)
        .each([&](TransformComponent& t, MeshRenderer& m, PBRMaterialComponent& mat)
              { _RenderPBRMesh(t, m, mat); });
}

void ShadowMapDemo::_RenderLightMesh()
{
    GetView<LightComponent, MeshRenderer, TransformComponent>().each(
        [&](LightComponent& l, MeshRenderer& m, TransformComponent& t)
        {
            auto* dx = D11_API;

            TransformConstant tc;
            tc.world             = t.GetWorld();
            tc.worldInvTranspose = tc.world.Invert().Transpose();
            m.transConst->Bind(0, eShaderFlags_VertexShader);
            m.transConst->UpdateData(tc);

            m.mesh->Draw();
        });
}

void ShadowMapDemo::_RenderSkybox()
{
    // Environment Map
    GetView<TransformComponent, SkyboxRenderer>().each(
        [](TransformComponent& t, SkyboxRenderer& s)
        {
            auto*             dx = D11_API;
            TransformConstant tc;
            tc.world             = t.GetWorld();
            tc.worldInvTranspose = tc.world.Invert().Transpose();
            s.transConst->Bind(0, eShaderFlags_VertexShader);
            s.transConst->UpdateData(tc);

            if (s.envTexture)
                s.envTexture->Bind(0, eShaderFlags_PixelShader);
            else
                dx->SetShaderResourceView(nullptr, 0, eShaderFlags_PixelShader);

            if (s.difTexture)
                s.difTexture->Bind(10, eShaderFlags_PixelShader);
            else
                dx->SetShaderResourceView(nullptr, 10, eShaderFlags_PixelShader);

            if (s.spcTexture)
                s.spcTexture->Bind(11, eShaderFlags_PixelShader);
            else
                dx->SetShaderResourceView(nullptr, 11, eShaderFlags_PixelShader);

            if (s.brdfTexture)
                s.brdfTexture->Bind(12, eShaderFlags_PixelShader);
            else
                dx->SetShaderResourceView(nullptr, 12, eShaderFlags_PixelShader);

            s.mesh->Draw();
        });
}

void ShadowMapDemo::_RenderPBRMesh(TransformComponent& t, MeshRenderer& m, PBRMaterialComponent& mat)
{
    TransformConstant tc = {};
    tc.world             = t.GetWorld();
    tc.worldInvTranspose = tc.world.Invert().Transpose();
    m.transConst->Bind(0, eShaderFlags_VertexShader);
    m.transConst->UpdateData(tc);

    PBRMaterialConstant pc = {};
    pc.albedo              = mat.albedo;
    pc.emissive            = mat.emissive;
    pc.metallic            = mat.metallic;
    pc.roughness           = mat.roughness;
    pc.useAlbedoMap        = mat.albedoMap != nullptr;
    pc.useNormalMap        = mat.normalMap != nullptr;
    pc.useAOMap            = mat.aoMap != nullptr;
    pc.invertNormalMapY    = mat.normalMapType == PBRMaterialComponent::eNormalMapType::OpenGL;
    pc.useMetallicMap      = mat.metallicMap != nullptr;
    pc.useRoughnessMap     = mat.roughnessMap != nullptr;
    pc.useEmissiveMap      = mat.emissiveMap != nullptr;

    PBRVSConstant pvs      = {};
    pvs.useDisplacementMap = mat.displacementMap != nullptr;
    pvs.displacementScale  = mat.displacementScale;

    mat.pbrConst->Bind(3, eShaderFlags_PixelShader);
    mat.pbrConst->UpdateData(pc);

    mat.pbrVSConst->Bind(2, eShaderFlags_VertexShader);
    mat.pbrVSConst->UpdateData(pvs);

    m.transConst->Bind(0, eShaderFlags_VertexShader);
    m.transConst->UpdateData(tc);

    auto* dx = D11_API;

    if (mat.displacementMap)
        mat.displacementMap->Bind(0, eShaderFlags_VertexShader);
    else
        dx->SetShaderResourceView(nullptr, 0, eShaderFlags_VertexShader);

    if (mat.albedoMap)
        mat.albedoMap->Bind(0, eShaderFlags_PixelShader);
    else
        dx->SetShaderResourceView(nullptr, 0, eShaderFlags_PixelShader);

    if (mat.normalMap)
        mat.normalMap->Bind(1, eShaderFlags_PixelShader);
    else
        dx->SetShaderResourceView(nullptr, 1, eShaderFlags_PixelShader);

    if (mat.aoMap)
        mat.aoMap->Bind(2, eShaderFlags_PixelShader);
    else
        dx->SetShaderResourceView(nullptr, 2, eShaderFlags_PixelShader);

    if (mat.metallicMap)
        mat.metallicMap->Bind(3, eShaderFlags_PixelShader);
    else
        dx->SetShaderResourceView(nullptr, 3, eShaderFlags_PixelShader);

    if (mat.roughnessMap)
        mat.roughnessMap->Bind(4, eShaderFlags_PixelShader);
    else
        dx->SetShaderResourceView(nullptr, 4, eShaderFlags_PixelShader);

    if (mat.emissiveMap)
        mat.emissiveMap->Bind(5, eShaderFlags_PixelShader);
    else
        dx->SetShaderResourceView(nullptr, 5, eShaderFlags_PixelShader);

    m.mesh->Draw();
}

void ShadowMapDemo::_BuildPostProcess()
{
    m_postProcess->ClearFilterList();
    m_combineConstant.reset();
    m_postEffectConstant.reset();

    auto [width, height] = GetAppWindow().GetWindowSize();

    auto sampling1 = ImageFilterFactory::CreateSampling(width, height);

    sampling1->AddInputTexture(D11_API->GetSwapChain()->GetBackBufferMSResolvedTexture(), 0);
    sampling1->SetOnBindCallback([]()
                                 { D11_API->GetSwapChain()->ResolveBackBufferMS(); });
    m_postProcess->AddFilter(sampling1);

    auto postEffect = ImageFilterFactory::CreatePostEffect(width, height, m_postEffectData);
    postEffect->AddInputTexture(sampling1->GetOutputTexture(), 0);
    postEffect->AddInputTexture(D11_API->GetSwapChain()->GetDepthOnlyBufferTexture(), 1);
    postEffect->AddInputTexture(sampling1->GetOutputTexture(), 2);
    m_postProcess->AddFilter(postEffect);

    m_postEffectConstant = postEffect->GetConstantBufferList().FindBuffer<PostEffectConstant>();

    if (m_bloomEnable)
    {
        // down
        for (uint32 i = 0; i < m_bloomMipChainCount; ++i)
        {
            float w = static_cast<float>(width) / std::pow(2, i + 1);
            float h = static_cast<float>(height) / std::pow(2, i + 1);

            auto blurDown = ImageFilterFactory::CreateBlurDown(static_cast<uint32>(w), static_cast<uint32>(h));
            blurDown->AddInputTexture(m_postProcess->GetLastFilter()->GetOutputTexture(), 0);
            m_postProcess->AddFilter(blurDown);
        }

        // up
        for (uint32 i = 0; i < m_bloomMipChainCount; ++i)
        {
            float w      = static_cast<float>(width) / std::pow(2, m_bloomMipChainCount - i - 1);
            float h      = static_cast<float>(height) / std::pow(2, m_bloomMipChainCount - i - 1);
            auto  blurUp = ImageFilterFactory::CreateBlurUp(static_cast<uint32>(w), static_cast<uint32>(h), 0.01f);
            blurUp->AddInputTexture(m_postProcess->GetLastFilter()->GetOutputTexture(), 0);
            m_postProcess->AddFilter(blurUp);
        }

        // combine
        auto combine = ImageFilterFactory::CreateCombine(width, height, m_bloomStrength);
        combine->AddInputTexture(m_postProcess->GetLastFilter()->GetOutputTexture(), 0);
        combine->AddInputTexture(postEffect->GetOutputTexture(), 1);
        m_postProcess->AddFilter(combine);

        m_combineConstant = combine->GetConstantBufferList().FindBuffer<CombinePSConstant>();
    }

    auto toneMap = ImageFilterFactory::CreateToneMapping(width, height, 1.f, 2.2f);
    toneMap->AddInputTexture(m_postProcess->GetLastFilter()->GetOutputTexture(), 0);
    toneMap->SetRenderTarget(D11_API->GetSwapChain()->GetBackBuffer());
    m_postProcess->AddFilter(toneMap);
}