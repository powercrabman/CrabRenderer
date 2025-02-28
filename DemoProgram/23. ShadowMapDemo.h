#pragma once

class SceneHierarchy;

class ShadowMapDemo : public Scene
{
public:
    void Init() override;

    void OnEnter() override;
    void OnExit() override;

    void OnUpdate(TimeStamp& in_ts) override;
    void OnRender(TimeStamp& in_ts) override;
    void OnRenderGUI(TimeStamp& in_ts) override;

    void OnEvent(CrabEvent& in_event) override;

    IMPLEMENT_SCENE(ShadowMapDemo);

private:
    void _RenderSkybox();
    void _RenderPBRMesh(TransformComponent& t, MeshRenderer& m, PBRMaterialComponent& mat);
    void _BuildPostProcess();
    void _RenderBasicMesh();
    void _RenderLightMesh();
    Mat4 _UpdateShadow(uint32 index, TransformComponent& t, ShadowMapComponent& s);

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

    Ref<ConstantBuffer<PostEffectConstant>> m_postEffectConstant = nullptr;
    PostEffectConstant                      m_postEffectData     = {};
    bool                                    m_postEffectDirty    = true;
};
