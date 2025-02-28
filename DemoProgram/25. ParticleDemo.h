#pragma once

class ParticleDemo : public Scene
{
public:
    ParticleDemo();
    virtual ~ParticleDemo();

    void Init() override;
    void OnEnter() override;
    void OnExit() override;

    void OnUpdate(TimeStamp& in_ts) override;
    void OnRender(TimeStamp& in_ts) override;
    void OnRenderGUI(TimeStamp& in_ts) override;

    void OnEvent(CrabEvent& in_event) override;

    IMPLEMENT_SCENE(ParticleDemo);

private:
    struct Particle
    {
        float radius = 0.f;
        float life   = -1.f;
        Vec2  position = Vec2::Zero;
        Vec3  color    = Vec3::Zero;

        Vec2 velocity = Vec2::Zero;
    };

    cbuffer ParticleConstant
    {
        float deltaTime;   // 4
        Vec3  pad;         // 12
    };

    Ref<ConstantBuffer<ParticleConstant>> m_cbParticle;

    std::vector<Particle> m_particleData;
    uint32                m_particeMaxCount = 1000;

    Ref<StructuredBuffer<Particle>> m_particles;

    Ref<VertexShader>   m_vs;
    Ref<PixelShader>    m_ps;
    Ref<GeometryShader> m_gs;
    Ref<ComputeShader>  m_cs;

    Ref<BlendState> m_blendState;

    static inline Vec3 m_colorPalette[] = {
        Vec3 { 1.f, 0.f, 0.f },
        Vec3 { 0.f, 1.f, 0.f },
        Vec3 { 0.f, 0.f, 1.f },
        Vec3 { 1.f, 1.f, 0.f },
        Vec3 { 1.f, 0.f, 1.f },
        Vec3 { 0.f, 1.f, 1.f }
    };
};
