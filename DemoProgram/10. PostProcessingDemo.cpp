#include "pch.h"

#include "10. PostProcessingDemo.h"

#include "D11_Model.h"
#include "GUIs.h"
#include "PostProcess.h"

void PostProcessingDemo::Init()
{
    auto* dx = D11_API;

    // Shader
    {
        m_commonVS = D11_VertexShader::CreateFromFile("LightVertexShader.hlsl", "VSmain", Vertex::s_inputElements);
        m_IBLPS    = D11_PixelShader::CreateFromFile("SimpleIBLPixelShader.hlsl", "main");
        m_normalVS = D11_VertexShader::CreateFromFile("NormalVertexShader.hlsl", "main", Vertex::s_inputElements);
        m_normalPS = D11_PixelShader::CreateFromFile("NormalPixelShader.hlsl", "main");
    }

    // Constant Buffer
    {
        m_cbObject      = D11_ConstantBuffer<CB_Object>::Create(0, eShaderFlags_VertexShader | eShaderFlags_PixelShader);
        m_cbCamera      = D11_ConstantBuffer<CB_Camera>::Create(1, eShaderFlags_VertexShader | eShaderFlags_PixelShader);
        m_cbPixelShader = D11_ConstantBuffer<CB_PixelShader>::Create(2, eShaderFlags_PixelShader);
    }

    // Depth Stencil State
    m_dsState = D11_DepthStencilState::Create(true, true, false);

    auto samplerState = D11_SamplerState::Create(eSamplerFilter::Linear, eSamplerAddress::Wrap);

    // Cube map load
    m_cubemapSpecular = D11_Texture::CreateTextureCubeByDDS(
        "Resources\\cubemap\\5\\specular.dds",
        samplerState);

    m_cubemapDiffuse = D11_Texture::CreateTextureCubeByDDS(
        "Resources\\cubemap\\5\\diffuse.dds",
        samplerState);

    m_cubemapTextureArray = D11_TextureArray({ m_cubemapDiffuse, m_cubemapSpecular });

    // Model Entity
    {
        m_modelEntity = CreateEntity();

        // Model Renderer
        D11_ModelLoader loader;
        loader.Load(R"(Resources\car\EBRC Jaguar Infantry Fighting Vehicle.obj)");
        auto modelVec = loader.GetMeshDatas();

        auto& mr                = m_modelEntity.AddComponent<ModelRenderer>();
        mr.model                = loader.CreateModel();
        mr.model->m_textures[0] = D11_Texture::Create("Resources\\car\\RGB_eec4b2d76bf544ba8425124e0137edb4_carro.jpg", samplerState);
        mr.pixelShader          = m_IBLPS;
        mr.vertexShader         = m_commonVS;
        mr.topology             = eTopology::TriangleList;

        // Transform
        Transform& t = m_modelEntity.GetComponent<Transform>();
        t.position   = Vec3(0.f, -4.f, 10.f);
        t.scale      = Vec3(2.f, 2.f, 2.f);
        t.rotation.x = 90.f * DEG2RAD;

        // Normal Renderer
        auto& nr        = m_modelEntity.AddComponent<NormalRenderer>();
        nr.mesh         = MeshFactory3D::CreateNormalLines(modelVec);
        nr.vertexShader = m_normalVS;
        nr.pixelShader  = m_normalPS;

        // Material Component
        auto& mc              = m_modelEntity.AddComponent<MaterialComponent>();
        mc.material.ambient   = Vec4(0.3f, 0.3f, 0.3f, 1.f);
        mc.material.diffuse   = Vec4(0.8f, 0.8f, .8f, 1.f);
        mc.material.specular  = Vec4(1.f, 1.f, 1.f, 1.f);
        mc.material.shininess = 32.f;
        mc.useTexture         = true;
    }

    // Camera Entity
    {
        m_cameraEntity = CreateEntity();

        CameraComponent cmr;
        cmr.aspect         = GetApplication().GetAppWindow().GetAspect();
        cmr.fov            = 45.f * DEG2RAD;
        cmr.nearZ      = 0.1f;
        cmr.farZ       = 200.f;
        cmr.projectionType = eProjectionType::Perspective;
        m_cameraEntity.AddComponent<CameraComponent>(cmr);

        Transform& cmrTrans = m_cameraEntity.GetComponent<Transform>();
        cmrTrans.position   = Vec3(0.f, 1.f, -5.f);
    }

    // CubeMap Entity
    {
        Entity e = CreateEntity();

        auto& mr        = e.AddComponent<MeshRenderer>();
        mr.mesh         = MeshFactory3D::CreateCubeMap({ 150.f, 150.f, 150.f });
        mr.vertexShader = m_commonVS;
        mr.pixelShader  = D11_PixelShader::CreateFromFile("CubeMapPixelShader.hlsl", "main");
        auto& mat       = e.AddComponent<MaterialComponent>();
        mat.material.textureArray.Add(m_cubemapSpecular);
        mat.material.textureStartSlot = 0;
        mat.useTexture                = true;
    }

    // Post processing
    {
        auto [width, height] = GetApplication().GetAppWindow().GetWindowSize();

        // Blur
        PostProcessSetting ppSetting = {};
        ppSetting.vertexShader       = D11_VertexShader::CreateFromFile("PostProcessingVertexShader.hlsl", "main", PostProcessingVertex::s_inputElements);

        auto samplingShader      = D11_PixelShader::CreateFromFile("SamplingPixelShader.hlsl", "main");
        auto gaussianBlurXShader = D11_PixelShader::CreateFromFile("GaussianBlurXPixelShader.hlsl", "main");
        auto gaussianBlurYShader = D11_PixelShader::CreateFromFile("GaussianBlurYPixelShader.hlsl", "main");
        auto bloomShader         = D11_PixelShader::CreateFromFile("BloomPixelShader.hlsl", "main");
        auto combineShader       = D11_PixelShader::CreateFromFile("CombinePixelShader.hlsl", "main");

        ppSetting.pixelShader        = samplingShader;
        ppSetting.renderTargetWidth  = width / 8;
        ppSetting.renderTargetHeight = height / 8;
        auto downSamplingFilter      = PostProcess::Create(ppSetting);
        auto backBufferTex           = dx->GetSwapChain()->GetBackBufferTexture();

        downSamplingFilter->SetTexture(backBufferTex);
        m_postProcessStack.push_back(downSamplingFilter);

        ppSetting.pixelShader = bloomShader;
        auto bloomFilter      = PostProcess::Create(ppSetting);
        bloomFilter->SetTexture(m_postProcessStack.back()->GetTexture());
        m_postProcessStack.push_back(bloomFilter);

        for (int i = 0; i < 5; i++)
        {
            ppSetting.pixelShader = gaussianBlurXShader;
            auto blurXFilter      = PostProcess::Create(ppSetting);
            blurXFilter->SetTexture(m_postProcessStack.back()->GetTexture());
            m_postProcessStack.push_back(blurXFilter);

            ppSetting.pixelShader = gaussianBlurYShader;
            auto blurYFilter      = PostProcess::Create(ppSetting);
            blurYFilter->SetTexture(m_postProcessStack.back()->GetTexture());
            m_postProcessStack.push_back(blurYFilter);
        }
        ppSetting.renderTargetWidth  = 0;
        ppSetting.renderTargetHeight = 0;
        ppSetting.pixelShader        = combineShader;
        auto combineFilter           = PostProcess::Create(ppSetting);

        combineFilter->SetTextures({ m_postProcessStack.back()->GetTexture(),
                                     backBufferTex });
        m_postProcessStack.push_back(combineFilter);

        ppSetting.pixelShader = samplingShader;
        auto finalFilter      = PostProcess::Create(ppSetting);
        finalFilter->SetTexture(m_postProcessStack.back()->GetTexture());
        finalFilter->SetRenderTarget(dx->GetSwapChain()->GetBackBuffer());
        m_postProcessStack.push_back(finalFilter);
    }
}

void PostProcessingDemo::OnEnter()
{
    _CreateRasterizerState();
}

void PostProcessingDemo::OnExit()
{
}

void PostProcessingDemo::OnUpdate(TimeStamp& in_ts)
{
    m_modelEntity.GetComponent<Transform>().rotation.y += 0.05f * PI * in_ts.deltaTime;

    if (Input::IsKeyDown(eKey::A))
    {
        auto& t = m_cameraEntity.GetComponent<Transform>();
        t.position += t.Left() * m_moveSpeed * in_ts.deltaTime;
    }

    if (Input::IsKeyDown(eKey::D))
    {
        auto& t = m_cameraEntity.GetComponent<Transform>();
        t.position += t.Right() * m_moveSpeed * in_ts.deltaTime;
    }

    if (Input::IsKeyDown(eKey::W))
    {
        auto& t = m_cameraEntity.GetComponent<Transform>();
        t.position += t.Forward() * m_moveSpeed * in_ts.deltaTime;
    }

    if (Input::IsKeyDown(eKey::S))
    {
        auto& t = m_cameraEntity.GetComponent<Transform>();
        t.position += t.Backward() * m_moveSpeed * in_ts.deltaTime;
    }

    if (Input::IsKeyDown(eKey::E))
    {
        auto& t = m_cameraEntity.GetComponent<Transform>();
        t.position += t.Up() * m_moveSpeed * in_ts.deltaTime;
    }

    if (Input::IsKeyDown(eKey::Q))
    {
        auto& t = m_cameraEntity.GetComponent<Transform>();
        t.position += t.Down() * m_moveSpeed * in_ts.deltaTime;
    }

    if (Input::IsKeyDown(eKey::Space) && Input::IsMouseDown(eMouse::Left))
    {
        auto [dx, dy] = Input::GetMouseDeltaPos();
        auto& t       = m_cameraEntity.GetComponent<Transform>();

        t.rotation.y += dx * m_rotSpeed * 0.01f;
        t.rotation.x += dy * m_rotSpeed * 0.01f;
    }
}
void PostProcessingDemo::OnRender(TimeStamp& in_ts)
{
    auto* dx = D11_API;
    dx->SetBackBufferToRenderTarget();
    dx->ClearBackBuffer(color::GRAY, true, false);
    m_cubemapTextureArray.Bind(1, eShaderFlags_PixelShader);

    CB_PixelShader pixelShaderData      = {};
    pixelShaderData.diffuseMapStrength  = m_diffuseMapStrength;
    pixelShaderData.specularMapStrength = m_specularMapStrength;
    pixelShaderData.useSmoothStep       = m_useSmoothStep;
    pixelShaderData.speculerShininess   = m_specularShininess;
    m_cbPixelShader->Bind();
    m_cbPixelShader->UpdateData(pixelShaderData);

    GetView<CameraComponent, Transform>().each(
        [&](CameraComponent& in_cmr,
            Transform&       in_trans)
        {
            CB_Camera data;
            data.eyePos   = in_trans.position;
            data.viewProj = in_cmr.GetViewProj(in_trans);

            m_cbCamera->Bind();
            m_cbCamera->UpdateData(data);
        });

    GetView<ModelRenderer, Transform, MaterialComponent>().each(
        [&](ModelRenderer&     in_mr,
            Transform&         in_trans,
            MaterialComponent& in_mat)
        {
            CB_Object data;
            data.world              = in_trans.GetWorld();
            data.worldInvTranspose  = in_trans.GetWorldInvTranspose();
            data.useTexture         = in_mat.useTexture;
            data.material.ambient   = in_mat.material.ambient;
            data.material.diffuse   = in_mat.material.diffuse;
            data.material.specular  = in_mat.material.specular;
            data.material.shininess = in_mat.material.shininess;
            m_cbObject->Bind();
            m_cbObject->UpdateData(data);

            dx->SetTopology(in_mr.topology);
            in_mr.vertexShader->Bind();
            in_mr.pixelShader->Bind();
            in_mr.model->Draw(in_mat.useTexture);
        });

    GetView<MeshRenderer, Transform, MaterialComponent>().each(
        [&](MeshRenderer&      in_mesh,
            Transform&         in_trans,
            MaterialComponent& in_mat)
        {
            CB_Object data;
            data.world              = in_trans.GetWorld();
            data.worldInvTranspose  = in_trans.GetWorldInvTranspose();
            data.useTexture         = in_mat.useTexture;
            data.material.ambient   = in_mat.material.ambient;
            data.material.diffuse   = in_mat.material.diffuse;
            data.material.specular  = in_mat.material.specular;
            data.material.shininess = in_mat.material.shininess;
            m_cbObject->UpdateData(data);

            dx->SetTopology(in_mesh.topology);

            in_mesh.vertexShader->Bind();
            in_mesh.pixelShader->Bind();

            in_mat.material.textureArray.Bind(in_mat.material.textureStartSlot, eShaderFlags_PixelShader);

            in_mesh.mesh->Draw();
        });

    GetView<NormalRenderer, Transform>().each(
        [&](NormalRenderer& in_normal,
            Transform&      in_trans)
        {
            if (!m_showNormal)
                return;

            CB_Object data;
            data.world             = in_trans.GetWorld();
            data.worldInvTranspose = in_trans.GetWorldInvTranspose();
            m_cbObject->UpdateData(data);

            dx->SetTopology(eTopology::LineList);
            in_normal.vertexShader->Bind();
            in_normal.pixelShader->Bind();
            in_normal.mesh->Draw();
        });

    if (m_ppConstantDirty)
    {
        for (auto& pp: m_postProcessStack)
            pp->UpdateConstantData(m_threshold, m_strength);

        m_ppConstantDirty = false;
    }

    if (m_eneblePostProcessing)
    {
        for (auto& pp: m_postProcessStack)
        {
            pp->Render();
        }
    }
}

void PostProcessingDemo::OnRenderGUI(TimeStamp& in_ts)
{
    DrawDemoInspector(GetName(),
                      [&]()
                      {
                          ImGui::SeparatorText("PostProcessing Value");
                          if (ImGui::DragFloat("Strength", &m_strength, 0.001f, 0.f, 1.f, "%.3f"))
                              m_ppConstantDirty = true;

                          if (ImGui::DragFloat("Threshold", &m_threshold, 0.001f, 0.f, 1.f, "%.3f"))
                              m_ppConstantDirty = true;

                          ImGui::TextUnformatted("Blue Effect Demo");
                          ImGui::Checkbox("Enable Post Processing", &m_eneblePostProcessing);

                          ImGui::SeparatorText("Camera");
                          DrawCameraInspector(m_cameraEntity.GetComponent<Transform>(),
                                              m_cameraEntity.GetComponent<CameraComponent>());

                          ImGui::DragFloat("Move Speed", &m_moveSpeed, 0.01f, 0.01f);
                          ImGui::DragFloat("Rotation Speed", &m_rotSpeed, 0.005f, 0.01f);

                          ImGui::SeparatorText("Rasterizer State");

                          if (ImGui::Checkbox("Wireframe", &m_wireframe))
                              _CreateRasterizerState();

                          ImGui::SameLine();

                          if (ImGui::Checkbox("Cull Enable", &m_cullenable))
                              _CreateRasterizerState();

                          // Shader
                          ImGui::SeparatorText("Pixel Shader");

                          // radio

                          ImGui::TextUnformatted("BlinnPhong Reflection Model");

                          ImGui::TextUnformatted("Pixel shader Setting");
                          ImGui::DragFloat("Diffuse Map Strength", &m_diffuseMapStrength, 0.01f, 0.f, 1.f, "%.3f");
                          ImGui::DragFloat("Specular Map Strength", &m_specularMapStrength, 0.01f, 0.f, 1.f, "%.3f");
                          ImGui::DragFloat("Specular Map Shininess", &m_specularShininess, 0.01f, 0.f);
                          ImGui::Checkbox("Use Smooth Step", &m_useSmoothStep);
                      });

    // Entity Inspector
    ImGui::Begin("Entity Inspector", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

    const auto& DrawMateriailComponentInspector = [](MaterialComponent& out_m)
    {
        ImGui::ColorEdit3("Ambient", (float*)&out_m.material.ambient);
        ImGui::ColorEdit3("Diffuse", (float*)&out_m.material.diffuse);
        ImGui::ColorEdit3("Specular", (float*)&out_m.material.specular);
        ImGui::DragFloat("Shininess", &out_m.material.shininess, 0.1f, 0.f, 0.f, "%.3f");

        ImGui::Checkbox("Use Texture", &out_m.useTexture);
    };

    ImGui::PushID("Model Entity");
    {
        ImGui::SeparatorText("Model Entity");

        ImGui::BulletText("Transform");
        DrawTransformInspector(m_modelEntity.GetComponent<Transform>());

        ImGui::BulletText("Material Component");
        DrawMateriailComponentInspector(m_modelEntity.GetComponent<MaterialComponent>());
    }
    ImGui::PopID();

    ImGui::Separator();
    ImGui::Checkbox("Show Normal", &m_showNormal);

    ImGui::End();
}

void PostProcessingDemo::OnEvent(CrabEvent& in_event)
{
}

void PostProcessingDemo::_CreateRasterizerState()
{
    m_rsState = D11_RasterizerState::Create(
        m_cullenable ? eCullMode::Back : eCullMode::None,
        m_wireframe ? eFillMode::Wireframe : eFillMode::Solid,
        eFrontFace::ClockWise);

    m_rsState->Bind();
}
