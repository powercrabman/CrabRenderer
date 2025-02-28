#include "pch.h"

#include "25. ParticleDemo.h"

#include "GUIs.h"

#include <fp16.h>

ParticleDemo::ParticleDemo()
{
}

ParticleDemo::~ParticleDemo()
{
}

void ParticleDemo::Init()
{
    m_particleData.resize(m_particeMaxCount);
    for (uint32 i = 0; i < m_particeMaxCount / 10; i++)
    {
        Particle p;
        p.position        = Vec2(RandomFloat(-1.f, 1.f), RandomFloat(-1.f, 1.f));
        p.radius          = RandomFloat(0.02f, 0.04f);
        p.life            = RandomFloat(3.f, 8.f);
        p.color           = m_colorPalette[RandomInt(0, ARRAYSIZE(m_colorPalette))];
        p.velocity        = Vec2(RandomFloat(-0.3f, 0.3f), 0.f);
        m_particleData[i] = p;
    }

    m_particles = StructuredBuffer<Particle>::Create(m_particleData);

    InputElements elem;
    elem.Add("POSITION", 0, eFormat::Float16);
    m_vs = VertexShader::CreateFromHLSL(L"ParticleVS.hlsl", "main", elem);
    m_ps = PixelShader::CreateFromHLSL(L"ParticlePS.hlsl", "main");
    m_gs = GeometryShader::CreateFromHLSL(L"ParticleGS.hlsl", "main");
    m_cs = ComputeShader::CreateFromHLSL(L"ParticleCS.hlsl", "main");

    m_cbParticle = ConstantBuffer<ParticleConstant>::Create();

    D3D11_BLEND_DESC blendDesc       = {};
    blendDesc.AlphaToCoverageEnable  = false;
    blendDesc.IndependentBlendEnable = false;

    D3D11_RENDER_TARGET_BLEND_DESC rtbd = {};

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
}

void ParticleDemo::OnEnter()
{
}

void ParticleDemo::OnExit()
{
}

void ParticleDemo::OnUpdate(TimeStamp& in_ts)
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

        for (auto& p: particles)
        {
            if (p.life <= 0.f)
            {
                p.position.x = (x / static_cast<float>(width) - 0.5f) * 2.f;
                p.position.y = (y / static_cast<float>(height) - 0.5f) * -2.f;

                p.velocity = Vec2(RandomFloat(-0.3f, 0.3f), RandomFloat(-0.3f, 1.f));
                p.color    = m_colorPalette[RandomInt(0, ARRAYSIZE(m_colorPalette))];
                p.life     = RandomFloat(3.f, 8.f);
                p.radius   = RandomFloat(0.02f, 0.04f);

                m_particles->WriteToBuffer(particles);
                break;
            }
        }
    }
}

void ParticleDemo::OnRender(TimeStamp& in_ts)
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

void ParticleDemo::OnRenderGUI(TimeStamp& in_ts)
{
    DrawDemoInspector(GetName());
}

void ParticleDemo::OnEvent(CrabEvent& in_event)
{
}