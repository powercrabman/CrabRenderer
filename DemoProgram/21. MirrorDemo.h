#pragma once

class SceneHierarchy;

class MirrorDemo : public Scene
{
public:
    void Init() override;

    void OnEnter() override;
    void OnExit() override;

    void OnUpdate(TimeStamp& in_ts) override;
    void _DrawMirror();
    void OnRender(TimeStamp& in_ts) override;
    void OnRenderGUI(TimeStamp& in_ts) override;

    void OnEvent(CrabEvent& in_event) override;
    void _RenderBasicMesh();

    IMPLEMENT_SCENE(MirrorDemo);

private:
    void _RenderSkybox();
    void _RenderPBRMesh(TransformComponent& t, MeshRenderer& m, PBRMaterialComponent& mat);
    void _BuildPostProcess();

    Ref<PipelineState> m_basicPipeline;
    Ref<PipelineState> m_skyboxPipeline;

    Ref<PostProcess> m_postProcess;

    Entity m_light;
    Entity m_camera;
    Entity m_mirror;

    Ref<Texture2D> m_envMapTex;
    Ref<Texture2D> m_difMapTex;
    Ref<Texture2D> m_spcMapTex;
    Ref<Texture2D> m_brdfTex;

    Scope<SceneHierarchy> m_sceneHierarchy;

    // post process factor
    int32 m_bloomMipChainCount = 4;
    bool  m_bloomEnable        = true;
    bool  m_postProcessDirty   = false;
    float m_bloomStrength      = 0.2f;

    Ref<ConstantBuffer<CombinePSConstant>> m_combineConstant = nullptr;
    float                                  m_mirrorAlpha     = 0.5f;
};
