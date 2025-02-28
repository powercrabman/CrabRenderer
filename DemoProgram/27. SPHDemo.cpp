#include "pch.h"

#include "27. SPHDemo.h"

#include "GUIs.h"

#include <fp16.h>

SPHDemo::SPHDemo()
{
}

SPHDemo::~SPHDemo()
{
}

void SPHDemo::Init()
{
    m_particleCPU.resize(m_particleMaxCount);
    m_particleCount = m_particleInitCount;
    std::generate_n(m_particleCPU.begin(), m_particleInitCount, [&]()
                    { return _CreateParticle(); });

    m_particleGPU = StructuredBuffer<SPHParticle>::Create(m_particleCPU);

    InputElements elem;
    elem.Add("POSITION", 0, eFormat::Float16);
    m_vs = VertexShader::CreateFromHLSL(L"SPHVS.hlsl", "main", elem);
    m_ps = PixelShader::CreateFromHLSL(L"SPHPS.hlsl", "main");
    m_gs = GeometryShader::CreateFromHLSL(L"SPHGS.hlsl", "main");

    m_computeForce           = ComputeShader::CreateFromHLSL(L"SPHComputeForce.hlsl", "main");
    m_computeDensityPressure = ComputeShader::CreateFromHLSL(L"SPHComputeDensityPressure.hlsl", "main");
    m_computeIntegrate       = ComputeShader::CreateFromHLSL(L"SPHIntegrate.hlsl", "main");

    m_cbParticle = ConstantBuffer<ParticleConstant>::Create();

    D3D11_BLEND_DESC blendDesc;
    blendDesc.AlphaToCoverageEnable  = false;
    blendDesc.IndependentBlendEnable = false;
}

void SPHDemo::OnEnter()
{
}

void SPHDemo::OnExit()
{
}

void SPHDemo::OnUpdate(TimeStamp& in_ts)
{
    if (Input::IsKeyPressed(eKey::Escape))
        GetApplication().Quit();

    if (Input::IsMouseDown(eMouse::Left))
    {
        if (m_particleCount < m_particleMaxCount)
        {
            auto [x, y] = Input::GetMousePos();
            Vec2 pos    = crab::ScreenToNDC(x, y);

            auto particle     = _CreateParticle();
            particle.position = pos;

            for (uint32 i = 0; i < 10; ++i)
            {
                m_particleCPU                    = m_particleGPU->ReadFromBuffer();
                m_particleCPU[m_particleCount++] = particle;
            }

            m_particleGPU->WriteToBuffer(m_particleCPU);
        }
    }

    // Constant Update
    {
        m_cbParticle->Bind(0, eShaderFlags_ComputeShader);

        ParticleConstant data = m_cbParticleData;
        data.deltaTime        = in_ts.deltaTime;
        m_cbParticle->WriteToBuffer(data);
    }

    // Run Compute Shader
    {
        uint32 groupX = static_cast<uint32>(std::ceilf(m_particleMaxCount / 32.f));

        m_particleGPU->BindUAV(0);
        m_computeDensityPressure->Dispatch(groupX, 1, 1);

        m_particleGPU->BindUAV(0);
        m_computeForce->Dispatch(groupX, 1, 1);

        m_particleGPU->BindUAV(0);
        m_computeIntegrate->Dispatch(groupX, 1, 1);
    }
}

void SPHDemo::OnRender(TimeStamp& in_ts)
{
    auto& r = GetRenderer();

    r.BindBackBuffer(r.GetDepthBuffer());
    r.ClearDepthBuffer(true, 1.f, false, 0);
    r.ClearBackBuffer(color::WHITE);

    r.SetTopology(eTopology::PointList);

    GetCommonState()->Sampler_LinearWrap()->Bind(0, eShaderFlags_PixelShader);
    GetCommonState()->DepthStencil_DepthNone()->Bind();
    GetCommonState()->Blend_AlphaBlendByAlphaValue(false)->Bind();

    m_particleGPU->BindImage(0, eShaderFlags_VertexShader);

    m_vs->Bind();
    m_ps->Bind();
    m_gs->Bind();

    r.Draw(m_particleGPU->GetItemMaxCount());

    r.SetShaderResourceView(nullptr, 0, eShaderFlags_VertexShader);
}

void SPHDemo::OnRenderGUI(TimeStamp& in_ts)
{
    DrawDemoInspector(GetName(), [&]()
                      {
                          ImGui::Text("Particle Count: %d", m_particleCount);

                          static bool s_dirty = false;

                          if (ImGui::DragFloat("Rest Density", &m_cbParticleData.restDensity, 0.01f, 0.01f, 100.f))
                              s_dirty = true;

                          if (ImGui::DragFloat("Stiffness", &m_cbParticleData.stiffness, 0.01f, 0.01f, 100.f))
                              s_dirty = true;

                          if (ImGui::DragFloat("Viscosity", &m_cbParticleData.viscosity, 0.01f, 0.01f, 100.f))
                              s_dirty = true;

                          if (ImGui::Button("Reset"))
                          {
                              m_particleCPU.clear();
                              m_particleCPU.resize(m_particleMaxCount);
                              std::generate_n(m_particleCPU.begin(), m_particleInitCount, [&]()
                                              { return _CreateParticle(); });
                              m_particleCount = m_particleInitCount;
                              m_particleGPU->WriteToBuffer(m_particleCPU);
                          } });
}

void SPHDemo::OnEvent(CrabEvent& in_event)
{
}