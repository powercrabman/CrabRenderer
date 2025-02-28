#pragma once

class SceneHierarchy;

class PBRDemo : public Scene
{
public:
    void Init() override;

    void OnEnter() override;
    void OnExit() override;

    void OnUpdate(TimeStamp& in_ts) override;
    void OnRender(TimeStamp& in_ts) override;
    void OnRenderGUI(TimeStamp& in_ts) override;

    void OnEvent(CrabEvent& in_event) override;

    IMPLEMENT_SCENE(PBRDemo);

private:
    void _BuildPostProcess();

    Ref<PipelineState> m_basicPipeline;
    Ref<PipelineState> m_skyboxPipeline;

    Ref<PostProcess> m_postProcess;

    Entity m_light;
    Entity m_camera;

    Ref<Texture2D> m_envMapTex;
    Ref<Texture2D> m_difMapTex;
    Ref<Texture2D> m_spcMapTex;
    Ref<Texture2D> m_brdfTex;

    Scope<SceneHierarchy> m_sceneHierarchy;

    // post process factor
    int32 m_bloomMipChainCount = 4;
    bool  m_bloomEnable        = true;
    float m_postProcessDirty   = false;
    float m_bloomStrength      = 0.2f;

    Ref<ConstantBuffer<CombinePSConstant>> m_combineConstant = nullptr;
};
