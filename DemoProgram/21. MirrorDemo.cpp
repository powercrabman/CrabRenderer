
#include "pch.h"

#include "21. MirrorDemo.h"

#include "CameraScript.h"
#include "ImageFilterFactory.h"
#include "MeshFactory.h"

#include "GUIs.h"

void MirrorDemo::Init()
{
    // Pipeline
    {
        m_basicPipeline  = GetPipelineLibrary().PBRState();
        m_skyboxPipeline = GetPipelineLibrary().SkyboxState();
    }

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
        _BuildPostProcess();
    }

    // Environment Map
    {
        Entity e = CreateEntity();

        auto& m       = e.GetOrCreateComponent<SkyboxRenderer>();
        m.mesh        = MeshFactory3D::CreateSphereTex(400.f, 16, 16, true);
        m.envTexture  = m_envMapTex;
        m.spcTexture  = m_spcMapTex;
        m.difTexture  = m_difMapTex;
        m.brdfTexture = m_brdfTex;
        m.transConst  = ConstantBuffer<TransformConstant>::Create();
    }

    // Cube
    {
        Entity e = CreateEntity();

        auto& t      = e.GetTransform();
        t.position.z = 5.f;
        t.SetRotateYaw(45.f * RAD2DEG);
        t.SetRotatePitch(45.f * RAD2DEG);
        t.scale = Vec3(1.f, 1.f, 1.f);

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

        e.GetOrCreateComponent<TagComponent>().name = "Cube";
    }

    // Sphere
    {
        Entity e = CreateEntity();

        auto& t      = e.GetTransform();
        t.position.z = 5.f;
        t.position.x = 4.f;
        t.scale      = Vec3(1.f, 1.f, 1.f);

        auto& m      = e.GetOrCreateComponent<MeshRenderer>();
        m.mesh       = MeshFactory3D::CreateSphereTexTangent(1.f, 128, 128);
        m.transConst = ConstantBuffer<TransformConstant>::Create();

        auto& mat      = e.GetOrCreateComponent<PBRMaterialComponent>();
        mat.albedo     = Vec3(1.f, 1.f, 1.f);
        mat.metallic   = 0.5f;
        mat.roughness  = 0.5f;
        mat.emissive   = Vec3(0.f, 0.f, 0.f);
        mat.pbrConst   = ConstantBuffer<PBRMaterialConstant>::Create();
        mat.pbrVSConst = ConstantBuffer<PBRVSConstant>::Create();

        e.GetOrCreateComponent<TagComponent>().name = "Sphere";
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

        m_camera = e;
    }

    // Mirror
    {
        Entity e = CreateEntity();

        auto& t      = e.GetTransform();
        t.position.z = 10.f;
        t.position.x = 0.f;
        t.scale      = Vec3(1.f, 1.f, 1.f);

        auto& m      = e.GetOrCreateComponent<MeshRenderer>();
        m.mesh       = MeshFactory3D::CreatePlaneTexTangent(10.f, 20.f, 1, 1);
        m.transConst = ConstantBuffer<TransformConstant>::Create();

        auto& mat      = e.GetOrCreateComponent<PBRMaterialComponent>();
        mat.albedo     = Vec3(1.f, 1.f, 1.f);
        mat.metallic   = 0.5f;
        mat.roughness  = 0.5f;
        mat.emissive   = Vec3(0.f, 0.f, 0.f);
        mat.pbrConst   = ConstantBuffer<PBRMaterialConstant>::Create();
        mat.pbrVSConst = ConstantBuffer<PBRVSConstant>::Create();

        auto& mirror = e.GetOrCreateComponent<MirrorComponent>();
        mirror.alpha = 0.5f;

        e.GetOrCreateComponent<TagComponent>().name = "Mirror";

        m_mirror = e;
    }
}

void MirrorDemo::OnEnter()
{
    // Panels
    m_sceneHierarchy = CreateScope<SceneHierarchy>();
}

void MirrorDemo::OnExit()
{
}

void MirrorDemo::OnUpdate(TimeStamp& in_ts)
{
    if (Input::IsKeyPressed(eKey::Escape))
    {
        AppClose_CoreEvent e;
        GetApplication().DispatchEvent(e);
    }
}

void MirrorDemo::_DrawMirror()
{
    GetView<TransformComponent, MeshRenderer, PBRMaterialComponent, MirrorComponent>().each(
        [&](TransformComponent& t, MeshRenderer& m, PBRMaterialComponent& mat, MirrorComponent& mirror)
        {
            _RenderPBRMesh(t, m, mat);
        });
}

void MirrorDemo::OnRender(TimeStamp& in_ts)
{
    auto* dx = D11_API;

    // pre render

    if (m_postProcessDirty)
    {
        _BuildPostProcess();
        m_postProcessDirty = false;
    }

    dx->BindFloatBackBuffer();
    dx->ClearFloatBackBuffer(Color { 0.f, 0.f, 0.f, 1.f });
    dx->ClearDepthBuffer(true, 1.f, true, 0);

    // Camera
    GetView<TransformComponent, CameraComponent>().each(
        [](TransformComponent& t, CameraComponent& cmr)
        {
            CameraConstant cc;
            cc.eyePosition = t.position;
            cc.viewProj    = cmr.GetViewProj(t.position, t.rotate);
            GetGlobalConstants().UpdateCamera(cc);
        });

    // Basic Pass
    {
        m_skyboxPipeline->Bind();
        _RenderSkybox();

        m_basicPipeline->Bind();
        _RenderBasicMesh();
    }

    // Mirror Pass
    if (GetView<MirrorComponent>().size() > 0)
    {
        // Mirror Rendering
        GetRenderStateStorage()->DepthTestDrawStencil()->Bind(1);

        _DrawMirror();

        dx->ClearDepthBuffer(true, 1.f, false, 0);

        {
            auto& mirrorT = m_mirror.GetTransform();

            Vec3  worldNormal = mirrorT.Backward();
            Plane mirrorPlane { mirrorT.position, worldNormal };
            mirrorPlane.Normalize();

            auto& camTrans = m_camera.GetTransform();
            auto& camComp  = m_camera.GetComponent<CameraComponent>();
            Mat4  viewProj = camComp.GetViewProj(camTrans.position, camTrans.rotate);

            Mat4 reflectionMat = Mat4::CreateReflection(mirrorPlane);
            Mat4 refViewProj   = reflectionMat * viewProj;

            CameraConstant cc;
            cc.viewProj    = refViewProj;
            cc.eyePosition = camTrans.position;
            GetGlobalConstants().UpdateCamera(cc);
        }

        // Basic Rendering
        GetPipelineLibrary().SkyboxMirrorState()->Bind(1);
        _RenderSkybox();

        GetPipelineLibrary().PBRMirrorState()->Bind(1);
        _RenderBasicMesh();

        // this is not good for performance
        {
            CameraConstant cc;
            cc.viewProj = m_camera.GetComponent<CameraComponent>().GetViewProj(m_camera.GetTransform().position, m_camera.GetTransform().rotate);
            cc.eyePosition = m_camera.GetTransform().position;
            GetGlobalConstants().UpdateCamera(cc);
        }

        GetRenderStateStorage()->CullCounterClockwise(true)->Bind();
        GetRenderStateStorage()->AlphaBlend(true)->Bind({ m_mirrorAlpha, m_mirrorAlpha, m_mirrorAlpha, 1.f });
        _DrawMirror();
    }

    m_postProcess->Render();
    dx->BindBackBuffer();
}

void MirrorDemo::OnRenderGUI(TimeStamp& in_ts)
{
    DrawDemoInspector(GetName(),
                      [&]()
                      {
                          if (ImGui::TreeNode("Post Process"))
                          {
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

void MirrorDemo::OnEvent(CrabEvent& in_event)
{
}

void MirrorDemo::_RenderBasicMesh()
{
    // Common Mesh
    GetView<TransformComponent, MeshRenderer, PBRMaterialComponent>(entt::exclude<MirrorComponent>)
        .each([&](TransformComponent& t, MeshRenderer& m, PBRMaterialComponent& mat)
              { _RenderPBRMesh(t, m, mat); });
}

void MirrorDemo::_RenderSkybox()
{
    // Environment Map
    GetView<TransformComponent, SkyboxRenderer>().each(
        [](TransformComponent& t, SkyboxRenderer& s)
        {
            auto*             dx = D11_API;
            TransformConstant tc = {};
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

void MirrorDemo::_RenderPBRMesh(TransformComponent& t, MeshRenderer& m, PBRMaterialComponent& mat)
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

void MirrorDemo::_BuildPostProcess()
{
    m_postProcess->ClearFilterList();
    m_combineConstant.reset();

    auto [width, height] = GetAppWindow().GetWindowSize();

    auto sampling1 = ImageFilterFactory::CreateSampling(width, height);

    Texture2DList list;
    sampling1->AddInputTexture(D11_API->GetSwapChain()->GetFloatBackBufferResolveTex(), 0);
    sampling1->SetOnBindCallback([]()
                                 { D11_API->GetSwapChain()->ResolveFloatBackBufferTex(); });
    m_postProcess->AddFilter(sampling1);

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
        combine->AddInputTexture(sampling1->GetOutputTexture(), 1);
        m_postProcess->AddFilter(combine);

        m_combineConstant = combine->GetConstantBufferList().FindBuffer<CombinePSConstant>();
    }

    auto toneMap = ImageFilterFactory::CreateToneMapping(width, height, 1.f, 2.2f);
    toneMap->AddInputTexture(m_postProcess->GetLastFilter()->GetOutputTexture(), 0);
    toneMap->SetRenderTarget(D11_API->GetSwapChain()->GetBackBufferRT());
    m_postProcess->AddFilter(toneMap);
}