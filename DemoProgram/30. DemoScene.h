#pragma once

class SceneHierarchy;

class DemoScene : public Scene
{
public:
    ~DemoScene() override;

    void Init() override;

    void OnEnter() override;
    void OnExit() override;

    void OnUpdate(TimeStamp& in_ts) override;
    void OnRender(TimeStamp& in_ts) override;
    void OnPostRender(TimeStamp& in_ts) override;
    void OnRenderGUI(TimeStamp& in_ts) override;

    void OnEvent(CrabEvent& in_event) override;

private:
    Ref<RenderTarget> m_backBufferHDR;
    Ref<DepthBuffer>  m_backBufferDepthBuffer;

    Scope<SceneHierarchy> m_sceneHierarchy;

    PostProcess m_postProcess;

    Entity m_camera;

    SkyboxRenderer m_daySkybox;
    SkyboxRenderer m_nightSkybox;
    enum class eSkyboxType
    {
        Day,
        Night,
    } m_skyboxType = eSkyboxType::Night;

    bool m_wireframeMode = false;
    bool m_drawNormal    = false;

    // Postprocess
    bool m_postProcessDirty = true;
    bool m_useBloom         = true;

    Ref<ConstantBuffer<CombineConstant>> m_postprocessCombineConst;
    float                                m_postprocessCombineFactor = 0.15f;
    float                                m_postprocessBlurRadius    = 0.01f;
};
