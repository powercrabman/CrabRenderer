#pragma once

class SPHDemo : public Scene
{
public:
    SPHDemo();
    virtual ~SPHDemo();

    void Init() override;
    void OnEnter() override;
    void OnExit() override;

    void OnUpdate(TimeStamp& in_ts) override;
    void OnRender(TimeStamp& in_ts) override;
    void OnRenderGUI(TimeStamp& in_ts) override;

    void OnEvent(CrabEvent& in_event) override;

    IMPLEMENT_SCENE(SPHDemo);

private:
    struct SPHParticle
    {
        Vec2  position = Vec2::Zero;
        Vec2  velocity = Vec2::Zero;
        Vec3  color    = Vec3::Zero;
        float radius   = 0.1f;
        float life     = -1.f;

        // for SPH
        float mass     = 1.f;
        float density  = 0.f;   // rho
        float pressure = 0.f;   // p
        Vec2  force    = Vec2::Zero;
    };

    SPHParticle _CreateParticle() const;

    cbuffer ParticleConstant
    {
        float deltaTime;            // 4
        float restDensity = 0.5f;    // p0
        float stiffness   = 0.5f;    // k
        float viscosity   = 0.1f;   // u
    };

    Ref<ConstantBuffer<ParticleConstant>> m_cbParticle;
    ParticleConstant                      m_cbParticleData;

    uint32 m_particleInitCount = 2000;
    uint32 m_particleMaxCount  = 10000;

    Ref<StructuredBuffer<SPHParticle>> m_particleGPU;
    std::vector<SPHParticle>           m_particleCPU;
    uint32                             m_particleCount = 0;

    Ref<VertexShader>   m_vs;
    Ref<PixelShader>    m_ps;
    Ref<GeometryShader> m_gs;

    Ref<ComputeShader> m_computeForce;
    Ref<ComputeShader> m_computeDensityPressure;
    Ref<ComputeShader> m_computeIntegrate;

    Ref<BlendState> m_blendState;

    std::vector<Vec3> m_colorPalette = {
        Vec3 { 0.f, 0.f, 1.f },
    };
};

inline SPHDemo::SPHParticle SPHDemo::_CreateParticle() const
{
    SPHParticle p;
    p.velocity = Vec2::Zero;
    p.color    = m_colorPalette[RandomInt(0, m_colorPalette.size())];
    p.radius   = 0.03f;
    p.position = Vec2 {
        RandomFloat(-0.9f + p.radius, 0.9f - p.radius),
        RandomFloat(-0.9f + p.radius, 0.9f - p.radius)
    };
    p.life     = 100.f;
    p.mass     = 1.f;
    p.density  = 0.f;
    p.pressure = 0.f;
    p.force    = Vec2::Zero;

    return p;
}