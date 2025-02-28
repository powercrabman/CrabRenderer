#include "pch.h"

#include "26. FireParticleDemo.h"

#include "GUIs.h"

#include <fp16.h>

FireParticleDemo::FireParticleDemo()
{
}

FireParticleDemo::~FireParticleDemo()
{
}

void FireParticleDemo::Init()
{
    m_particleData.resize(m_particeMaxCount);
    m_particles = StructuredBuffer<FireParticle>::Create(m_particleData);

    InputElements elem;
    elem.Add("POSITION", 0, eFormat::Float16);
    m_vs = VertexShader::CreateFromHLSL(L"FireParticleDemoVS.hlsl", "main", elem);
    m_ps = PixelShader::CreateFromHLSL(L"FireParticleDemoPS.hlsl", "main");
    m_gs = GeometryShader::CreateFromHLSL(L"FireParticleDemoGS.hlsl", "main");
    m_cs = ComputeShader::CreateFromHLSL(L"FireParticleDemoCS.hlsl", "main");

    m_cbParticle = ConstantBuffer<ParticleConstant>::Create();

    D3D11_BLEND_DESC blendDesc       = {};
    blendDesc.AlphaToCoverageEnable  = false;
    blendDesc.IndependentBlendEnable = false;

    D3D11_RENDER_TARGET_BLEND_DESC rtbd;

    rtbd.BlendEnable           = true;
    rtbd.SrcBlend              = D3D11_BLEND_SRC_ALPHA;
    rtbd.DestBlend             = D3D11_BLEND_ONE;
    rtbd.BlendOp               = D3D11_BLEND_OP_ADD;
    rtbd.SrcBlendAlpha         = D3D11_BLEND_ONE;
    rtbd.DestBlendAlpha        = D3D11_BLEND_ONE;
    rtbd.BlendOpAlpha          = D3D11_BLEND_OP_ADD;
    rtbd.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

    blendDesc.RenderTarget[0] = rtbd;

    m_blendState = BlendState::Create(blendDesc);

    m_texture = Texture2D::Create(R"(Resources\flare0.dds)");
    m_texture->Bind(0, eShaderFlags_PixelShader);
    GetRenderStateStorage()->LinearWrap()->Bind(0, eShaderFlags_PixelShader);
}

void FireParticleDemo::OnEnter()
{
}

void FireParticleDemo::OnExit()
{
}

void FireParticleDemo::OnUpdate(TimeStamp& in_ts)
{
    if (Input::IsKeyPressed(eKey::Escape))
        GetApplication().Quit();

    m_cbParticle->Bind(0, eShaderFlags_ComputeShader);
    m_cbParticle->WriteToBuffer({ in_ts.deltaTime });

    auto [width, height] = GetAppWindow().GetWindowSize();

    m_particles->BindUnorderedAccessView(0);

    m_cs->Dispatch(
        static_cast<uint32>(std::ceilf(width / 32.f)),
        static_cast<uint32>(std::ceilf(height / 32.f)),
        1);

    if (Input::IsMouseDown(eMouse::Left))
    {
        auto [x, y] = Input::GetMousePos();

        auto particles = m_particles->ReadFromBuffer();
        uint32 maxCount  = 0;

        for (auto& p: particles)
        {
            if (p.life <= 0.f)
            {
                p.position.x = (x / static_cast<float>(width) - 0.5f) * 2.f;
                p.position.y = (y / static_cast<float>(height) - 0.5f) * -2.f;

                p.life  = RandomFloat(1.f, 3.f);
                p.scale = RandomFloat(0.5f, 1.5f);
                p.velocity = Vec2(RandomFloat(-0.5f, 0.5f), RandomFloat(-0.2f,0.5f));

                ++maxCount;
                if (maxCount >= 30)
                {
                    m_particles->WriteToBuffer(particles);
                    break;
                }
            }
        }
    }
}

void FireParticleDemo::OnRender(TimeStamp& in_ts)
{
    auto* dx = D11_API;

    dx->BindBackBuffer(dx->GetDepthBuffer());
    dx->ClearDepthBuffer(true, 1.f, false, 0);
    dx->ClearBackBuffer(color::BLACK);

    dx->SetTopology(eTopology::PointList);

    GetRenderStateStorage()->DepthNone()->Bind();
    m_blendState->Bind();

    m_particles->BindTexture(0, eShaderFlags_VertexShader);

    m_vs->Bind();
    m_ps->Bind();
    m_gs->Bind();

    dx->Draw(m_particles->GetItemMaxCount());

    dx->SetShaderResourceView(nullptr, 0, eShaderFlags_VertexShader);
}

void FireParticleDemo::OnRenderGUI(TimeStamp& in_ts)
{
    DrawDemoInspector(GetName());
}

void FireParticleDemo::OnEvent(CrabEvent& in_event)
{
}