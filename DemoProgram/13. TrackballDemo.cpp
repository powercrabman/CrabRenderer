#include "pch.h"

#include "13. TrackballDemo.h"
#include "GUIs.h"

void TrackballDemo::Init()
{
    //===================================================
    //                    Texture
    //===================================================

    {
        m_sampler = D11_SamplerState::Create(
            eSamplerFilter::Linear,
            eSamplerAddress::Clamp);

        std::filesystem::path root = R"(Resources\TrackballDemo)";
        m_cubeTexture              = D11_Texture::CreateTextureCube(
            root / "px.png",
            root / "nx.png",
            root / "py.png",
            root / "ny.png",
            root / "pz.png",
            root / "nz.png",
            m_sampler);

        m_diffuseCubeMap  = D11_Texture::CreateTextureCubeByDDS(root / "diffuse.dds", m_sampler);
        m_specularCubeMap = D11_Texture::CreateTextureCubeByDDS(root / "specular.dds", m_sampler);

        root           = R"(Resources\MousePickingDemo)";
        m_floorTexture = D11_Texture::Create(root / "floor.png", m_sampler);
    }

    //===================================================
    //                Constant Buffer
    //===================================================

    {
        m_cbObject      = D11_ConstantBuffer<CB_Object>::Create(0, eShaderFlags_VertexShader | eShaderFlags_PixelShader);
        m_cbCamera      = D11_ConstantBuffer<CB_Camera>::Create(1, eShaderFlags_VertexShader | eShaderFlags_PixelShader);
        m_cbPixelShader = D11_ConstantBuffer<CB_PixelShader>::Create(2, eShaderFlags_PixelShader);
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
            "MPIBLPS.hlsl", "main");

        m_debugPS = D11_PixelShader::CreateFromFile(
            "MPBBDebugPS.hlsl", "main");
    }

    //===================================================
    //                     Entity
    //===================================================

    // cube map (sphere mapping)
    {
        Entity e = CreateEntity();

        auto& mc        = e.AddComponent<MeshComponent>();
        mc.mesh         = MeshFactory3D::CreateSphereMap(300.f, 24, 24);
        mc.vertexShader = m_commonVS;
        mc.pixelShader  = m_cubemapPS;
        mc.textureArray.Add(m_cubeTexture);
        mc.useTexture = true;
        mc.topology   = eTopology::TriangleList;

        auto& mat     = e.AddComponent<MaterialComponent>();
        mat.ambient   = Vec4(1.f, 1.f, 1.f, 1.f);
        mat.diffuse   = Vec4(1.f, 1.f, 1.f, 1.f);
        mat.specular  = Vec4(1.f, 1.f, 1.f, 1.f);
        mat.shininess = 1.f;

        Log::Debug("cube map -> {0}", e.GetID().id);
    }

    // camera
    {
        Entity e = CreateEntity();

        auto& t      = e.GetComponent<Transform>();
        t.position.x = 14.5f;
        t.position.y = 8.3f;
        t.position.z = 8.6f;

        auto& cc                 = e.AddComponent<CameraControllerComponent>();
        cc.camera.aspect         = GetApplication().GetAppWindow().GetAspect();
        cc.camera.fov            = 45.f * DEG2RAD;
        cc.camera.nearZ          = 0.1f;
        cc.camera.farZ           = 500.f;
        cc.camera.projectionType = eProjectionType::Perspective;
        cc.moveSpeed             = 5.f;
        cc.rotSpeed              = 15.f * DEG2RAD;

        m_camera = e;
    }

    MeshComponent commonMC;   // Common Mesh Component
    commonMC.vertexShader = m_commonVS;
    commonMC.pixelShader  = m_IBLPS;
    commonMC.constantBuffers.push_back(m_cbObject);
    commonMC.constantBuffers.push_back(m_cbCamera);
    commonMC.constantBuffers.push_back(m_cbPixelShader);
    commonMC.topology = eTopology::TriangleList;
    commonMC.textureArray.Add(m_floorTexture);
    commonMC.textureArray.Add(m_diffuseCubeMap);
    commonMC.textureArray.Add(m_specularCubeMap);
    commonMC.useTexture = false;

    // floor
    {
        Entity e      = CreateEntity();
        auto&  mc     = e.AddComponent<MeshComponent>();
        mc            = commonMC;
        mc.mesh       = MeshFactory3D::CreateGrid(Vec2 { 100.f, 100.f }, 10, 10);
        mc.useTexture = true;

        auto& mat     = e.AddComponent<MaterialComponent>();
        mat.ambient   = Vec4(1.f, 1.f, 1.f, 1.f);
        mat.diffuse   = Vec4(1.f, 1.f, 1.f, 1.f);
        mat.specular  = Vec4(1.f, 1.f, 1.f, 1.f);
        mat.shininess = 1.f;

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

            auto& mat     = e.AddComponent<MaterialComponent>();
            mat.ambient   = Vec4(1.f, 1.f, 1.f, 1.f);
            mat.diffuse   = Vec4(0.8f, 0.1f, 0.1f, 1.f);
            mat.specular  = Vec4(1.f, 1.f, 1.f, 1.f);
            mat.shininess = 1.f;

            e.AddComponent<TagComponent>("Sphere");

            auto& bs     = e.AddComponent<BoundingShpereComponent>();
            bs.center    = Vec3::Zero;
            bs.radius    = 1.f;
            bs.debugMesh = MeshFactory3D::CreateSphere(1.f, 32, 32);
        }

        // Cube
        {
            Entity e   = CreateEntity();
            auto&  t   = e.GetComponent<Transform>();
            t.position = Vec3(20.f, 7.f, 20.f);

            auto& mc = e.AddComponent<MeshComponent>();
            mc       = commonMC;
            mc.mesh  = MeshFactory3D::CreateCube(Vec3 { 2.f, 2.f, 2.f });

            auto& mat     = e.AddComponent<MaterialComponent>();
            mat.ambient   = Vec4(1.f, 1.f, 1.f, 1.f);
            mat.diffuse   = Vec4(0.1f, 0.1f, 0.8f, 1.f);
            mat.specular  = Vec4(1.f, 1.f, 1.f, 1.f);
            mat.shininess = 1.f;

            e.AddComponent<TagComponent>("Cube");

            auto& obb     = e.AddComponent<OBBComponent>();
            obb.size      = Vec3(2.f, 2.f, 2.f);
            obb.center    = Vec3::Zero;
            obb.debugMesh = MeshFactory3D::CreateCube(Vec3 { 2.f, 2.f, 2.f });
        }

        // earth
        {
            Entity e   = CreateEntity();
            auto&  t   = e.GetComponent<Transform>();
            t.position = Vec3(20.f, 12.f, 35.f);

            auto& mc = e.AddComponent<MeshComponent>();
            mc       = commonMC;
            mc.mesh  = MeshFactory3D::CreateSphere(5.f, 48, 48);
            mc.textureArray.Clear();
            mc.textureArray.Add(D11_Texture::Create("Resources\\TrackballDemo\\earthDemo.jpg", m_sampler));
            mc.textureArray.Add(m_diffuseCubeMap);
            mc.textureArray.Add(m_specularCubeMap);
            mc.useTexture = true;

            auto& mat     = e.AddComponent<MaterialComponent>();
            mat.ambient   = Vec4(1.f, 1.f, 1.f, 1.f);
            mat.diffuse   = Vec4(1.f, 1.f, 1.f, 1.f);
            mat.specular  = Vec4(0.7f, 0.7f, 0.7f, 1.f);
            mat.shininess = 2.f;

            e.AddComponent<TagComponent>("Earth");

            auto& bs     = e.AddComponent<BoundingShpereComponent>();
            bs.center    = Vec3::Zero;
            bs.radius    = 5.f;
            bs.debugMesh = MeshFactory3D::CreateSphere(5.f, 48, 48);
        }

        // for debug sphere
        {
            Entity e   = CreateEntity();
            auto&  t   = e.GetComponent<Transform>();
            t.position = Vec3(10000.f, 10000.f, 10000.f);

            auto& mc = e.AddComponent<MeshComponent>();
            mc       = commonMC;
            mc.mesh  = MeshFactory3D::CreateSphere(0.1f, 16, 16);

            auto& mat     = e.AddComponent<MaterialComponent>();
            mat.ambient   = Vec4(1.f, 1.f, 1.f, 1.f);
            mat.diffuse   = Vec4(1.f, 1.f, 0.f, 1.f);
            mat.specular  = Vec4(1.f, 1.f, 1.f, 1.f);
            mat.shininess = 1.f;

            m_debugSphere = e;
        }
    }

    //===================================================
    //                  Field Init
    //===================================================
    {
        m_cbPixelShaderData.diffuseMapStrength  = 0.7f;
        m_cbPixelShaderData.specularMapStrength = 0.9f;
        m_cbPixelShaderData.speculerShininess   = 1.0f;
        m_cbPixelShaderData.useSmoothStep       = true;

        m_defaultDSS = D11_DepthStencilState::Create(true, true, false);
        m_debugDDS   = D11_DepthStencilState::Create(false, false, false);
    }
}

void TrackballDemo::OnEnter()
{
}

void TrackballDemo::OnExit()
{
    GetAppWindow().EnableMouseRelativeMode(false);
}

void TrackballDemo::OnUpdate(TimeStamp& in_ts)
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

    // Rasterizer State
    {
        if (m_rsStateDirty)
        {
            m_rsState = D11_RasterizerState::Create(
                m_cullenable ? eCullMode::Back : eCullMode::None,
                m_wireframe ? eFillMode::Wireframe : eFillMode::Solid,
                eFrontFace::ClockWise);
            m_rsState->Bind();
            m_rsStateDirty = false;
        }
    }

    // Trackball
    if (!m_rotateMode)
    {
        if (Input::IsMouseDown(eMouse::Left))
        {
            auto [x, y] = Input::GetMousePos();
            if (_MousePicking(x, y, m_camera, &m_selectedEntity, &m_intersectionPoint, &m_ray))
            {
                m_debugSphere.GetTransform().position = m_intersectionPoint;
            }
            else
            {
                // if there is no intersection, move debug sphere to far away
                // best way is do not render it -> but for debug purpose, move it to far away
                m_debugSphere.GetTransform().position = Vec3(10000.f, 10000.f, 10000.f);
            }
        }

        _UpdateTrackball();
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

void TrackballDemo::OnRender(TimeStamp& in_ts)
{
    auto* dx = D11_API;
    m_defaultDSS->Bind();
    dx->SetBackBufferToRenderTarget();
    dx->ClearBackBuffer(color::BLACK);

    GetView<CameraControllerComponent, Transform>().each(
        [&](CameraControllerComponent& in_cc,
            Transform&                 in_t)
        {
            CB_Camera cmr;
            cmr.eyePos   = in_t.position;
            cmr.viewProj = in_cc.camera.GetViewProj(in_t);

            m_cbCamera->Bind();
            m_cbCamera->UpdateData(cmr);
        });

    GetView<MeshComponent, Transform, MaterialComponent, IDComponent>().each(
        [&](MeshComponent&     in_mc,
            Transform&         in_t,
            MaterialComponent& in_mat,
            IDComponent&       in_id)
        {
            CB_Object data;
            data.world             = in_t.GetWorld();
            data.worldInvTranspose = in_t.GetWorldInvTranspose();
            data.ambient           = in_mat.ambient;
            data.diffuse           = in_mat.diffuse;
            data.specular          = in_mat.specular;
            data.shininess         = in_mat.shininess;
            data.useTexture        = in_mc.useTexture;
            data.entityID          = in_id.id;

            m_cbObject->Bind();
            m_cbObject->UpdateData(data);

            for (const auto& cb: in_mc.constantBuffers)
                cb->Bind();

            dx->SetTopology(in_mc.topology);
            in_mc.vertexShader->Bind();
            in_mc.pixelShader->Bind();
            in_mc.textureArray.Bind(0, eShaderFlags_PixelShader);
            in_mc.mesh->Draw();
        });

    if (m_visibleBoundingBox)
    {
        m_debugDDS->Bind();
        m_debugPS->Bind();

        GetView<OBBComponent, Transform>().each(
            [&](OBBComponent& in_obb,
                Transform&    in_t)
            {
                CB_Object data;
                data.world = in_t.GetWorld();
                m_cbObject->Bind();
                m_cbObject->UpdateData(data);
                in_obb.debugMesh->Draw();
            });

        GetView<BoundingShpereComponent, Transform>().each(
            [&](BoundingShpereComponent& in_bs,
                Transform&               in_t)
            {
                CB_Object data;
                data.world = in_t.GetWorld();
                m_cbObject->Bind();
                m_cbObject->UpdateData(data);
                in_bs.debugMesh->Draw();
            });
    }

    if (m_cbPixelShaderDirty)
    {
        m_cbPixelShader->Bind();
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

void TrackballDemo::OnRenderGUI(TimeStamp& in_ts)
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

    ImGui::End();

    ImGui::Begin("Mouse Picking", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::Checkbox("Visible Bounding Box", &m_visibleBoundingBox);

    ImGui::TextUnformatted("Click on screen any where!");

    ImGui::TextUnformatted("Selected Entity ID:");

    ImGui::SameLine();
    if (m_selectedEntity.IsValid())
        ImGui::Text("%s", m_selectedEntity.GetComponent<TagComponent>().name.c_str());
    else
        ImGui::Text("None");

    ImGui::Text("Intersection Point: %.2f, %.2f, %.2f",
                m_intersectionPoint.x,
                m_intersectionPoint.y,
                m_intersectionPoint.z);

    ImGui::SeparatorText("Trackball Mode");
    ImGui::RadioButton("None", (int*)&m_trackballMode, (int)eTrackballMode::None);
    ImGui::RadioButton("Rotate", (int*)&m_trackballMode, (int)eTrackballMode::Rotate);
    ImGui::RadioButton("Translate", (int*)&m_trackballMode, (int)eTrackballMode::Translate);

    ImGui::End();
}

void TrackballDemo::OnEvent(CrabEvent& in_event)
{
}

void TrackballDemo::_DrawGUI()
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

    ImGui::SeparatorText("Rasterizer State");

    if (ImGui::Checkbox("Wireframe", &m_wireframe))
        m_rsStateDirty = true;

    ImGui::SameLine();
    if (ImGui::Checkbox("Cull Enable", &m_cullenable))
        m_rsStateDirty = true;

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
}

void TrackballDemo::_BuildPostProcessData()
{
    m_ppStack.clear();

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

    // sampling
    setting.pixelShader = samplingPS;
    auto sampling       = PostProcess::Create(setting);
    sampling->SetTexture(D11_API->GetSwapChain()->GetBackBufferTexture());
    m_ppStack.push_back(sampling);

    // anti-aliasing
    if (m_enableAntiAliasing)
    {
        for (int i = 0; i < m_fxaaQuality; ++i)
        {
            setting.pixelShader = FXAAPS;
            auto FXAA           = PostProcess::Create(setting);
            FXAA->SetTexture(m_ppStack.back()->GetTexture());
            m_ppStack.push_back(FXAA);
        }
    }

    // Final Sampling
    setting.pixelShader = samplingPS;
    auto finalSampling  = PostProcess::Create(setting);
    finalSampling->SetTexture(m_ppStack.back()->GetTexture());
    finalSampling->SetRenderTarget(D11_API->GetSwapChain()->GetBackBuffer());
    m_ppStack.push_back(finalSampling);
}

bool TrackballDemo::_MousePicking(uint32  in_x,
                                  uint32  in_y,
                                  Entity  in_cameraEntity,
                                  Entity* out_selectedEntity,
                                  Vec3*   out_intersectionPoint,
                                  Ray*    out_ray)
{
    auto [width, height] = GetAppWindow().GetWindowSize();

    if (in_x >= width || in_y >= height)
        return false;

    // return value
    Entity selectedEntity    = Entity::s_null;
    Vec3   intersactionPoint = Vec3::Zero;
    bool   isIntersected     = false;

    // ray casting
    Vec3 posInNDC;
    posInNDC.x = 2 * float(in_x) / width - 1.f;
    posInNDC.y = -(2 * float(in_y) / height - 1.f);
    posInNDC.z = 0.f;

    auto& cmr = in_cameraEntity.GetComponent<CameraControllerComponent>().camera;
    auto& t   = in_cameraEntity.GetComponent<Transform>();

    Mat4 invViewProj = cmr.GetViewProj(t).Invert();
    Vec3 posInWorld  = Vec3::Transform(posInNDC, invViewProj);

    float shortestDist = CRAB_FLOAT_MAX;

    Ray ray       = {};
    ray.position  = posInWorld;
    ray.direction = Normalize(ray.position - t.position);

    // collision detection
    GetView<OBBComponent, Transform, IDComponent>().each(
        [&](OBBComponent& in_obb,
            Transform&    in_t,
            IDComponent&  in_id)
        {
            OBB obb;
            obb.Center      = in_t.position;
            obb.Extents     = in_obb.size * 0.5f;
            obb.Orientation = in_t.quaternion;

            float dist = 0.f;
            if (obb.Intersects(ray.position, ray.direction, dist))
            {
                if (dist < shortestDist)
                {
                    shortestDist   = dist;
                    selectedEntity = FindEntity(in_id.id);
                }
            }
        });

    GetView<BoundingShpereComponent, Transform, IDComponent>().each(
        [&](BoundingShpereComponent& in_bs,
            Transform&               in_t,
            IDComponent&             in_id)
        {
            BoundingShpere sphere;
            sphere.Center = in_t.position;
            sphere.Radius = in_bs.radius;

            float dist = 0.f;
            if (ray.Intersects(sphere, dist))
            {
                if (dist < shortestDist)
                {
                    shortestDist   = dist;
                    selectedEntity = FindEntity(in_id.id);
                }
            }
        });

    // not selected over farZ
    Plane farZ { t.position + t.Forward() * cmr.farZ,
                 t.Forward() };

    float distTofarZ = 0.f;
    CRAB_ASSERT(ray.Intersects(farZ, distTofarZ),
                "Ray does not intersect with far plane");

    if (shortestDist > distTofarZ)
        selectedEntity = Entity::s_null;

    if (selectedEntity.IsValid())
    {
        Log::Debug("Selected Entity ID: {0}", selectedEntity.GetID().id);
        intersactionPoint = ray.position + ray.direction * shortestDist;
        isIntersected     = true;
    }
    else
    {
        Log::Debug("No Entity Selected");
        intersactionPoint = Vec3::Zero;
        isIntersected     = false;
    }

    if (out_selectedEntity)
        *out_selectedEntity = selectedEntity;

    if (out_intersectionPoint)
        *out_intersectionPoint = intersactionPoint;

    if (out_ray)
        *out_ray = ray;

    return isIntersected;
}

void TrackballDemo::_UpdateTrackball()
{
    if (m_trackballMode == eTrackballMode::None)
        return;

    if (!Input::IsMouseDown(eMouse::Left))
        return;

    // if there is no mouse movement, return
    auto [dx, dy] = Input::GetMouseDeltaPos();
    if (dx == 0 && dy == 0)
        return;

    // if there is no selected entity, return
    if (!m_selectedEntity.IsValid())
        return;

    auto [prevX, prevY] = Input::GetPrevMousePos();

    Entity selectedPrevEntity;
    Vec3   intersectionPrevPoint;
    Ray    prevRay;

    // if there is no intersection, return
    if (!_MousePicking(prevX, prevY, m_camera, &selectedPrevEntity, &intersectionPrevPoint, &prevRay))
        return;

    // if selected entity is changed, return
    if (selectedPrevEntity != m_selectedEntity)
        return;

    auto [cmrT, cmr] = m_camera.GetComponents<Transform, CameraControllerComponent>();
    auto& entityT    = m_selectedEntity.GetComponent<Transform>();

    if (m_trackballMode == eTrackballMode::Rotate)
    {
        Vec3 v0 = Normalize(intersectionPrevPoint - entityT.position);
        Vec3 v1 = Normalize(m_intersectionPoint - entityT.position);

        Vec3  axis = v0.Cross(v1);
        float len  = axis.Length();

        // if the length of axis is too small, return
        if (len < SMALL_NUMBER)
            return;

        axis      = axis / len;
        float rad = std::acosf(v0.Dot(v1));

        if (rad > SMALL_NUMBER)
        {
            Quat q = Quat::CreateFromAxisAngle(axis, rad);
            entityT.quaternion *= q;
        }
    }
    else if (m_trackballMode == eTrackballMode::Translate)
    {
        float distCmrToNearZ     = (cmrT.position - prevRay.position).Length();
        float distCmrToPrevPoint = (cmrT.position - intersectionPrevPoint).Length();

        float scaleNearZToPrevPoint = distCmrToPrevPoint / distCmrToNearZ;
        Vec3  moveVec               = m_ray.position - prevRay.position;
        float moveDist              = moveVec.Length() * scaleNearZToPrevPoint;

        entityT.position += Normalize(moveVec) * moveDist;
    }
}
