#include "pch.h"

#include "16. MipmapDemo.h"

#include "GUIs.h"

void MipmapDemo::Init()
{
    //===================================================
    //                    Texture
    //===================================================

    {
        std::filesystem::path root = R"(Resources\BasicGSDemo)";
        m_cubeTexture              = D11_Texture::CreateTextureCube(
            root / "px.png",
            root / "nx.png",
            root / "py.png",
            root / "ny.png",
            root / "pz.png",
            root / "nz.png");

        m_diffuseCubeMap  = D11_Texture::CreateTextureCubeByDDS(root / "diffuse.dds");
        m_specularCubeMap = D11_Texture::CreateTextureCubeByDDS(root / "specular.dds");

        root           = R"(Resources\MousePickingDemo)";
        m_floorTexture = D11_Texture::CreateMipmap(root / "floor.png");
    }

    //===================================================
    //                Constant Buffer
    //===================================================

    {
        m_cbObject       = D11_ConstantBuffer<CB_Object>::Create(0, eShaderFlags_VertexShader | eShaderFlags_PixelShader);
        m_cbCamera       = D11_ConstantBuffer<CB_Camera>::Create(1, eShaderFlags_VertexShader | eShaderFlags_PixelShader);
        m_cbPixelShader  = D11_ConstantBuffer<CB_PixelShader>::Create(2, eShaderFlags_PixelShader);
    }

    //===================================================
    //                    Shaders
    //===================================================

    {
        m_commonVS = D11_VertexShader::CreateFromFile(
            "MPCommonVS.hlsl", "main", Vertex::s_inputElements);

        m_cubemapPS = D11_PixelShader::CreateFromFile(
            "MPCubeMapPS.hlsl", "main");

        m_IBLPS = D11_PixelShader::CreateFromFile(
            "MipmapDemoIBLPS.hlsl", "main");

        D11_InputElements elem;
        elem.Add("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT);

        m_drawNormalVS = D11_VertexShader::CreateFromFile(
            "DrawNormalVS.hlsl", "main", Vertex::s_inputElements);

        m_drawNormalPS = D11_PixelShader::CreateFromFile(
            "DrawNormalPS.hlsl", "main");

        m_drawNormalGS = D11_GeometryShader::CreateFromFile(
            "DrawNormalGS.hlsl", "main");
    }

    //===================================================
    //                  Render State
    //===================================================

    {
        m_commonSS    = D11_SamplerState::Create(eSamplerFilter::Linear, eSamplerAddress::Wrap);
        m_wireframeRS = D11_RasterizerState::Create(eCullMode::None, eFillMode::Wireframe, eFrontFace::ClockWise);
        m_solidRS     = D11_RasterizerState::Create(eCullMode::Back, eFillMode::Solid, eFrontFace::ClockWise);

        if (m_wireframeMode)
            m_wireframeRS->Bind();
        else
            m_solidRS->Bind();
    }

    //===================================================
    //                     Entity
    //===================================================

    // cube map (sphere mapping)
    {
        Entity e = CreateEntity();

        auto& mc          = e.AddComponent<MeshComponent>();
        mc.mesh           = MeshFactory3D::CreateSphereMap(300.f, 24, 24);
        mc.topology       = eTopology::TriangleList;
        mc.vertexShader   = m_commonVS;
        mc.pixelShader    = m_cubemapPS;
        mc.geometryShader = nullptr;
        mc.constantBuffers.push_back(m_cbPixelShader);
        mc.textures.push_back(m_cubeTexture);
        mc.samplers.push_back(m_commonSS);

        auto& mat      = e.AddComponent<MaterialComponent>();
        mat.ambient    = Vec4(1.f, 1.f, 1.f, 1.f);
        mat.diffuse    = Vec4(1.f, 1.f, 1.f, 1.f);
        mat.specular   = Vec4(1.f, 1.f, 1.f, 1.f);
        mat.shininess  = 1.f;
        mat.useTexture = true;
    }

    // camera
    {
        Entity e = CreateEntity();

        auto& t      = e.GetComponent<Transform>();
        t.position.x = 40.f;
        t.position.y = 2.6f;
        t.position.z = 2.3f;

        auto& cc                 = e.AddComponent<CameraControllerComponent>();
        cc.camera.aspect         = GetApplication().GetAppWindow().GetAspect();
        cc.camera.fov            = 45.f * DEG2RAD;
        cc.camera.nearZ          = 0.1f;
        cc.camera.farZ           = 500.f;
        cc.camera.projectionType = eProjectionType::Perspective;
        cc.moveSpeed             = 5.f;
        cc.rotSpeed              = 10.f * DEG2RAD;

        m_camera = e;
    }

    MeshComponent commonMC;   // Common Mesh Component
    commonMC.topology       = eTopology::TriangleList;
    commonMC.vertexShader   = m_commonVS;
    commonMC.pixelShader    = m_IBLPS;
    commonMC.geometryShader = nullptr;
    commonMC.constantBuffers.push_back(m_cbPixelShader);
    commonMC.samplers.push_back(m_commonSS);
    commonMC.textures.push_back(m_diffuseCubeMap);
    commonMC.textures.push_back(m_specularCubeMap);

    // floor
    {
        std::vector<Vertex> vertices;
        Vertex              v1 = {};
        v1.position            = Vec3(-50.f, 0.f, 50.f);
        v1.normal              = Vec3(0.f, 1.f, 0.f);
        v1.texCoord            = Vec2(0.f, 0.f);
        vertices.push_back(v1);

        Vertex v2 = {};
        v2.position = Vec3(50.f, 0.f, 50.f);
        v2.normal   = Vec3(0.f, 1.f, 0.f);
        v2.texCoord = Vec2(50.f, 0.f);
        vertices.push_back(v2);

        Vertex v3 = {};
        v3.position = Vec3(50.f, 0.f, -50.f);
        v3.normal   = Vec3(0.f, 1.f, 0.f);
        v3.texCoord = Vec2(50.f, 50.f);
        vertices.push_back(v3);

        Vertex v4 = {};
        v4.position = Vec3(-50.f, 0.f, -50.f);
        v4.normal   = Vec3(0.f, 1.f, 0.f);
        v4.texCoord = Vec2(0.f, 50.f);
        vertices.push_back(v4);

        std::vector<uint32> indices = { 0, 1, 2, 0,2,3 };

        Entity e  = CreateEntity();
        auto&  mc = e.AddComponent<MeshComponent>();
        mc        = commonMC;
        mc.mesh   = D11_Mesh::Create(vertices, indices);
        mc.textures.push_back(m_floorTexture);

        auto& mat      = e.AddComponent<MaterialComponent>();
        mat.ambient    = Vec4(1.f, 1.f, 1.f, 1.f);
        mat.diffuse    = Vec4(1.f, 1.f, 1.f, 1.f);
        mat.specular   = Vec4(1.f, 1.f, 1.f, 1.f);
        mat.shininess  = 1.f;
        mat.useTexture = true;

        auto& t      = e.GetComponent<Transform>();
        t.position.x = -12.5f;
        t.position.z = -12.5f;

        e.AddComponent<TagComponent>("Floor");
    }

    // Primitive
    {
        // Sphere
        {
            Entity e   = CreateEntity();
            auto&  t   = e.GetComponent<Transform>();
            t.position = Vec3(10.f, 7.f, 20.f);

            auto& mc = e.AddComponent<MeshComponent>();
            mc       = commonMC;
            mc.mesh  = MeshFactory3D::CreateSphere(1.f, 32, 32);

            auto& mat      = e.AddComponent<MaterialComponent>();
            mat.ambient    = Vec4(1.f, 1.f, 1.f, 1.f);
            mat.diffuse    = Vec4(0.8f, 0.1f, 0.1f, 1.f);
            mat.specular   = Vec4(1.f, 1.f, 1.f, 1.f);
            mat.shininess  = 1.f;
            mat.useTexture = false;
            e.AddComponent<DrawNormalComponent>();
        }

        // Cube
        {
            Entity e   = CreateEntity();
            auto&  t   = e.GetComponent<Transform>();
            t.position = Vec3(20.f, 7.f, 20.f);

            auto& mc = e.AddComponent<MeshComponent>();
            mc       = commonMC;
            mc.mesh  = MeshFactory3D::CreateCube(Vec3 { 2.f, 2.f, 2.f });

            auto& mat      = e.AddComponent<MaterialComponent>();
            mat.ambient    = Vec4(1.f, 1.f, 1.f, 1.f);
            mat.diffuse    = Vec4(0.1f, 0.1f, 0.8f, 1.f);
            mat.specular   = Vec4(1.f, 1.f, 1.f, 1.f);
            mat.shininess  = 1.f;
            mat.useTexture = false;
            e.AddComponent<DrawNormalComponent>();
        }

        // earth
        {
            Entity e   = CreateEntity();
            auto&  t   = e.GetComponent<Transform>();
            t.position = Vec3(20.f, 12.f, 35.f);

            auto& mc = e.AddComponent<MeshComponent>();
            mc       = commonMC;
            mc.mesh  = MeshFactory3D::CreateSphere(5.f, 48, 48);
            mc.textures.push_back(D11_Texture::CreateMipmap("Resources\\TrackballDemo\\earthDemo.jpg"));

            auto& mat      = e.AddComponent<MaterialComponent>();
            mat.ambient    = Vec4(1.f, 1.f, 1.f, 1.f);
            mat.diffuse    = Vec4(1.f, 1.f, 1.f, 1.f);
            mat.specular   = Vec4(0.7f, 0.7f, 0.7f, 1.f);
            mat.shininess  = 2.f;
            mat.useTexture = true;
            e.AddComponent<DrawNormalComponent>();
        }

    }

    //===================================================
    //                  Field Init
    //===================================================
    {
        m_cbPixelShaderData.diffuseMapStrength  = 0.4f;
        m_cbPixelShaderData.specularMapStrength = 0.85f;
        m_cbPixelShaderData.speculerShininess   = 1.2f;
        m_cbPixelShaderData.useSmoothStep       = true;
    }
}

void MipmapDemo::OnEnter()
{
}

void MipmapDemo::OnExit()
{
    GetAppWindow().EnableMouseRelativeMode(false);
}

void MipmapDemo::OnUpdate(TimeStamp& in_ts)
{
    static int s_mousePosX = 0;
    static int s_mousePosY = 0;

    if (Input::IsKeyPressed(eKey::Space))
    {
        GetAppWindow().EnableMouseRelativeMode(true);
        m_rotateMode = true;

        auto [x, y] = Input::GetMousePos();
        s_mousePosX = x;
        s_mousePosY = y;
    }

    if (Input::IsKeyReleased(eKey::Space))
    {
        GetAppWindow().EnableMouseRelativeMode(false);
        m_rotateMode = false;

        Input::SetMousePos(s_mousePosX, s_mousePosY);
    }

    if (m_rotateMode)
    {
        auto [t, cmr] = m_camera.GetComponents<Transform, CameraControllerComponent>();
        auto [dx, dy] = Input::GetMouseRelativeDeltaPos();

        bool cond = (dx == 0 && dy == 0);
        if (!cond)
        {
            Vec3 euler = t.quaternion.ToEuler();
            euler.x += dy * cmr.rotSpeed * in_ts.deltaTime;
            euler.y += dx * cmr.rotSpeed * in_ts.deltaTime;
            t.quaternion = QuatFromEuler(euler);
        }
    }

    // Camera Movement
    {
        auto [t, cmr] = m_camera.GetComponents<Transform, CameraControllerComponent>();

        if (Input::IsKeyDown(eKey::W))
            t.position += t.Forward() * cmr.moveSpeed * in_ts.deltaTime;

        if (Input::IsKeyDown(eKey::S))
            t.position += t.Backward() * cmr.moveSpeed * in_ts.deltaTime;

        if (Input::IsKeyDown(eKey::A))
            t.position += t.Left() * cmr.moveSpeed * in_ts.deltaTime;

        if (Input::IsKeyDown(eKey::D))
            t.position += t.Right() * cmr.moveSpeed * in_ts.deltaTime;

        if (Input::IsKeyDown(eKey::Q))
            t.position += t.Down() * cmr.moveSpeed * in_ts.deltaTime;

        if (Input::IsKeyDown(eKey::E))
            t.position += t.Up() * cmr.moveSpeed * in_ts.deltaTime;

        if (Input::IsKeyPressed(eKey::Escape))
        {
            AppClose_CoreEvent e;
            GetApplication().DispatchEvent(e);
        }
    }

    // Post Processing
    {
        if (m_ppStackDirty)
        {
            _BuildPostProcessData();
            m_ppStackDirty = false;
        }
    }
}

void MipmapDemo::OnRender(TimeStamp& in_ts)
{
    auto* dx = D11_API;
    dx->SetBackBufferToRenderTarget();
    dx->ClearBackBuffer(color::BLACK);
    m_commonSS->Bind(0, eShaderFlags_PixelShader);
    m_commonSS->Bind(1, eShaderFlags_PixelShader);
    m_commonSS->Bind(2, eShaderFlags_PixelShader);

    m_cbCamera->Bind();
    m_cbObject->Bind();

    GetView<CameraControllerComponent, Transform>().each(
        [&](CameraControllerComponent& in_cc,
            Transform&                 in_t)
        {
            CB_Camera cmr;
            cmr.eyePos   = in_t.position;
            cmr.viewProj = in_cc.camera.GetViewProj(in_t);
            m_cbCamera->UpdateData(cmr);
        });

    GetView<MeshComponent, Transform, MaterialComponent>().each(
        [&](MeshComponent&     in_mc,
            Transform&         in_t,
            MaterialComponent& in_mat)
        {
            CB_Object data;
            data.world             = in_t.GetWorld();
            data.worldInvTranspose = in_t.GetWorldInvTranspose();
            data.ambient           = in_mat.ambient;
            data.diffuse           = in_mat.diffuse;
            data.specular          = in_mat.specular;
            data.shininess         = in_mat.shininess;
            data.useTexture        = in_mat.useTexture;
            m_cbObject->UpdateData(data);

            for (const auto& cBuffer: in_mc.constantBuffers)
                cBuffer->Bind();

            for (uint32 i = 0; i < in_mc.textures.size(); ++i)
                in_mc.textures[i]->Bind(i, eShaderFlags_PixelShader);

            for (uint32 i = 0; i < in_mc.samplers.size(); ++i)
                in_mc.samplers[i]->Bind(i, eShaderFlags_PixelShader);

            dx->SetTopology(in_mc.topology);
            in_mc.vertexShader->Bind();
            in_mc.pixelShader->Bind();

            if (in_mc.geometryShader)
                in_mc.geometryShader->Bind();
            else
                dx->SetGeometryShader(nullptr);

            in_mc.mesh->Draw();
        });

    if (m_drawNormal)
    {
        GetView<MeshComponent, Transform, DrawNormalComponent>().each(
            [&](MeshComponent&       in_mc,
                Transform&           in_t,
                DrawNormalComponent& in_dnc)
            {
                CB_Object data;
                data.world             = in_t.GetWorld();
                data.worldInvTranspose = in_t.GetWorldInvTranspose();
                m_cbObject->UpdateData(data);

                for (const auto& cBuffer: in_mc.constantBuffers)
                    cBuffer->Bind();

                dx->SetTopology(eTopology::TriangleList);
                m_drawNormalVS->Bind();
                m_drawNormalPS->Bind();
                m_drawNormalGS->Bind();

                in_mc.mesh->Draw();
            });
    }

    if (m_cbPixelShaderDirty)
    {
        m_cbPixelShader->UpdateData(m_cbPixelShaderData);
        m_cbPixelShaderDirty = false;
    }

    // Post Processing
    if (m_enablePostProcessing)
    {
        for (auto& pp: m_ppStack)
            pp->Render();
    }
}

void MipmapDemo::OnRenderGUI(TimeStamp& in_ts)
{
    DrawDemoInspector(GetName(), [&]()
                      { _DrawGUI(); });

    ImGui::Begin("Post Processing", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::Checkbox("Enable Post Processing", &m_enablePostProcessing);

    ImGui::SeparatorText("Anti-Aliasing");
    ImGui::Text("Anti-Aliasing based on FXAA");

    if (ImGui::Checkbox("Enable Anti-Aliasing", &m_enableAntiAliasing))
        m_ppStackDirty = true;

    static uint32 s_uintStep = 1.f;
    if (ImGui::InputScalar("Quality", ImGuiDataType_S32, &m_fxaaQuality, &s_uintStep))
    {
        Clamp(m_fxaaQuality, 1, 16);
        m_ppStackDirty = true;
    }

    ImGui::SeparatorText("Bloom");
    ImGui::Text("Bloom based on Gaussian Blur");

    if (ImGui::Checkbox("Enable Bloom", &m_enableBloom))
        m_ppStackDirty = true;

    if (ImGui::DragFloat("Threshold", &m_bloomThreshold, 0.01f, 0.f, 1.f))
    {
        if (m_thresholdPP)
            m_thresholdPP->UpdateConstantData(m_bloomThreshold, 1.f);
    }

    if (ImGui::DragInt("Blur Strength", &m_blurStrength, 1, 1, 10))
        m_ppStackDirty = true;

    if (ImGui::DragFloat("Blur Blend Factor", &m_blurBlendFactor, 0.01f, 0.f, 1.f))
    {
        if (m_combinePP)
            m_combinePP->UpdateConstantData(0.f, m_blurBlendFactor);
    }

    ImGui::End();
}

void MipmapDemo::OnEvent(CrabEvent& in_event)
{
}

void MipmapDemo::_DrawGUI()
{
    ImGui::SeparatorText("Camera");

    auto [cmr, trans] = m_camera.GetComponents<CameraControllerComponent, Transform>();
    ImGui::Text("Control mode: %s", m_rotateMode ? "Rotate" : "Default");
    ImGui::DragFloat("Move Speed", &cmr.moveSpeed, 0.01f, 0.01f);
    ImGui::DragFloat("Rotation Speed", &cmr.rotSpeed, 0.005f, 0.01f);
    ImGui::Text("Position: %.2f, %.2f, %.2f",
                trans.position.x,
                trans.position.y,
                trans.position.z);

    if (ImGui::Button("Reset Position"))
        trans.position = Vec3::Zero;

    Vec3 euler = trans.quaternion.ToEuler() * RAD2DEG;
    ImGui::Text("Pitch: %.2f deg", euler.x);
    ImGui::Text("Yaw: %.2f deg", euler.y);
    ImGui::Text("Roll: %.2f deg", euler.z);

    if (ImGui::Button("Reset Rotation"))
        trans.quaternion = Quat::Identity;

    static bool s_nearFarZDirty = false;

    if (ImGui::DragFloat("NearZ", &cmr.camera.nearZ, 0.1f, 0.1f))
        s_nearFarZDirty = true;

    if (ImGui::DragFloat("FarZ", &cmr.camera.farZ, 0.1f, 0.1f))
        s_nearFarZDirty = true;

    if (s_nearFarZDirty)
    {
        Clamp(cmr.camera.farZ, cmr.camera.nearZ + 0.001f, 1000.f);
        Clamp(cmr.camera.nearZ, 0.001f, cmr.camera.farZ - 0.001f);
        s_nearFarZDirty = false;
    }

    ImGui::DragFloat("Field of view", &cmr.camera.fov, 0.01f, 0.01f, 3.14f);

    if (ImGui::DragFloat("Diffuse Map Strength", &m_cbPixelShaderData.diffuseMapStrength, 0.01f, 0.f, 1.f))
        m_cbPixelShaderDirty = true;

    if (ImGui::DragFloat("Specular Map Strength", &m_cbPixelShaderData.specularMapStrength, 0.01f, 0.f, 1.f))
        m_cbPixelShaderDirty = true;

    if (ImGui::DragFloat("Specular Shininess", &m_cbPixelShaderData.speculerShininess, 0.01f, 0.f, 100.f))
        m_cbPixelShaderDirty = true;

    static bool tempSmoothStepBool = m_cbPixelShaderData.useSmoothStep;
    if (ImGui::Checkbox("Use Smooth Step", &tempSmoothStepBool))
    {
        m_cbPixelShaderData.useSmoothStep = tempSmoothStepBool ? 1 : 0;
        m_cbPixelShaderDirty              = true;
    }

    ImGui::Checkbox("Draw Normal", &m_drawNormal);

    if (ImGui::Checkbox("Wireframe Mode", &m_wireframeMode))
    {
        if (m_wireframeMode)
            m_wireframeRS->Bind();
        else
            m_solidRS->Bind();
    }
}

void MipmapDemo::_BuildPostProcessData()
{
    m_ppStack.clear();
    m_thresholdPP.reset();
    m_combinePP.reset();

    Ref<D11_VertexShader> commonVS = D11_VertexShader::CreateFromFile(
        "PostProcessingVertexShader.hlsl", "main", PostProcess::Vertex::s_inputElements);

    Ref<D11_PixelShader> FXAAPS = D11_PixelShader::CreateFromFile(
        "FXAAPS.hlsl", "main");

    Ref<D11_PixelShader> samplingPS = D11_PixelShader::CreateFromFile(
        "SamplingPixelShader.hlsl", "main");

    auto [width, height] = GetAppWindow().GetWindowSize();

    // setting
    PostProcessSetting setting = {};
    setting.vertexShader       = commonVS;
    setting.pixelShader        = samplingPS;

    if (m_enableBloom)
    {
        // down sampling
        PostProcessSetting bownSamplingSetting = setting;
        bownSamplingSetting.renderTargetWidth  = width / 8;
        bownSamplingSetting.renderTargetHeight = height / 8;
        auto sampling                          = PostProcess::Create(bownSamplingSetting);
        sampling->SetTexture(D11_API->GetSwapChain()->GetBackBufferTexture(), 0);
        m_ppStack.push_back(sampling);

        // binary threshold
        PostProcessSetting binaryThresholdSetting = bownSamplingSetting;
        binaryThresholdSetting.pixelShader        = D11_PixelShader::CreateFromFile(
            "BloomPixelShader.hlsl", "main");
        auto binaryThreshold = PostProcess::Create(binaryThresholdSetting);
        binaryThreshold->SetTexture(m_ppStack.back()->GetTexture(), 0);
        binaryThreshold->UpdateConstantData(m_bloomThreshold, 1.f);
        m_ppStack.push_back(binaryThreshold);

        // gaussian blur
        PostProcessSetting gaussianBlurSetting = bownSamplingSetting;
        gaussianBlurSetting.pixelShader        = D11_PixelShader::CreateFromFile(
            "GaussianBlurPixelShader.hlsl", "main");

        for (int32 idx = 0; idx < m_blurStrength; ++idx)
        {
            auto blur = PostProcess::Create(gaussianBlurSetting);
            blur->SetTexture(m_ppStack.back()->GetTexture(), 0);
            m_ppStack.push_back(blur);
        }

        // combine
        PostProcessSetting combineSetting = setting;
        combineSetting.pixelShader        = D11_PixelShader::CreateFromFile(
            "CombinePixelShader.hlsl", "main");
        auto combine = PostProcess::Create(combineSetting);
        combine->SetTexture(m_ppStack.back()->GetTexture(), 0);
        combine->SetTexture(D11_API->GetSwapChain()->GetBackBufferTexture(), 1);
        combine->UpdateConstantData(0.f, m_blurBlendFactor);
        m_ppStack.push_back(combine);

        m_thresholdPP = binaryThreshold;
        m_combinePP   = combine;
    }
    else
    {
        auto sampling = PostProcess::Create(setting);
        sampling->SetTexture(D11_API->GetSwapChain()->GetBackBufferTexture(), 0);
        m_ppStack.push_back(sampling);
    }

    // anti-aliasing
    if (m_enableAntiAliasing)
    {
        for (int i = 0; i < m_fxaaQuality; ++i)
        {
            PostProcessSetting fxaaSetting = setting;
            fxaaSetting.pixelShader        = FXAAPS;
            auto FXAA                      = PostProcess::Create(fxaaSetting);
            FXAA->SetTexture(m_ppStack.back()->GetTexture(), 0);
            m_ppStack.push_back(FXAA);
        }
    }

    // Final Sampling
    PostProcessSetting finalSetting = setting;
    finalSetting.pixelShader        = samplingPS;
    auto finalSampling              = PostProcess::Create(finalSetting);
    finalSampling->SetTexture(m_ppStack.back()->GetTexture(), 0);
    finalSampling->SetRenderTarget(D11_API->GetSwapChain()->GetBackBuffer());
    m_ppStack.push_back(finalSampling);
}
