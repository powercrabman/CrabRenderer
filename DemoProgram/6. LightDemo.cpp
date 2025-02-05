#include "pch.h"

#include "6. LightDemo.h"

#include "GUIs.h"

void LightDemo::Init()
{
    auto* dx = D11_API;

    // Shader
    {
        m_lightVS      = D11_VertexShader::CreateFromFile("LightVertexShader.hlsl", "VSmain", Vertex::s_inputElements);
        m_phongPS      = D11_PixelShader::CreateFromFile("PhongPixelShader.hlsl", "PSmain");
        m_blinnPhongPS = D11_PixelShader::CreateFromFile("BlinnPhongPixelShader.hlsl", "PSmain");
        m_normalVS     = D11_VertexShader::CreateFromFile("NormalVertexShader.hlsl", "main", Vertex::s_inputElements);
        m_normalPS     = D11_PixelShader::CreateFromFile("NormalPixelShader.hlsl", "main");
    }

    // Constant Buffer
    {
        m_cbObject = D11_ConstantBuffer<CB_Object>::Create();
        m_cbCamera = D11_ConstantBuffer<CB_Camera>::Create();
        m_cbLight  = D11_ConstantBuffer<CB_Light>::Create();
    }

    // Depth Stencil State
    m_dsState = D11_DepthStencilState::Create(true, true, false);

    // - Rasterizer State
    _CreateRasterizerState();

    auto samplerState = D11_SamplerState::Create(eSamplerFilter::Linear, eSamplerAddress::Wrap);

    // Sphere Entity
    {
        Entity e = CreateEntity();

        // Mesh Renderer
        MeshData meshdata = MeshFactory3D::CreateSphereMeshData(1.f, 16, 16);

        auto& mr        = e.AddComponent<MeshRenderer>();
        mr.mesh         = D11_Mesh::Create(meshdata);
        mr.pixelShader  = m_phongPS;
        mr.vertexShader = m_lightVS;
        mr.topology     = eTopology::TriangleList;

        // Transform
        Transform& sphereTrans = e.GetComponent<Transform>();
        sphereTrans.position   = Vec3(0.f, 0.f, 3.f);

        // Normal Renderer
        auto& nr        = e.AddComponent<NormalRenderer>();
        nr.mesh         = MeshFactory3D::CreateNormalLines(meshdata);
        nr.vertexShader = m_normalVS;
        nr.pixelShader  = m_normalPS;

        // Material Component
        auto& mc                 = e.AddComponent<MaterialComponent>();
        mc.material.ambient      = Vec4(0.2f, 0.2f, 0.2f, 1.f);
        mc.material.diffuse      = Vec4(0.8f, 0.8f, 0.8f, 1.f);
        mc.material.specular     = Vec4(1.f, 1.f, 1.f, 1.f);
        mc.material.shininess    = 32.f;
        mc.useTexture            = true;
        mc.material.textureArray = TextureArray({ Texture::Create("earth.png", samplerState) });

        m_sphereEntity = e;
    }

    // Light Entity
    {
        Entity e       = CreateEntity();
        m_lightEntity = e;

        auto& lc          = e.AddComponent<LightComponent>();
        lc.lightColor     = Vec3(1.f, 1.f, 1.f);
        lc.lightIntensity = 1.f;
        lc.falloffStart   = 1.f;
        lc.falloffEnd     = 50.f;
        lc.lightType      = eLightType::Point;

        m_lightEntity.GetComponent<Transform>().position.x = 2.f;

        auto& mr        = m_lightEntity.AddComponent<MeshRenderer>();
        mr.mesh         = MeshFactory3D::CreateCube({ 0.1f, 0.1f, 0.1f });
        mr.vertexShader = m_lightVS;
        mr.pixelShader  = D11_PixelShader::CreateFromFile("LightPixelShader.hlsl", "PSmain");
        mr.topology     = eTopology::TriangleList;

        auto& mat      = m_lightEntity.AddComponent<MaterialComponent>();
        mat.useTexture = false;
    }

    // Camera Entity
    {
        m_cameraEntity = CreateEntity();

        CameraComponent cmr;
        cmr.aspect         = GetApplication().GetAppWindow().GetAspect();
        cmr.fov            = 45.f * DEG2RAD;
        cmr.nearPlane      = 0.1f;
        cmr.farPlane       = 100.f;
        cmr.projectionType = eProjectionType::Perspective;
        m_cameraEntity.AddComponent<CameraComponent>(cmr);

        Transform& cmrTrans = m_cameraEntity.GetComponent<Transform>();
        cmrTrans.position   = Vec3(0.f, 1.f, -5.f);
    }

    // Floor Entity
    {
        m_floorEntity = CreateEntity();

        // Mesh Renderer
        MeshData floorMeshData = MeshFactory3D::CreateCubeMeshData({ 32.f, 32.f, 1.f });
        auto&    mr            = m_floorEntity.AddComponent<MeshRenderer>();
        mr.mesh                = D11_Mesh::Create(floorMeshData);
        mr.topology            = eTopology::TriangleList;
        mr.pixelShader         = m_phongPS;
        mr.vertexShader        = m_lightVS;

        // Transform
        Transform& floorTrans = m_floorEntity.GetComponent<Transform>();
        floorTrans.position   = Vec3(0.f, -6.f, 0.f);
        floorTrans.rotation.x = 90.f * DEG2RAD;

        // Normal Renderer
        auto& normalRenderer        = m_floorEntity.AddComponent<NormalRenderer>();
        normalRenderer.mesh         = MeshFactory3D::CreateNormalLines(floorMeshData);
        normalRenderer.vertexShader = m_normalVS;
        normalRenderer.pixelShader  = m_normalPS;

        // Material Component
        PhongMaterial floorMat;
        floorMat.ambient   = Vec4(0.4f, 0.4f, 0.4f, 1.f);
        floorMat.diffuse   = Vec4(1.f, 1.f, 1.f, 1.f);
        floorMat.specular  = Vec4(1.f, 1.f, 1.f, 1.f);
        floorMat.shininess = 32.f;
        floorMat.textureArray.Add(Texture::Create("floor.png", samplerState));

        auto& mc      = m_floorEntity.AddComponent<MaterialComponent>(floorMat, true);
        mc.material   = floorMat;
        mc.useTexture = true;
    }
}

void LightDemo::OnEnter()
{
    auto* dx = D11_API;
    dx->SetConstantBuffer(m_cbObject, 0, eShaderFlags_VertexShader | eShaderFlags_PixelShader);
    dx->SetConstantBuffer(m_cbCamera, 1, eShaderFlags_VertexShader | eShaderFlags_PixelShader);
    dx->SetConstantBuffer(m_cbLight, 2, eShaderFlags_PixelShader);
    dx->SetDepthStencilState(m_dsState);
    _CreateRasterizerState();
}

void LightDemo::OnExit()
{
}

void LightDemo::OnUpdate(TimeStamp& in_ts)
{
    m_sphereEntity.GetComponent<Transform>().rotation.y += in_ts.deltaTime * 0.5f;

    if (m_autoRotate)
    {
        static float angleYaw   = 0.f;
        static float anglePitch = 0.f;

        angleYaw += in_ts.deltaTime * m_deltaYaw;
        anglePitch += in_ts.deltaTime * m_deltaPitch;

        float sinY, cosY;
        FastCosSin(angleYaw, cosY, sinY);

        float sinP, cosP;
        FastCosSin(anglePitch, cosP, sinP);

        Vec3       center = m_sphereEntity.GetComponent<Transform>().position;
        Transform& trans  = m_lightEntity.GetComponent<Transform>();

        Mat4 rMat      = Mat4::CreateRotationX(anglePitch) * Mat4::CreateRotationY(angleYaw);
        trans.position = center + m_radius * Vec3::Transform(Vec3::Backward, rMat);
    }

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

void LightDemo::OnRender(TimeStamp& in_ts)
{
    auto* dx = D11_API;
    dx->SetBackBufferToFrameBuffer();
    dx->ClearFrameBuffer(color::BLACK, true, false);

    GetView<CameraComponent, Transform>().each(
        [&](CameraComponent& in_cmr,
            Transform&       in_trans)
        {
            CB_Camera data;
            data.eyePos   = in_trans.position;
            data.viewProj = in_cmr.GetViewProj(in_trans);

            m_cbCamera->UpdateData(data);
        });

    GetView<LightComponent, Transform>().each(
        [&](LightComponent& in_light,
            Transform&      in_trans)
        {
            CB_Light light;
            light.lightColor     = in_light.lightColor;
            light.lightPos       = in_trans.position;
            light.lightIntensity = in_light.lightIntensity;
            light.falloffStart   = in_light.falloffStart;
            light.falloffEnd     = in_light.falloffEnd;
            light.useSmoothStep  = m_useSmoothStep;

            m_cbLight->UpdateData(light);
        });

    GetView<MeshRenderer, Transform, MaterialComponent>().each(
        [&](MeshRenderer&      in_mesh,
            Transform&         in_trans,
            MaterialComponent& in_mat)
        {
            CB_Object data;
            data.world             = in_trans.GetWorld();
            data.worldInvTranspose = in_trans.GetWorldInvTranspose();
            data.useTexture        = in_mat.useTexture;
            data.material          = in_mat.material;
            m_cbObject->UpdateData(data);

            dx->SetTopology(in_mesh.topology);
            dx->SetVertexBuffer(in_mesh.mesh->vertexBuffer);
            dx->SetIndexBuffer(in_mesh.mesh->indexBuffer);
            dx->SetVertexShader(in_mesh.vertexShader);
            dx->SetPixelShader(in_mesh.pixelShader);
            dx->SetTextureArray(in_mat.material.textureArray, 0, eShaderFlags_PixelShader);

            dx->DrawIndexed(in_mesh.mesh->indexBuffer->indexCount);
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
            dx->SetVertexBuffer(in_normal.mesh->vertexBuffer);
            dx->SetIndexBuffer(in_normal.mesh->indexBuffer);
            dx->SetVertexShader(in_normal.vertexShader);
            dx->SetPixelShader(in_normal.pixelShader);

            dx->DrawIndexed(in_normal.mesh->indexBuffer->indexCount);
        });
}

void LightDemo::OnRenderGUI(TimeStamp& in_ts)
{
    DrawDemoInspector(GetName(),
                      [&]()
                      {
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

                          const char* items[] = { "Phong", "BlinnPhong" };
                          // radio
                          if (ImGui::RadioButton("Phong", (int*)&m_shaderMode, 0))
                          {
                              m_sphereEntity.GetComponent<MeshRenderer>().pixelShader = m_phongPS;
                              m_floorEntity.GetComponent<MeshRenderer>().pixelShader  = m_phongPS;
                          }
                          ImGui::SameLine();
                          if (ImGui::RadioButton("BlinnPhong", (int*)&m_shaderMode, 1))
                          {
                              m_sphereEntity.GetComponent<MeshRenderer>().pixelShader = m_blinnPhongPS;
                              m_floorEntity.GetComponent<MeshRenderer>().pixelShader  = m_blinnPhongPS;
                          }

                          // Light
                          ImGui::SeparatorText("Light");

                          auto [lightCmp, lightTrans] = m_lightEntity.GetComponents<LightComponent, Transform>();

                          ImGui::DragFloat3("Light Position", (float*)&lightTrans.position, 0.01f);
                          ImGui::ColorEdit3("Light Color", (float*)&lightCmp.lightColor);
                          ImGui::DragFloat("LightIntensity", &lightCmp.lightIntensity, 0.001f, 0.001f, 1.f);
                          if (ImGui::DragFloat("Falloff Start", &lightCmp.falloffStart, 0.01f, 0.01f))
                          {
                              if (lightCmp.falloffStart >= lightCmp.falloffEnd)
                                  lightCmp.falloffStart = lightCmp.falloffEnd - 0.1f;
                          }

                          if (ImGui::DragFloat("Falloff End", &lightCmp.falloffEnd, 0.01f, 0.01f))
                          {
                              if (lightCmp.falloffEnd <= lightCmp.falloffStart)
                                  lightCmp.falloffEnd = lightCmp.falloffStart + 0.1f;
                          }

                          float* dtAngle[] = { &m_deltaPitch, &m_deltaYaw };
                          ImGui::DragFloat2("Light Rotation Speed", *dtAngle, 0.01f);
                          ImGui::DragFloat("Light Radius", &m_radius, 0.01f);
                          ImGui::Checkbox("Auto Rotate", &m_autoRotate);
                          ImGui::Checkbox("Use Smooth Step", &m_useSmoothStep);
                      });

    // Entity Inspector
    ImGui::Begin("Entity Inspector", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

    // Sphere Entity
    ImGui::PushID("Sphere Entity");
    {
        ImGui::SeparatorText("Sphere Entity");
        auto [trans, mr, mat] = m_sphereEntity.GetComponents<Transform, MeshRenderer, MaterialComponent>();

        ImGui::BulletText("Transform");
        DrawTransformInspector(trans);

        ImGui::BulletText("Material");
        ImGui::ColorEdit3("Ambient", (float*)&mat.material.ambient);
        ImGui::ColorEdit3("Diffuse", (float*)&mat.material.diffuse);
        ImGui::ColorEdit3("Specular", (float*)&mat.material.specular);
        if (ImGui::InputFloat("Shininess", &mat.material.shininess))
        {
            if (mat.material.shininess <= 0.f)
                mat.material.shininess = 1.f;
        }

        ImGui::Checkbox("Use Texture", &mat.useTexture);
    }
    ImGui::PopID();

    // Floor Entity
    ImGui::PushID("Floor Entity");
    {
        ImGui::SeparatorText("Floor Entity");
        auto [trans, mr, mat] = m_floorEntity.GetComponents<Transform, MeshRenderer, MaterialComponent>();

        ImGui::BulletText("Transform");
        DrawTransformInspector(trans);

        ImGui::BulletText("Material");
        ImGui::ColorEdit3("Ambient", (float*)&mat.material.ambient);
        ImGui::ColorEdit3("Diffuse", (float*)&mat.material.diffuse);
        ImGui::ColorEdit3("Specular", (float*)&mat.material.specular);
        if (ImGui::InputFloat("Shininess", &mat.material.shininess))
        {
            if (mat.material.shininess <= 0.f)
                mat.material.shininess = 1.f;
        }

        ImGui::Checkbox("Use Texture", &mat.useTexture);
    }
    ImGui::PopID();

    ImGui::Separator();
    ImGui::Checkbox("Show Normal", &m_showNormal);

    ImGui::End();
}

void LightDemo::OnEvent(CrabEvent& in_event)
{
}

void LightDemo::_CreateRasterizerState()
{
    m_rsState = D11_RasterizerState::Create(
        m_cullenable ? eCullMode::Back : eCullMode::None,
        m_wireframe ? eFillMode::Wireframe : eFillMode::Solid,
        eFrontFace::ClockWise);

    D11_API->SetRasterizerState(m_rsState);
}
