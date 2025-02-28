#include "pch.h"

#include "19. PBRDemo.h"

#include "GUIs.h"
#include "MeshFactory.h"

void PBRDemo::Init()
{
    // Init
    {
        // render state
        m_wrapSampler  = GetRenderStateStorage()->LinearWrap();
        m_clampSampler = GetRenderStateStorage()->LinearClamp();
        m_rsStateDirty = true;

        // constant buffer
        m_cbHDRI              = ConstantBuffer<CB_HDRI>::Create();
        m_cbBlurCombine       = ConstantBuffer<CB_BlurCombine>::Create();
        m_cbObject            = ConstantBuffer<CB_Object>::Create();
        m_cbCamera            = ConstantBuffer<CB_Camera>::Create();
        m_cbBlur              = ConstantBuffer<CB_Blur>::Create();
        m_cbLight             = ConstantBuffer<CB_Light>::Create();
        m_cbObjectPixelShader = ConstantBuffer<CB_ObjectPixelShader>::Create();

        // Shaders
        m_basicVS = VertexShader::CreateFromFile("PBRCommonVS.hlsl", "main", Vertex::s_elements);
        m_basicPS = PixelShader::CreateFromFile("PBRCommonPS.hlsl", "main");

        // Texture
        std::filesystem::path path = "Resources/PBRDemo";
        m_envMapTex                = Texture2D::CreateTextureCube(path / "PBRDemoTextureEnvHDR.dds");
        m_irradianceTex            = Texture2D::CreateTextureCube(path / "PBRDemoTextureDiffuseHDR.dds");
        m_specularTex              = Texture2D::CreateTextureCube(path / "PBRDemoTextureSpecularHDR.dds");
        m_brdfLUT                  = Texture2D::Create(path / "PBRDemoTextureBrdf.dds");

        m_normalTex    = Texture2D::Create(path / "steelplate1_normal-dx.png");
        m_albedoTex    = Texture2D::Create(path / "steelplate1_albedo.png");
        m_metallicTex  = Texture2D::Create(path / "steelplate1_metallic.png");
        m_roughnessTex = Texture2D::Create(path / "steelplate1_roughness.png");
        m_aoTex        = Texture2D::Create(path / "steelplate1_ao.png");
        m_heightTex    = Texture2D::Create(path / "steelplate1_height.png");
    }

    // Entity
    {
        // sphere
        {
            Entity e = CreateEntity();

            auto& mc        = e.AddComponent<MeshComponent>();
            mc.topology     = eTopology::TriangleList;
            mc.mesh         = MeshFactory3D::CreateSphereTexTangent(1.f, 128, 128);
            mc.vertexShader = m_basicVS;
            mc.pixelShader  = m_basicPS;
            mc.constantBufferList
                .Add(m_cbObject, 0, eShaderFlags_VertexPixelShader)
                .Add(m_cbCamera, 1, eShaderFlags_VertexPixelShader)
                .Add(m_cbLight, 2, eShaderFlags_PixelShader)
                .Add(m_cbObjectPixelShader, 3, eShaderFlags_VertexPixelShader);

            mc.samplerList
                .Add(m_wrapSampler, 0, eShaderFlags_VertexPixelShader)
                .Add(m_clampSampler, 1, eShaderFlags_PixelShader);

            mc.textureList
                .Add(m_irradianceTex, 0, eShaderFlags_PixelShader)
                .Add(m_specularTex, 1, eShaderFlags_PixelShader)
                .Add(m_brdfLUT, 2, eShaderFlags_PixelShader)
                .Add(m_normalTex, 3, eShaderFlags_PixelShader)
                .Add(m_albedoTex, 4, eShaderFlags_PixelShader)
                .Add(m_roughnessTex, 5, eShaderFlags_PixelShader)
                .Add(m_metallicTex, 6, eShaderFlags_PixelShader)
                .Add(m_aoTex, 7, eShaderFlags_PixelShader)
                .Add(m_heightTex, 0, eShaderFlags_VertexShader);

            auto& mat     = e.AddComponent<MaterialComponent>();
            mat.albedo    = Vec3(1.f, 1.f, 1.f);
            mat.metallic  = 0.5f;
            mat.roughness = 0.5f;

            m_sphereEntity = e;
        }

        // environment
        {
            Entity e = CreateEntity();

            auto& mc        = e.AddComponent<MeshComponent>();
            mc.topology     = eTopology::TriangleList;
            mc.mesh         = MeshFactory3D::CreateSphereTex(300.f, 32, 32, true);
            mc.vertexShader = VertexShader::CreateFromFile("EnvmapVS.hlsl", "main", Vertex3D_PosNormalTex::s_elements);
            mc.pixelShader  = PixelShader::CreateFromFile("EnvmapPS.hlsl", "main");
            mc.constantBufferList
                .Add(m_cbObject, 0, eShaderFlags_VertexPixelShader)
                .Add(m_cbCamera, 1, eShaderFlags_VertexPixelShader);

            mc.samplerList
                .Add(m_wrapSampler, 0, eShaderFlags_PixelShader);

            mc.textureList
                .Add(m_envMapTex, 0, eShaderFlags_PixelShader);
        }

        // model
        {
            Entity e = CreateEntity();

            auto& mc        = e.AddComponent<ModelComponent>();
            mc.topology     = eTopology::TriangleList;
            mc.model        = Model::Create("Resources\\dragon\\Black Dragon NEW\\Dragon 2.5_fbx.fbx");
            mc.vertexShader = m_basicVS;
            mc.pixelShader  = m_basicPS;

            mc.constantBufferList
                .Add(m_cbObject, 0, eShaderFlags_VertexPixelShader)
                .Add(m_cbCamera, 1, eShaderFlags_VertexPixelShader)
                .Add(m_cbLight, 2, eShaderFlags_PixelShader)
                .Add(m_cbObjectPixelShader, 3, eShaderFlags_VertexPixelShader);

            mc.samplerList
                .Add(m_wrapSampler, 0, eShaderFlags_VertexPixelShader)
                .Add(m_clampSampler, 1, eShaderFlags_PixelShader);

            auto& mat     = e.AddComponent<MaterialComponent>();
            mat.albedo    = Vec3(1.f, 1.f, 1.f);
            mat.metallic  = 0.5f;
            mat.roughness = 0.5f;

            m_modelEntity = e;
        }

        // camera
        {
            Entity e       = CreateEntity();
            m_cameraEntity = e;

            auto& cc           = e.AddComponent<CameraControllerComponent>();
            cc.moveSpeed       = 1.f;
            cc.rotateSpeed     = 10.f * DEG2RAD;
            auto& cmr          = cc.camera;
            cmr.aspect         = GetAppWindow().GetAspect();
            cmr.fov            = 45.f * DEG2RAD;
            cmr.nearZ          = 0.1f;
            cmr.farZ           = 1000.f;
            cmr.projectionType = eProjectionType::Perspective;

            auto& t      = e.GetComponent<Transform>();
            t.position.z = -3.f;

            class CameraControllerScript : public Script
            {
            public:
                CameraControllerScript(Entity& in_entity)
                    : Script(in_entity)
                {
                }

                void OnUpdate(TimeStamp& in_ts) override
                {
                    auto [cc, t] = GetEntity().GetComponents<CameraControllerComponent, Transform>();

                    if (Input::IsKeyDown(eKey::A))
                        t.position += t.Left() * cc.moveSpeed * in_ts.deltaTime;

                    if (Input::IsKeyDown(eKey::D))
                        t.position += t.Right() * cc.moveSpeed * in_ts.deltaTime;

                    if (Input::IsKeyDown(eKey::W))
                        t.position += t.Forward() * cc.moveSpeed * in_ts.deltaTime;

                    if (Input::IsKeyDown(eKey::S))
                        t.position += t.Backward() * cc.moveSpeed * in_ts.deltaTime;

                    if (Input::IsKeyDown(eKey::Q))
                        t.position += t.Down() * cc.moveSpeed * in_ts.deltaTime;

                    if (Input::IsKeyDown(eKey::E))
                        t.position += t.Up() * cc.moveSpeed * in_ts.deltaTime;

                    if (Input::IsKeyPressed(eKey::Space))
                    {
                        GetAppWindow().EnableMouseRelativeMode(true);
                        m_rotateMode = true;
                    }

                    if (Input::IsKeyReleased(eKey::Space))
                    {
                        GetAppWindow().EnableMouseRelativeMode(false);
                        m_rotateMode = false;
                    }

                    if (m_rotateMode)
                    {
                        auto [dx, dy] = Input::GetMouseRelativeDeltaPos();

                        if (dx != 0 || dy != 0)
                        {
                            Vec3 euler = t.quaternion.ToEuler();
                            euler.x += dy * cc.rotateSpeed * in_ts.deltaTime;
                            euler.y += dx * cc.rotateSpeed * in_ts.deltaTime;

                            t.quaternion = EulerToQuat(euler);
                        }
                    }
                }

            private:
                bool m_rotateMode = false;
            };

            e.AddComponent<ScriptComponent>(CreateScope<CameraControllerScript>(e));
        }

        // Light
        {
            Entity e = CreateEntity();

            auto& t       = e.GetComponent<Transform>();
            t.position    = Vec3(3.f, 3.f, -3.f);
            m_lightEntity = e;

            e.AddComponent<LightComponent>();
        }
    }

    m_postProcess = PostProcess::Create();
}

void PBRDemo::OnEnter()
{
}

void PBRDemo::OnExit()
{
}

void PBRDemo::OnUpdate(TimeStamp& in_ts)
{
    auto* api = D11_API;

    if (Input::IsKeyPressed(eKey::Escape))
    {
        AppClose_CoreEvent e;
        GetApplication().DispatchEvent(e);
    }
}

void PBRDemo::OnRender(TimeStamp& in_ts)
{
    auto* api = D11_API;
    api->BindFloatBackBuffer();
    api->ClearFloatBackBuffer(color::BLACK);
    api->ClearDepthBuffer();

    if (m_rsStateDirty)
    {
        m_rsState = RasterizerState::Create(
            m_enableCulling ? eCullMode::Back : eCullMode::None,
            m_enableWireframeMode ? eFillMode::Wireframe : eFillMode::Solid,
            eFrontFace::ClockWise);

        m_rsState->Bind();
        m_rsStateDirty = false;
    }

    if (m_hdriDirty)
    {
        CB_HDRI data;
        data.exposure = m_hdriExposure;
        data.gamma    = m_hdriGamma;
        m_cbHDRI->UpdateData(data);
        m_hdriDirty = false;
    }

    if (m_blurDirty)
    {
        CB_BlurCombine data2;
        data2.blurStrength = m_blurStrength;
        m_cbBlurCombine->UpdateData(data2);
    }

    if (m_needPostProcessRebuild)
    {
        _BuildPostProcess();
        m_needPostProcessRebuild = false;
    }

    // Camera
    {
        auto [t, c] = m_cameraEntity.GetComponents<Transform, CameraControllerComponent>();

        CB_Camera data;
        data.eyePosW  = t.position;
        data.viewProj = c.camera.GetViewProj(t);

        m_cbCamera->UpdateData(data);
    }

    // Light
    {
        auto [t, l] = m_lightEntity.GetComponents<Transform, LightComponent>();

        CB_LightData lightData;
        lightData.position     = t.position;
        lightData.radiance     = l.radiance;
        lightData.direction    = t.Forward();
        lightData.fallOffStart = l.fallOffStart;
        lightData.fallOffEnd   = l.fallOffEnd;
        lightData.spotPower    = l.spotPower;

        CB_Light data;
        data.light = lightData;

        m_cbLight->UpdateData(data);
    }

    GetView<MeshComponent, Transform, IDComponent>().each(
        [&](MeshComponent& in_mc,
            Transform&     in_t,
            IDComponent&   in_id)
        {
            CB_Object data;
            data.world             = in_t.GetWorld();
            data.worldInvTranspose = in_t.GetWorldInvTranspose();
            m_cbObject->UpdateData(data);

            Entity e = FindEntity(in_id);
            if (e.HasComponent<MaterialComponent>())
            {
                auto&                mat   = e.GetComponent<MaterialComponent>();
                CB_ObjectPixelShader data2 = {};
                data2.material             = mat;
                data2.useNormalMap         = m_useNormalMap;
                data2.useHeightMap         = m_useHeightMap;
                data2.heightScale          = m_heightScale;
                data2.useAlbedoMap         = m_useAlbedoMap;
                data2.useRoughnessMap      = m_useRoughnessMap;
                data2.useMetallicMap       = m_useMetallicMap;
                data2.useAOMap             = m_useAOMap;
                m_cbObjectPixelShader->UpdateData(data2);
            }

            api->SetTopology(in_mc.topology);

            in_mc.vertexShader->Bind();
            in_mc.pixelShader->Bind();
            in_mc.samplerList.Bind();
            in_mc.textureList.Bind();
            in_mc.constantBufferList.Bind();

            in_mc.mesh->Draw();
        });

    GetView<ModelComponent, Transform, IDComponent>().each(
        [&](ModelComponent& in_mc,
            Transform&      in_t,
            IDComponent&    in_id)
        {
            CB_Object data;
            data.world             = in_t.GetWorld();
            data.worldInvTranspose = in_t.GetWorldInvTranspose();
            m_cbObject->UpdateData(data);

            Entity e = FindEntity(in_id);
            if (e.HasComponent<MaterialComponent>())
            {
                auto&                mat   = e.GetComponent<MaterialComponent>();
                CB_ObjectPixelShader data2 = {};
                data2.material             = mat;
                data2.useNormalMap         = m_useNormalMap;
                data2.useHeightMap         = m_useHeightMap;
                data2.heightScale          = m_heightScale;
                data2.useAlbedoMap         = m_useAlbedoMap;
                data2.useRoughnessMap      = m_useRoughnessMap;
                data2.useMetallicMap       = m_useMetallicMap;
                data2.useAOMap             = m_useAOMap;
                m_cbObjectPixelShader->UpdateData(data2);
            }

            api->SetTopology(in_mc.topology);

            in_mc.vertexShader->Bind();
            in_mc.pixelShader->Bind();
            in_mc.samplerList.Bind();
            in_mc.constantBufferList.Bind();

            in_mc.model->Draw();
        });

    m_postProcess->Render();

    api->BindBackBuffer();
}

void PBRDemo::OnRenderGUI(TimeStamp& in_ts)
{
    DrawDemoInspector(GetName(),
                      [&]()
                      {
                          ImGui::SeparatorText("Camera");
                          {
                              auto [t, cc] = m_cameraEntity.GetComponents<Transform, CameraControllerComponent>();
                              ImGui::Text("Position : (%.2f, %.2f, %.2f)", t.position.x, t.position.y, t.position.z);

                              Vec3 euler = t.quaternion.ToEuler();
                              ImGui::Text("Rotation : (%.2f, %.2f, %.2f)", euler.x, euler.y, euler.z);

                              ImGui::DragFloat("Move Speed", &cc.moveSpeed, 0.1f, 0.f, 100.f);
                              ImGui::DragFloat("Rotate Speed", &cc.rotateSpeed, 0.1f, 0.f, 100.f);
                          }

                          ImGui::SeparatorText("Rasterizer State");
                          if (ImGui::Checkbox("Enable culling", &m_enableCulling))
                              m_rsStateDirty = true;

                          if (ImGui::Checkbox("Enable wireframe mode", &m_enableWireframeMode))
                              m_rsStateDirty = true;

                          if (ImGui::TreeNode("Light"))
                          {
                              auto& t = m_lightEntity.GetTransform();

                              ImGui::DragFloat3("Position", &t.position.x, 0.1f);

                              Vec3 euler = t.quaternion.ToEuler() * RAD2DEG;
                              if (ImGui::DragFloat3("Rotation", &euler.x, 0.1f))
                              {
                                  t.quaternion = EulerToQuat(euler * DEG2RAD);
                              }

                              if (ImGui::Button("Position Reset"))
                                  t.position = Vec3::Zero;

                              if (ImGui::Button("Rotation Reset"))
                                  t.quaternion = Quat::Identity;
                              ImGui::TreePop();
                          }

                          if (ImGui::TreeNode("Entity"))
                          {
                              if (ImGui::TreeNode("Transform"))
                              {
                                  auto& t = m_sphereEntity.GetTransform();
                                  ImGui::DragFloat3("Position", &t.position.x, 0.1f);

                                  Vec3 euler = t.quaternion.ToEuler() * RAD2DEG;
                                  if (ImGui::DragFloat3("Rotation", &euler.x, 0.1f))
                                  {
                                      t.quaternion = EulerToQuat(euler * DEG2RAD);
                                  }

                                  if (ImGui::Button("Position Reset"))
                                      t.position = Vec3::Zero;

                                  if (ImGui::Button("Rotation Reset"))
                                      t.quaternion = Quat::Identity;

                                  ImGui::TreePop();
                              }

                              if (ImGui::TreeNode("Material"))
                              {
                                  auto& mat = m_sphereEntity.GetComponent<MaterialComponent>();

                                  ImGui::ColorEdit3("Albedo", &mat.albedo.x);
                                  ImGui::DragFloat("Metallic", &mat.metallic, 0.01f, 0.f, 1.f);
                                  ImGui::DragFloat("Roughness", &mat.roughness, 0.01f, 0.f, 1.f);
                                  ImGui::TreePop();
                              }

                              if (ImGui::TreeNode("Texture"))
                              {
                                  ImGui::Checkbox("Use Height Map", &m_useHeightMap);
                                  if (m_useHeightMap)
                                      ImGui::DragFloat("Height Scale", &m_heightScale, 0.01f, 0.f, 5.f);

                                  ImGui::Checkbox("Use Normal Map", &m_useNormalMap);
                                  ImGui::Checkbox("Use Albedo Map", &m_useAlbedoMap);
                                  ImGui::Checkbox("Use Roughness Map", &m_useRoughnessMap);
                                  ImGui::Checkbox("Use Metallic Map", &m_useMetallicMap);
                                  ImGui::Checkbox("Use AO Map", &m_useAOMap);

                                  ImGui::TreePop();
                              }

                              ImGui::TreePop();
                          }
                      });

    ImGui::Begin("HDRI", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

    if (ImGui::SliderFloat("Exposure", &m_hdriExposure, 0.1f, 10.f))
        m_hdriDirty = true;

    if (ImGui::SliderFloat("Gamma", &m_hdriGamma, 0.1f, 10.f))
        m_hdriDirty = true;

    if (ImGui::DragFloat("Blur Strength", &m_blurStrength, 0.01f, 0.f, 1.f))
        m_blurDirty = true;

    constexpr uint32 BLUR_STEP = 1;
    if (ImGui::InputScalar("Blur Count", ImGuiDataType_U32, &m_blurCount, &BLUR_STEP))
        m_needPostProcessRebuild = true;

    ImGui::End();

    ImGui::Begin("Model Entity", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

    {
        auto& t = m_modelEntity.GetTransform();
        ImGui::DragFloat3("Position", &t.position.x, 0.1f);

        Vec3 euler = t.quaternion.ToEuler() * RAD2DEG;
        if (ImGui::DragFloat3("Rotation", &euler.x, 0.1f))
        {
            t.quaternion = EulerToQuat(euler * DEG2RAD);
        }

        ImGui::DragFloat3("Scale", &t.scale.x, 0.1f);

        if (ImGui::Button("Position Reset"))
            t.position = Vec3::Zero;

        if (ImGui::Button("Rotation Reset"))
            t.quaternion = Quat::Identity;

        if (ImGui::Button("Scale Reset"))
            t.scale = Vec3::One;
    }

    ImGui::End();
}

void PBRDemo::OnEvent(CrabEvent& in_event)
{
}

void PBRDemo::_BuildPostProcess() const
{
    m_postProcess->ClearFilterList();

    auto [width, height] = GetAppWindow().GetWindowSize();

    auto ppVS          = VertexShader::CreateFromFile("PostProcessVS.hlsl", "main", PostProcessVertex::s_inputElements);
    auto samplingPS    = PixelShader::CreateFromFile("SamplingPostProcessPS.hlsl", "main");
    auto tonemappingPS = PixelShader::CreateFromFile("ToneMapPS.hlsl", "main");
    auto blurXPS       = PixelShader::CreateFromFile("BlurXPS.hlsl", "main");
    auto blurYPS       = PixelShader::CreateFromFile("BlurYPS.hlsl", "main");
    auto combindPS     = PixelShader::CreateFromFile("BlurCombinePS.hlsl", "main");
    auto commonSampler = m_wrapSampler;

    m_cbBlur->UpdateData(CB_Blur { 1.f / width, 1.f / height });

    Ref<ImageFilter> samplingFilter1 = ImageFilter::Create(0, 0, ppVS, samplingPS);
    samplingFilter1->AddInputSampler(commonSampler, 0);
    samplingFilter1->AddInputTexture(D11_API->GetSwapChain()->GetFloatBackBufferResolveTex(), 0);
    samplingFilter1->SetOnBindCallback([]()
                                       { D11_API->GetSwapChain()->ResolveFloatBackBufferTex(); });
    m_postProcess->AddFilter(samplingFilter1);

    for (int i = 0; i < m_blurCount; ++i)
    {
        float w = static_cast<float>(width / std::pow(2, i + 1));
        float h = static_cast<float>(height / std::pow(2, i + 1));

        {
            Ref<ImageFilter> downsampleFilter = ImageFilter::Create(w, h, ppVS, blurXPS);
            downsampleFilter->AddInputSampler(commonSampler, 0);
            downsampleFilter->AddInputTexture(m_postProcess->GetLastFilter()->GetOutputTexture(), 0);
            downsampleFilter->AddConstantBuffer(m_cbBlur, 0);
            m_postProcess->AddFilter(downsampleFilter);
        }

        {
            Ref<ImageFilter> downsampleFilter = ImageFilter::Create(w, h, ppVS, blurYPS);
            downsampleFilter->AddInputSampler(commonSampler, 0);
            downsampleFilter->AddInputTexture(m_postProcess->GetLastFilter()->GetOutputTexture(), 0);
            downsampleFilter->AddConstantBuffer(m_cbBlur, 0);
            m_postProcess->AddFilter(downsampleFilter);
        }
    }

    for (int i = 0; i < m_blurCount; ++i)
    {
        float w = static_cast<float>(width / std::pow(2, m_blurCount - 1 - i));
        float h = static_cast<float>(height / std::pow(2, m_blurCount - 1 - i));

        {
            Ref<ImageFilter> upsampleFilter = ImageFilter::Create(w, h, ppVS, blurXPS);
            upsampleFilter->AddInputSampler(commonSampler, 0);
            upsampleFilter->AddInputTexture(m_postProcess->GetLastFilter()->GetOutputTexture(), 0);
            upsampleFilter->AddConstantBuffer(m_cbBlur, 0);
            m_postProcess->AddFilter(upsampleFilter);
        }
        {
            Ref<ImageFilter> upsampleFilter = ImageFilter::Create(w, h, ppVS, blurYPS);
            upsampleFilter->AddInputSampler(commonSampler, 0);
            upsampleFilter->AddInputTexture(m_postProcess->GetLastFilter()->GetOutputTexture(), 0);
            upsampleFilter->AddConstantBuffer(m_cbBlur, 0);
            m_postProcess->AddFilter(upsampleFilter);
        }
    }

    Ref<ImageFilter> combindFilter = ImageFilter::Create(0, 0, ppVS, combindPS);
    combindFilter->AddInputSampler(commonSampler, 0);
    combindFilter->AddInputTexture(m_postProcess->GetLastFilter()->GetOutputTexture(), 0);
    combindFilter->AddInputTexture(D11_API->GetSwapChain()->GetFloatBackBufferResolveTex(), 1);
    combindFilter->AddConstantBuffer(m_cbBlurCombine, 0);
    m_postProcess->AddFilter(combindFilter);

    Ref<ImageFilter> tonemappingFilter = ImageFilter::Create(0, 0, ppVS, tonemappingPS);
    tonemappingFilter->AddInputSampler(commonSampler, 0);
    tonemappingFilter->AddConstantBuffer(m_cbHDRI, 0);
    tonemappingFilter->SetRenderTarget(D11_API->GetBackBuffer());
    tonemappingFilter->AddInputTexture(m_postProcess->GetLastFilter()->GetOutputTexture(), 0);
    m_postProcess->AddFilter(tonemappingFilter);
}
