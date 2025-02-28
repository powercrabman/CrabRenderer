#include "pch.h"

#include "18. HDRIDemo.h"

#include "GUIs.h"

const char* g_whiteVS = R"(
cbuffer CB_Object : register(b0)
{
    MATRIX g_world;
    MATRIX g_worldInvTranspose;

    bool g_useTexture;
};

cbuffer CB_Camera : register(b1)
{
    MATRIX g_viewProj;
    float3 g_eyePosW;
}

struct VSInput
{
    float3 posL : POSITION;
    float2 texCoord : TEXCOORD;
    float3 normalL : NORMAL;
    float4 color : COLOR;
};

struct PSInput
{
    float4 posH : SV_POSITION;
    float3 posW : POSITION;
    float3 normalW : NORMAL;
    float2 texCoord : TEXCOORD;
};

PSInput main( VSInput input)
{
    PSInput output;
    output.posW = mul(float4(input.posL, 1.0f), g_world).xyz;
    output.normalW = mul(float4(input.normalL, 1.0f), g_worldInvTranspose).xyz;
    output.normalW = normalize(output.normalW);
    output.posH = mul(float4(output.posW, 1.0f), g_viewProj);
    output.texCoord = input.texCoord;
    return output;
}
)";
const char* g_whitePS = R"(            
struct PSInput
{
    float4 posH : SV_POSITION;
    float3 posW : POSITION;
    float3 normalW : NORMAL;
    float2 texCoord : TEXCOORD;
};

float4 main(PSInput input) : SV_TARGET
{
    return float4(1.f, 1.f, 1.f, 1.f);
})";

const char* g_hdriPS = R"(
struct PSInput
{
    float4 posH : SV_POSITION;
    float3 posW : POSITION;
    float3 normalW : NORMAL;
    float2 texCoord : TEXCOORD;
    float3 tangentW : TANGENT;
};

Texture2D g_diffuseMap : register(t0);
SamplerState g_sampler : register(s0);

float4 main(PSInput input) : SV_TARGET
{
    return g_diffuseMap.Sample(g_sampler, input.texCoord) ;
}
)";

void HDRIDemo::Init()
{
    // Init
    {
        // render state
        m_commonSampler = D11_SamplerState::Create(eSamplerFilter::Linear, eSamplerAddress::Wrap);
        m_rsStateDirty  = true;

        m_hdriDirty    = true;
        m_hdriExposure = 1.f;
        m_hdriGamma    = 2.2f;

        D11_InputElements elem = {};
        elem.Add("POSITION", 0, eD11_Format::Float32x3)
            .Add("TEXCOORD", 0, eD11_Format::Float32x2)
            .Add("NORMAL", 0, eD11_Format::Float32x3)
            .Add("TANGENT", 0, eD11_Format::Float32x3);

        // Shaders
        m_commonVS = D11_VertexShader::CreateFromFile("NMDemoVS.hlsl", "main", elem);
        m_commonPS = D11_PixelShader::CreateFromFile("NMDemoPS.hlsl", "main");
        m_whiteVS  = D11_VertexShader::CreateFromString(g_whiteVS, "main", Vertex_Pos3D_Normal_Tex_Color::s_inputElements);
        m_whitePS  = D11_PixelShader::CreateFromString(g_whitePS, "main");
        m_hdriPS   = D11_PixelShader::CreateFromString(g_hdriPS, "main");

        // constant buffer
        m_cbObject      = D11_ConstantBuffer<CB_Object>::Create();
        m_cbCamera      = D11_ConstantBuffer<CB_Camera>::Create();
        m_cbLight       = D11_ConstantBuffer<CB_PixelShader>::Create();
        m_cbHDRI        = D11_ConstantBuffer<CB_HDRI>::Create();
        m_cbBlurSample  = D11_ConstantBuffer<CB_BlurSample>::Create();
        m_cbBlurCombine = D11_ConstantBuffer<CB_BlurCombine>::Create();

        // texture
        std::filesystem::path path = R"(Resources\NormalMapDemo\)";

        m_diffuseBlock  = D11_Texture2D::CreateMipmap(path / "sandstone_blocks_05_diff_4k.jpg");
        m_normalBlock   = D11_Texture2D::CreateMipmap(path / "sandstone_blocks_05_nor_4k_dx.png");
        path            = R"(Resources\HDRIDemo\)";
        m_hdriTexture   = D11_Texture2D::Create(path / "NightSkyHDRI008_4K-HDR.exr");
        m_skyboxTexture = D11_Texture2D::CreateTextureCubeByDDS(R"(Resources\cubemap\6\specular.dds)");
    }

    // Entity
    {
        // cube
        {
            std::vector<Vertex> v { 24, Vertex {} };

            // front
            v[0].position = Vec3 { -1.f, 1.f, -1.f };
            v[1].position = Vec3 { 1.f, 1.f, -1.f };
            v[2].position = Vec3 { 1.f, -1.f, -1.f };
            v[3].position = Vec3 { -1.f, -1.f, -1.f };

            v[0].texCoord = Vec2 { 0.f, 0.f };
            v[1].texCoord = Vec2 { 100.f, 0.f };
            v[2].texCoord = Vec2 { 100.f, 100.f };
            v[3].texCoord = Vec2 { 0.f, 100.f };

            v[0].normal = Vec3 { 0.f, 0.f, -1.f };
            v[1].normal = Vec3 { 0.f, 0.f, -1.f };
            v[2].normal = Vec3 { 0.f, 0.f, -1.f };
            v[3].normal = Vec3 { 0.f, 0.f, -1.f };

            v[0].tangent = Vec3 { 1.f, 0.f, 0.f };
            v[1].tangent = Vec3 { 1.f, 0.f, 0.f };
            v[2].tangent = Vec3 { 1.f, 0.f, 0.f };
            v[3].tangent = Vec3 { 1.f, 0.f, 0.f };

            // right
            v[4].position = Vec3 { 1.f, 1.f, -1.f };
            v[5].position = Vec3 { 1.f, 1.f, 1.f };
            v[6].position = Vec3 { 1.f, -1.f, 1.f };
            v[7].position = Vec3 { 1.f, -1.f, -1.f };

            v[4].texCoord = Vec2 { 0.f, 0.f };
            v[5].texCoord = Vec2 { 100.f, 0.f };
            v[6].texCoord = Vec2 { 100.f, 100.f };
            v[7].texCoord = Vec2 { 0.f, 100.f };

            v[4].normal = Vec3 { 1.f, 0.f, 0.f };
            v[5].normal = Vec3 { 1.f, 0.f, 0.f };
            v[6].normal = Vec3 { 1.f, 0.f, 0.f };
            v[7].normal = Vec3 { 1.f, 0.f, 0.f };

            v[4].tangent = Vec3 { 0.f, 0.f, 1.f };
            v[5].tangent = Vec3 { 0.f, 0.f, 1.f };
            v[6].tangent = Vec3 { 0.f, 0.f, 1.f };
            v[7].tangent = Vec3 { 0.f, 0.f, 1.f };

            // top
            v[8].position  = Vec3 { -1.f, 1.f, 1.f };
            v[9].position  = Vec3 { 1.f, 1.f, 1.f };
            v[10].position = Vec3 { 1.f, 1.f, -1.f };
            v[11].position = Vec3 { -1.f, 1.f, -1.f };

            v[8].texCoord  = Vec2 { 0.f, 0.f };
            v[9].texCoord  = Vec2 { 100.f, 0.f };
            v[10].texCoord = Vec2 { 100.f, 100.f };
            v[11].texCoord = Vec2 { 0.f, 100.f };

            v[8].normal  = Vec3 { 0.f, 1.f, 0.f };
            v[9].normal  = Vec3 { 0.f, 1.f, 0.f };
            v[10].normal = Vec3 { 0.f, 1.f, 0.f };
            v[11].normal = Vec3 { 0.f, 1.f, 0.f };

            v[8].tangent  = Vec3 { 1.f, 0.f, 0.f };
            v[9].tangent  = Vec3 { 1.f, 0.f, 0.f };
            v[10].tangent = Vec3 { 1.f, 0.f, 0.f };
            v[11].tangent = Vec3 { 1.f, 0.f, 0.f };

            // back
            v[12].position = Vec3 { 1.f, 1.f, 1.f };
            v[13].position = Vec3 { -1.f, 1.f, 1.f };
            v[14].position = Vec3 { -1.f, -1.f, 1.f };
            v[15].position = Vec3 { 1.f, -1.f, 1.f };

            v[12].texCoord = Vec2 { 0.f, 0.f };
            v[13].texCoord = Vec2 { 100.f, 0.f };
            v[14].texCoord = Vec2 { 100.f, 100.f };
            v[15].texCoord = Vec2 { 0.f, 100.f };

            v[12].normal = Vec3 { 0.f, 0.f, 1.f };
            v[13].normal = Vec3 { 0.f, 0.f, 1.f };
            v[14].normal = Vec3 { 0.f, 0.f, 1.f };
            v[15].normal = Vec3 { 0.f, 0.f, 1.f };

            v[12].tangent = Vec3 { -1.f, 0.f, 0.f };
            v[13].tangent = Vec3 { -1.f, 0.f, 0.f };
            v[14].tangent = Vec3 { -1.f, 0.f, 0.f };
            v[15].tangent = Vec3 { -1.f, 0.f, 0.f };

            // left
            v[16].position = Vec3 { -1.f, 1.f, 1.f };
            v[17].position = Vec3 { -1.f, 1.f, -1.f };
            v[18].position = Vec3 { -1.f, -1.f, -1.f };
            v[19].position = Vec3 { -1.f, -1.f, 1.f };

            v[16].texCoord = Vec2 { 0.f, 0.f };
            v[17].texCoord = Vec2 { 100.f, 0.f };
            v[18].texCoord = Vec2 { 100.f, 100.f };
            v[19].texCoord = Vec2 { 0.f, 100.f };

            v[16].normal = Vec3 { -1.f, 0.f, 0.f };
            v[17].normal = Vec3 { -1.f, 0.f, 0.f };
            v[18].normal = Vec3 { -1.f, 0.f, 0.f };
            v[19].normal = Vec3 { -1.f, 0.f, 0.f };

            v[16].tangent = Vec3 { 0.f, 0.f, -1.f };
            v[17].tangent = Vec3 { 0.f, 0.f, -1.f };
            v[18].tangent = Vec3 { 0.f, 0.f, -1.f };
            v[19].tangent = Vec3 { 0.f, 0.f, -1.f };

            // bottom
            v[20].position = Vec3 { -1.f, -1.f, -1.f };
            v[21].position = Vec3 { 1.f, -1.f, -1.f };
            v[22].position = Vec3 { 1.f, -1.f, 1.f };
            v[23].position = Vec3 { -1.f, -1.f, 1.f };

            v[20].texCoord = Vec2 { 0.f, 0.f };
            v[21].texCoord = Vec2 { 100.f, 0.f };
            v[22].texCoord = Vec2 { 100.f, 100.f };
            v[23].texCoord = Vec2 { 0.f, 100.f };

            v[20].normal = Vec3 { 0.f, -1.f, 0.f };
            v[21].normal = Vec3 { 0.f, -1.f, 0.f };
            v[22].normal = Vec3 { 0.f, -1.f, 0.f };
            v[23].normal = Vec3 { 0.f, -1.f, 0.f };

            v[20].tangent = Vec3 { 1.f, 0.f, 0.f };
            v[21].tangent = Vec3 { 1.f, 0.f, 0.f };
            v[22].tangent = Vec3 { 1.f, 0.f, 0.f };
            v[23].tangent = Vec3 { 1.f, 0.f, 0.f };

            // clang-format off
            std::vector<uint32> i =
            {
                // front
                0, 1, 2, 0, 2, 3,
                // right
                4, 5, 6, 4, 6, 7,
                // top
                8, 9, 10, 8, 10, 11,
                // back
                12, 13, 14, 12, 14, 15,
                // left
                16, 17, 18, 16, 18, 19,
                // bottom
                20, 21, 22, 20, 22, 23
            };
            // clang-format on

            Entity e        = CreateEntity();
            auto&  mc       = e.AddComponent<MeshComponent>();
            mc.mesh         = D11_Mesh::Create(v, i);
            mc.topology     = eTopology::TriangleList;
            mc.vertexShader = m_commonVS;
            mc.pixelShader  = m_commonPS;

            mc.constantBufferList
                .Add(m_cbObject, 0, eShaderFlags_VertexPixelShader)
                .Add(m_cbCamera, 1, eShaderFlags_VertexPixelShader)
                .Add(m_cbLight, 2, eShaderFlags_PixelShader);

            mc.samplerlist.Add(m_commonSampler, 0, eShaderFlags_PixelShader);

            mc.texturelist
                .Add(m_diffuseBlock, 0, eShaderFlags_PixelShader)
                .Add(m_normalBlock, 1, eShaderFlags_PixelShader);

            auto& t      = e.GetComponent<Transform>();
            t.position   = Vec3 { 0.f, -8.f, 0.f };
            t.quaternion = Quat::Identity;
            t.scale      = Vec3 { 300.f, 1.f, 300.f };
        }

        // simple sprite
        {
            auto  imageData = m_hdriTexture->GetImageData();
            float width     = static_cast<float>(imageData.width) / 1000;
            float height    = static_cast<float>(imageData.height) / 1000;

            std::vector<Vertex> v;
            v.resize(4);
            v[0].position = Vec3 { -width, height, 0.f };
            v[1].position = Vec3 { width, height, 0.f };
            v[2].position = Vec3 { width, -height, 0.f };
            v[3].position = Vec3 { -width, -height, 0.f };

            v[0].texCoord = Vec2 { 0.f, 0.f };
            v[1].texCoord = Vec2 { 1.f, 0.f };
            v[2].texCoord = Vec2 { 1.f, 1.f };
            v[3].texCoord = Vec2 { 0.f, 1.f };

            std::vector<uint32> i = { 0, 1, 2, 0, 2, 3 };

            Entity e        = CreateEntity();
            auto&  mc       = e.AddComponent<MeshComponent>();
            mc.mesh         = D11_Mesh::Create(v, i);
            mc.topology     = eTopology::TriangleList;
            mc.vertexShader = m_commonVS;
            mc.pixelShader  = m_hdriPS;

            mc.constantBufferList
                .Add(m_cbObject, 0, eShaderFlags_VertexShader)
                .Add(m_cbCamera, 1, eShaderFlags_VertexShader);

            mc.samplerlist.Add(m_commonSampler, 0, eShaderFlags_PixelShader);
            mc.texturelist.Add(m_hdriTexture, 0, eShaderFlags_PixelShader);

            auto& t    = e.GetComponent<Transform>();
            t.position = Vec3 { 0.f, 0.f, 5.f };
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
            cmr.farZ           = 500.f;
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

                            t.quaternion = QuatFromEuler(euler);
                        }
                    }
                }

            private:
                bool m_rotateMode = false;
            };

            e.AddComponent<ScriptComponent>(CreateScope<CameraControllerScript>(e));
        }

        // light
        {
            Entity e   = CreateEntity();
            auto&  pos = e.GetTransform().position;
            pos.x      = 1.f;
            pos.y      = 35.f;
            pos.z      = -1.f;

            m_lightEntity = e;

            auto& ms        = e.AddComponent<MeshComponent>();
            ms.mesh         = MeshFactory3D::CreateSphere(0.1f, 16, 16);
            ms.topology     = eTopology::TriangleList;
            ms.vertexShader = m_commonVS;
            ms.pixelShader  = m_whitePS;

            ms.constantBufferList
                .Add(m_cbObject, 0, eShaderFlags_VertexShader)
                .Add(m_cbCamera, 1, eShaderFlags_VertexShader);
        }

        // sphere map
        {
            std::vector<Vertex> vertices;
            std::vector<uint32> indices;

            float radius = 100.f;
            float stack  = 32;
            float slice  = 32;

            // vertex
            for (int i = 0; i <= stack; ++i)
            {
                float v   = i / (float)stack;
                float phi = v * PI;
                for (int j = 0; j <= slice; ++j)
                {
                    float  u     = j / (float)slice;
                    float  theta = u * PI * 2;
                    float  x     = cos(theta) * sin(phi);
                    float  y     = cos(phi);
                    float  z     = sin(theta) * sin(phi);
                    Vertex vertex;
                    vertex.position = Vec3 { x, y, z } * radius;
                    vertex.normal   = Vec3 { x, y, z };
                    vertex.texCoord = Vec2 { u, v };
                    vertices.push_back(vertex);
                }
            }

            // index
            for (int i = 0; i < stack; ++i)
            {
                for (int j = 0; j < slice; ++j)
                {
                    indices.push_back(i * (slice + 1) + j);
                    indices.push_back((i + 1) * (slice + 1) + j);
                    indices.push_back((i + 1) * (slice + 1) + j + 1);
                    indices.push_back(i * (slice + 1) + j);
                    indices.push_back((i + 1) * (slice + 1) + j + 1);
                    indices.push_back(i * (slice + 1) + j + 1);
                }
            }

            D11_InputElements elem = {};
            elem.Add("POSITION", 0, eD11_Format::Float32x3)
                .Add("TEXCOORD", 0, eD11_Format::Float32x2)
                .Add("NORMAL", 0, eD11_Format::Float32x3)
                .Add("TANGENT", 0, eD11_Format::Float32x3);

            Entity e  = CreateEntity();
            auto&  mc = e.AddComponent<MeshComponent>();
            mc.constantBufferList
                .Add(m_cbObject, 0, eShaderFlags_VertexShader)
                .Add(m_cbCamera, 1, eShaderFlags_VertexShader);
            mc.mesh         = D11_Mesh::Create(vertices, indices);
            mc.pixelShader  = D11_PixelShader::CreateFromFile("HDRICubemapPS.hlsl", "main");
            mc.vertexShader = D11_VertexShader::CreateFromFile("HDRICubemapVS.hlsl", "main", elem);
            mc.samplerlist.Add(m_commonSampler, 0, eShaderFlags_PixelShader);
            mc.texturelist.Add(m_skyboxTexture, 0, eShaderFlags_PixelShader);
            mc.topology = eTopology::TriangleList;
        }
    }

    m_postProcess = PostProcess::Create();
    _BuildPostProcess();
}

void HDRIDemo::OnEnter()
{
}

void HDRIDemo::OnExit()
{
}

void HDRIDemo::OnUpdate(TimeStamp& in_ts)
{
    auto* api = D11_API;

    if (Input::IsKeyPressed(eKey::Escape))
    {
        AppClose_CoreEvent e;
        GetApplication().DispatchEvent(e);
    }
}

void HDRIDemo::OnRender(TimeStamp& in_ts)
{
    if (m_rsStateDirty)
    {
        m_rsState = D11_RasterizerState::Create(
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

    auto* api = D11_API;
    api->BindFloatBackBuffer();
    api->ClearFloatBackBuffer(color::BLACK);
    api->ClearDepthBuffer();

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
        auto& t = m_lightEntity.GetTransform();

        CB_PixelShader data;
        data           = m_cbPixelShader;
        data.lightPosW = t.position;

        m_cbLight->UpdateData(data);
    }

    GetView<MeshComponent, Transform>().each(
        [&](MeshComponent& in_mc,
            Transform&     in_t)
        {
            CB_Object data;
            data.useTexture        = true;
            data.world             = in_t.GetWorld();
            data.worldInvTranspose = in_t.GetWorldInvTranspose();
            m_cbObject->UpdateData(data);

            api->SetTopology(in_mc.topology);

            in_mc.vertexShader->Bind();
            in_mc.pixelShader->Bind();
            in_mc.samplerlist.Bind();
            in_mc.texturelist.Bind();
            in_mc.constantBufferList.Bind();

            in_mc.mesh->Draw();
        });

    {
    }

    m_postProcess->Render();

    api->BindBackBuffer();
}

void HDRIDemo::OnRenderGUI(TimeStamp& in_ts)
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

                          ImGui::SeparatorText("Point light");
                          {
                              auto& pos = m_lightEntity.GetTransform().position;

                              ImGui::DragFloat("X", &pos.x, 0.01f);
                              ImGui::DragFloat("Y", &pos.y, 0.01f);
                              ImGui::DragFloat("Z", &pos.z, 0.01f);
                          }

                          ImGui::SeparatorText("Rasterizer State");
                          if (ImGui::Checkbox("Enable culling", &m_enableCulling))
                              m_rsStateDirty = true;

                          if (ImGui::Checkbox("Enable wireframe mode", &m_enableWireframeMode))
                              m_rsStateDirty = true;

                          ImGui::SeparatorText("Textures");

                          static bool s_useDiffuseTexture = m_cbPixelShader.useDiffuseTexture;
                          static bool s_useNormalTexture  = m_cbPixelShader.useNormalTexture;

                          if (ImGui::Checkbox("Use Diffuse Texture", &s_useDiffuseTexture))
                              m_cbPixelShader.useDiffuseTexture = s_useDiffuseTexture;

                          if (ImGui::Checkbox("Use Normal Texture", &s_useNormalTexture))
                              m_cbPixelShader.useNormalTexture = s_useNormalTexture;
                      });

    ImGui::Begin("HDRI", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

    if (ImGui::SliderFloat("Exposure", &m_hdriExposure, 0.1f, 10.f))
        m_hdriDirty = true;

    if (ImGui::SliderFloat("Gamma", &m_hdriGamma, 0.1f, 10.f))
        m_hdriDirty = true;

    if (ImGui::DragFloat("Blur Strength", &m_blurStrength, 0.01f, 0.f, 1.f))
        m_blurDirty = true;

    ImGui::End();
}

void HDRIDemo::OnEvent(CrabEvent& in_event)
{
}

void HDRIDemo::_BuildPostProcess()
{
    m_postProcess->ClearFilterList();

    auto [width, height]   = GetAppWindow().GetWindowSize();
    const uint32 downCount = 4;
    const uint32 upCount   = 4;

    auto ppVS          = D11_VertexShader::CreateFromFile("PostProcessVS.hlsl", "main", PostProcessVertex::s_inputElements);
    auto samplingPS    = D11_PixelShader::CreateFromFile("SamplingPostProcessPS.hlsl", "main");
    auto tonemappingPS = D11_PixelShader::CreateFromFile("ToneMapPS.hlsl", "main");
    auto downBlurPS    = D11_PixelShader::CreateFromFile("DownBlurPS.hlsl", "main");
    auto upBlurPS      = D11_PixelShader::CreateFromFile("UpBlurPS.hlsl", "main");
    auto combindPS     = D11_PixelShader::CreateFromFile("BlurCombinePS.hlsl", "main");
    auto commonSampler = D11_SamplerState::Create(eSamplerFilter::Linear, eSamplerAddress::Clamp);

    m_cbBlurSample->UpdateData(CB_BlurSample { 1.f / width, 1.f / height });

    Ref<ImageFilter> samplingFilter1 = ImageFilter::Create(0, 0, ppVS, samplingPS);
    samplingFilter1->AddInputSampler(commonSampler, 0);
    samplingFilter1->AddInputTexture(D11_API->GetSwapChain()->GetFloatBackBufferResolveTex(), 0);
    samplingFilter1->SetOnBindCallback([]()
                                       { D11_API->GetSwapChain()->ResolveFloatBackBufferTex(); });
    m_postProcess->AddFilter(samplingFilter1);

    for (int i = 0; i < downCount; ++i)
    {
        float w = static_cast<float>(width / std::pow(2, i + 1));
        float h = static_cast<float>(height / std::pow(2, i + 1));

        Ref<ImageFilter> downsampleFilter = ImageFilter::Create(w, h, ppVS, downBlurPS);
        downsampleFilter->AddInputSampler(commonSampler, 0);
        downsampleFilter->AddInputTexture(m_postProcess->GetLastFilter()->GetOutputTexture(), 0);
        downsampleFilter->AddConstantBuffer(m_cbBlurSample, 0);
        m_postProcess->AddFilter(downsampleFilter);
    }

    for (int i = 0; i < upCount; ++i)
    {
        float w = static_cast<float>(width / std::pow(2, downCount - 1 - i));
        float h = static_cast<float>(height / std::pow(2, downCount - 1 - i));

        Ref<ImageFilter> upsampleFilter = ImageFilter::Create(w, h, ppVS, upBlurPS);
        upsampleFilter->AddInputSampler(commonSampler, 0);
        upsampleFilter->AddInputTexture(m_postProcess->GetLastFilter()->GetOutputTexture(), 0);
        upsampleFilter->AddConstantBuffer(m_cbBlurSample, 0);
        m_postProcess->AddFilter(upsampleFilter);
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
