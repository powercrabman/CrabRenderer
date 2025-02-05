#include "pch.h"

#include "9. ImageBasedLightningDemo.h"

#include "D11_Model.h"
#include "GUIs.h"

void ImageBasedLightningDemo::Init()
{
    auto* dx = D11_API;

    // Shader
    {
        m_commonVS = D11_VertexShader::CreateFromFile("LightVertexShader.hlsl", "VSmain", Vertex::s_inputElements);
        m_IBLPS    = D11_PixelShader::CreateFromFile("ImageBasedLightningPixelShader.hlsl", "main");
        m_normalVS = D11_VertexShader::CreateFromFile("NormalVertexShader.hlsl", "main", Vertex::s_inputElements);
        m_normalPS = D11_PixelShader::CreateFromFile("NormalPixelShader.hlsl", "main");
    }

    // Constant Buffer
    {
        m_cbObject      = D11_ConstantBuffer<CB_Object>::Create();
        m_cbCamera      = D11_ConstantBuffer<CB_Camera>::Create();
        m_cbPixelShader = D11_ConstantBuffer<CB_PixelShader>::Create();
    }

    // Depth Stencil State
    m_dsState = D11_DepthStencilState::Create(true, true, false);

    auto samplerState = D11_SamplerState::Create(eSamplerFilter::Linear, eSamplerAddress::Wrap);

    m_diffuseCubeMap = Texture::CreateTextureCubeByDDS(
        R"(Resources\cubemap\4\diffuse.dds)",
        samplerState);

    m_specularCubeMap = Texture::CreateTextureCubeByDDS(
        R"(Resources\cubemap\4\specular.dds)",
        samplerState);

    // Model Entity
    {
        m_modelEntity = CreateEntity();

        // Model Renderer
        D11_ModelLoader loader;
        loader.Load(R"(Resources\status\marble_bust_01_2k.fbx)");
        auto modelVec = loader.GetMeshDatas();

        auto& mr        = m_modelEntity.AddComponent<ModelRenderer>();
        mr.model        = loader.CreateModel();
        mr.pixelShader  = m_IBLPS;
        mr.vertexShader = m_commonVS;
        mr.topology     = eTopology::TriangleList;

        // Transform
        Transform& t = m_modelEntity.GetComponent<Transform>();
        t.position   = Vec3(0.f, -3.f, 5.f);
        t.scale      = Vec3(10.f, 10.f, 10.f);
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

    // Directional Light
    {
        Entity dirLightEntity = CreateEntity();
        auto&  lc             = dirLightEntity.AddComponent<LightComponent>();
        lc.lightType          = eLightType::Directional;
        lc.lightColor         = Vec3(1.f, 1.f, 1.f);
        lc.lightIntensity     = 1.f;
        lc.falloffStart       = 1.f;
        lc.falloffEnd         = 50.f;

        dirLightEntity.GetComponent<Transform>().rotation = EulerFromLookAt({ 0.f, 0.f, 0.f }, { 1.f, -1.f, 1.f });

        m_dirLightEntity = dirLightEntity;
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

    // CubeMap Entity
    {
        m_cubeMapEntity = CreateEntity();

        auto& mr        = m_cubeMapEntity.AddComponent<MeshRenderer>();
        mr.mesh         = MeshFactory3D::CreateCubeMap({ 100.f, 100.f, 100.f });
        mr.vertexShader = m_commonVS;
        mr.pixelShader  = D11_PixelShader::CreateFromFile("CubeMapPixelShader.hlsl", "main");
        auto& mat       = m_cubeMapEntity.AddComponent<MaterialComponent>();
        mat.useTexture  = true;
        mat.material.textureArray.Add(m_specularCubeMap);
    }

    // Primitive Cube
    {
        Entity e        = CreateEntity();
        auto&  mr       = e.AddComponent<MeshRenderer>();
        auto   meshdata = MeshFactory3D::CreateCubeMeshData({ 1.f, 1.f, 1.f });
        mr.mesh         = D11_Mesh::Create(meshdata);
        mr.vertexShader = m_commonVS;
        mr.pixelShader  = m_IBLPS;

        auto& mat                     = e.AddComponent<MaterialComponent>();
        mat.material.ambient          = Vec4(0.3f, 0.3f, 0.3f, 1.f);
        mat.material.diffuse          = Vec4(0.8f, 0.8f, 0.8f, 1.f);
        mat.material.specular         = Vec4(1.f, 1.f, 1.f, 1.f);
        mat.material.shininess        = 32.f;
        mat.useTexture                = false;
        mat.material.textureStartSlot = 1;

        auto& nr        = e.AddComponent<NormalRenderer>();
        nr.mesh         = MeshFactory3D::CreateNormalLines(meshdata);
        nr.vertexShader = m_normalVS;
        nr.pixelShader  = m_normalPS;

        Transform& t = e.GetComponent<Transform>();
        t.position   = Vec3(-2.f, 0.f, 0.f);
    }

    // Primitive Sphere
    {
        Entity e               = CreateEntity();
        auto&  mr              = e.AddComponent<MeshRenderer>();
        auto   meshData        = MeshFactory3D::CreateSphereMeshData(0.5f, 32, 32);
        mr.mesh                = D11_Mesh::Create(meshData);
        mr.vertexShader        = m_commonVS;
        mr.pixelShader         = m_IBLPS;
        auto& mat              = e.AddComponent<MaterialComponent>();
        mat.material.ambient   = Vec4(0.3f, 0.3f, 0.3f, 1.f);
        mat.material.diffuse   = Vec4(0.8f, 0.8f, 0.8f, 1.f);
        mat.material.specular  = Vec4(1.f, 1.f, 1.f, 1.f);
        mat.material.shininess = 32.f;
        mat.useTexture         = false;

        auto& nr        = e.AddComponent<NormalRenderer>();
        nr.mesh         = MeshFactory3D::CreateNormalLines(meshData);
        nr.vertexShader = m_normalVS;
        nr.pixelShader  = m_normalPS;
        Transform& t    = e.GetComponent<Transform>();
        t.position      = Vec3(2.f, 0.f, 0.f);
    }

    // CubeMap Array
    m_cubeMapArray.Add(m_diffuseCubeMap).Add(m_specularCubeMap);
}

void ImageBasedLightningDemo::OnEnter()
{
    auto* dx = D11_API;
    dx->SetConstantBuffer(m_cbObject, 0, eShaderFlags_VertexShader | eShaderFlags_PixelShader);
    dx->SetConstantBuffer(m_cbCamera, 1, eShaderFlags_VertexShader | eShaderFlags_PixelShader);
    dx->SetConstantBuffer(m_cbPixelShader, 2, eShaderFlags_PixelShader);
    dx->SetDepthStencilState(m_dsState);
    _CreateRasterizerState();
}

void ImageBasedLightningDemo::OnExit()
{
}

void ImageBasedLightningDemo::OnUpdate(TimeStamp& in_ts)
{
    m_modelEntity.GetComponent<Transform>().rotation.y += 0.1f * PI * in_ts.deltaTime;

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

void ImageBasedLightningDemo::OnRender(TimeStamp& in_ts)
{
    auto* dx = D11_API;
    dx->SetBackBufferToFrameBuffer();
    dx->ClearFrameBuffer(color::GRAY, true, false);

    dx->SetTextureArray(m_cubeMapArray, 1, eShaderFlags_PixelShader);

    GetView<CameraComponent, Transform>().each(
        [&](CameraComponent& in_cmr,
            Transform&       in_trans)
        {
            CB_Camera data;
            data.eyePos   = in_trans.position;
            data.viewProj = in_cmr.GetViewProj(in_trans);

            m_cbCamera->UpdateData(data);
        });

    CB_PixelShader pixelShaderData;

    GetView<LightComponent, Transform>().each(
        [&](LightComponent& in_light,
            Transform&      in_trans)
        {
            CB_LightComponent data;
            data.lightColor     = in_light.lightColor;
            data.falloffEnd     = in_light.falloffEnd;
            data.falloffStart   = in_light.falloffStart;
            data.lightIntensity = in_light.lightIntensity;
            data.spotPower      = in_light.spotPower;
            data.direction      = in_trans.Forward();
            data.direction.Normalize();
            data.position = in_trans.position;

            if (in_light.lightType == eLightType::Directional)
                pixelShaderData.dirLight = data;
            else if (in_light.lightType == eLightType::Point)
                pixelShaderData.pointLight = data;
            else if (in_light.lightType == eLightType::Spot)
                pixelShaderData.spotLight = data;
        });

    // Light data
    pixelShaderData.diffuseMapStrength  = m_diffuseMapStrength;
    pixelShaderData.specularMapStrength = m_specularMapStrength;
    pixelShaderData.useSmoothStep       = m_useSmoothStep;
    m_cbPixelShader->UpdateData(pixelShaderData);

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
            m_cbObject->UpdateData(data);

            dx->SetTopology(in_mr.topology);
            dx->SetVertexShader(in_mr.vertexShader);
            dx->SetPixelShader(in_mr.pixelShader);
            dx->SetTextureArray(in_mat.material.textureArray,
                                in_mat.material.textureStartSlot,
                                eShaderFlags_PixelShader);

            const auto& meshes   = in_mr.model->meshes;
            const auto& textures = in_mr.model->textures;
            for (uint32 idx = 0; idx < meshes.size(); ++idx)
            {
                dx->SetTexture(textures[idx], 0, eShaderFlags_PixelShader);
                dx->SetVertexBuffer(meshes[idx]->vertexBuffer);
                dx->SetIndexBuffer(meshes[idx]->indexBuffer);
                dx->DrawIndexed(meshes[idx]->indexBuffer->indexCount);
            }
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
            dx->SetVertexBuffer(in_mesh.mesh->vertexBuffer);
            dx->SetIndexBuffer(in_mesh.mesh->indexBuffer);
            dx->SetVertexShader(in_mesh.vertexShader);
            dx->SetPixelShader(in_mesh.pixelShader);
            dx->SetTextureArray(in_mat.material.textureArray,
                                in_mat.material.textureStartSlot,
                                eShaderFlags_PixelShader);

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

void ImageBasedLightningDemo::OnRenderGUI(TimeStamp& in_ts)
{
    DrawDemoInspector(GetName(),
                      [&]()
                      {
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

                          ImGui::TextUnformatted("BlinnPhong Reflection Model");

                          ImGui::TextUnformatted("Pixel shader Setting");
                          ImGui::DragFloat("Diffuse Map Strength", &m_diffuseMapStrength, 0.01f, 0.f, 1.f, "%.3f");
                          ImGui::DragFloat("Specular Map Strength", &m_specularMapStrength, 0.01f, 0.f, 1.f, "%.3f");
                          ImGui::Checkbox("Use Smooth Step", &m_useSmoothStep);
                      });

    // Entity Inspector
    ImGui::Begin("Entity Inspector", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

    const auto& DrawMateriailComponentInspector = [](MaterialComponent& out_m)
    {
        ImGui::ColorEdit3("Ambient", (float*)&out_m.material.ambient);
        ImGui::ColorEdit3("Diffuse", (float*)&out_m.material.diffuse);
        ImGui::ColorEdit3("Specular", (float*)&out_m.material.specular);
        if (ImGui::InputFloat("Shininess", &out_m.material.shininess))
        {
            if (out_m.material.shininess <= 0.f)
                out_m.material.shininess = 1.f;
        }

        ImGui::Checkbox("Use Texture", &out_m.useTexture);
    };

    const auto& DrawLightComponentInspector = [](LightComponent& out_light,
                                                 bool            in_canSetLightType = true)
    {
        ImGui::ColorEdit3("Light Color", (float*)&out_light.lightColor);
        ImGui::DragFloat("Light Intensity", &out_light.lightIntensity, 0.01f, 0.f, 0.f, "%.3f");

        if (in_canSetLightType)
        {
            if (ImGui::RadioButton("Directional", out_light.lightType == eLightType::Directional))
                out_light.lightType = eLightType::Directional;

            ImGui::SameLine();

            if (ImGui::RadioButton("Point", out_light.lightType == eLightType::Point))
                out_light.lightType = eLightType::Point;

            ImGui::SameLine();

            if (ImGui::RadioButton("Spot", out_light.lightType == eLightType::Spot))
                out_light.lightType = eLightType::Spot;
        }

        ImGui::DragFloat("Falloff Start", &out_light.falloffStart, 0.01f);
        ImGui::DragFloat("Falloff End", &out_light.falloffEnd, 0.01f);
        ImGui::DragFloat("Spot Power", &out_light.spotPower, 0.01f);
    };

    // Sphere Entity
    ImGui::PushID("Model Entity");
    {
        ImGui::SeparatorText("Model Entity");

        ImGui::BulletText("Transform");
        DrawTransformInspector(m_modelEntity.GetComponent<Transform>());

        ImGui::BulletText("Material Component");
        DrawMateriailComponentInspector(m_modelEntity.GetComponent<MaterialComponent>());
    }
    ImGui::PopID();

    // Directional Light
    ImGui::PushID("Directional Light Entity");
    {
        ImGui::SeparatorText("Directional Light");

        ImGui::BulletText("Transform");
        DrawTransformInspector(m_dirLightEntity.GetComponent<Transform>());

        ImGui::BulletText("Light Component");
        DrawLightComponentInspector(m_dirLightEntity.GetComponent<LightComponent>(), false);
    }
    ImGui::PopID();

    ImGui::Separator();
    ImGui::Checkbox("Show Normal", &m_showNormal);

    ImGui::End();
}

void ImageBasedLightningDemo::OnEvent(CrabEvent& in_event)
{
}

void ImageBasedLightningDemo::_CreateRasterizerState()
{
    m_rsState = D11_RasterizerState::Create(
        m_cullenable ? eCullMode::Back : eCullMode::None,
        m_wireframe ? eFillMode::Wireframe : eFillMode::Solid,
        eFrontFace::ClockWise);

    D11_API->SetRasterizerState(m_rsState);
}
