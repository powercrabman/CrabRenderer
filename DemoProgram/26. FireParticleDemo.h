#pragma once

class FireParticleDemo : public Scene
{
public:
    FireParticleDemo();
    virtual ~FireParticleDemo();

    void Init() override;
    void OnEnter() override;
    void OnExit() override;

    void OnUpdate(TimeStamp& in_ts) override;
    void OnRender(TimeStamp& in_ts) override;
    void OnRenderGUI(TimeStamp& in_ts) override;

    void OnEvent(CrabEvent& in_event) override;

    IMPLEMENT_SCENE(FireParticleDemo);

private:
    struct FireParticle
    {
        Vec2  position    = Vec2::Zero;
        Vec2  velocity    = Vec2::Zero;
        float life        = -1.f;
        float scale       = 1.f;
    };

    cbuffer ParticleConstant
    {
        float deltaTime;   // 4
        Vec3  pad;         // 12
    };

    Ref<ConstantBuffer<ParticleConstant>> m_cbParticle;

    std::vector<FireParticle> m_particleData;
    uint32                    m_particeMaxCount = 5000;

    Ref<StructuredBuffer<FireParticle>> m_particles;

    Ref<VertexShader>   m_vs;
    Ref<PixelShader>    m_ps;
    Ref<GeometryShader> m_gs;
    Ref<ComputeShader>  m_cs;

    Ref<BlendState> m_blendState;

    Ref<Texture2D> m_texture;
};
