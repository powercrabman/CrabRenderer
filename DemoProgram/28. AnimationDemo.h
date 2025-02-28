#pragma once

class SceneHierarchy;

class AnimationDemo : public Scene
{
public:
    AnimationDemo() = default;
    ~AnimationDemo() override;

    void Init() override;

    void OnEnter() override;
    void OnExit() override;

    void OnUpdate(TimeStamp& in_ts) override;
    void OnRender(TimeStamp& in_ts) override;
    void OnPostRender(TimeStamp& in_ts) override;
    void OnRenderGUI(TimeStamp& in_ts) override;

    void OnEvent(CrabEvent& in_event) override;

private:
    void _DrawSkybox();
    void _DrawPBRMesh(const TransformComponent& t, const Ref<Mesh>& mesh, const Ref<Material>& mat);

    PostProcess m_postProcess;
    bool        m_postProcessDirty = true;

    Entity m_cameraEntity;
    Entity m_skyboxDayEntity;
    Entity m_skyboxNightEntity;

    bool  m_wireframeMode            = false;
    bool  m_renderNormalMode         = false;
    float m_normalStrength           = 0.2f;

    enum class eSkyboxType
    {
        Day,
        Night
    } m_skyboxType = eSkyboxType::Day;

    Scope<SceneHierarchy>                m_sceneHierarchy;
    Ref<ConstantBuffer<ToneMapConstant>> m_toneMapConstant;
};
