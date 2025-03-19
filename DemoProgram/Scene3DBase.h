#pragma once

class Scene3DBase : public Scene
{
public:
    ~Scene3DBase() override;

    void Init() override;

    void OnEnter() override;
    void OnExit() override;

    void OnUpdate(TimeStamp& in_ts) override;
    void OnRender(TimeStamp& in_ts) override;
    void OnPostRender(TimeStamp& in_ts) override;

    void OnEvent(CrabEvent& in_event) override;

    //===================================================
    // Rendering Properties
    //===================================================

    void EnableDrawNormal(bool in_enable) { m_renderingProp.drawNormal = in_enable; }
    void EnableWireframeMode(bool in_enable) { m_renderingProp.wireframeMode = in_enable; }

    bool IsDrawNormalEnabled() const { return m_renderingProp.drawNormal; }
    bool IsWireframeModeEnabled() const { return m_renderingProp.wireframeMode; }

    Entity              GetCameraEntity() const { return m_cameraEntity; }
    TransformComponent& GetCameraTransform() { return m_cameraEntity.GetComponent<TransformComponent>(); }
    CameraComponent&    GetCameraComponent() { return m_cameraEntity.GetComponent<CameraComponent>(); }

    //===================================================
    // Post Process Properties
    //===================================================

    // bloom
    void EnablePostProcessBloom(bool in_enable);
    void SetBloomCombineFactor(float in_factor);
    void SetBloomBlurRadius(float in_radius);
    void SetBloomBlurCount(uint32 in_count);

    bool   IsPostProcessBloomEnabled() const { return m_postProcessProp.useBloom; }
    float  GetBloomCombineFactor() const { return m_postProcessProp.bloomCombineFactor; }
    float  GetBloomBlurRadius() const { return m_postProcessProp.bloomBlurRadius; }
    uint32 GetBloomBlurCount() const { return m_postProcessProp.bloomBlurCount; }

    // tone mapping
    void  EnablePostProcessToneMapping(bool in_enable) { m_postProcessProp.useToneMapping = in_enable; }
    void  SetToneMappingExposure(float in_exposure) { m_postProcessProp.exposure = in_exposure; }
    void  SetToneMappingGamma(float in_gamma) { m_postProcessProp.gamma = in_gamma; }

    bool  IsPostProcessToneMappingEnabled() const { return m_postProcessProp.useToneMapping; }
    float GetToneMappingExposure() const { return m_postProcessProp.exposure; }
    float GetToneMappingGamma() const { return m_postProcessProp.gamma; }

protected:
    Scene3DBase() = default;
    void _CreateStagingTexture(uint32 in_width, uint32 in_height);

    //===================================================
    // Core Renderer Resource
    //===================================================

    // G-buffer
    PBRGeometryPass m_gBufferPass;

    // HDR
    Ref<RenderTarget> m_backBufferHDR;
    Ref<Texture2D>    m_copyOfBackBufferHDR;

    Ref<DepthBuffer> m_backBufferDepthBuffer;
    Viewport         m_screenViewport;

    Entity m_cameraEntity;

private:
    //===================================================
    // Rendering options
    //===================================================

    struct RenderingProp
    {
        bool drawNormal    = false;
        bool wireframeMode = false;
    };

    RenderingProp m_renderingProp = {};

    //===================================================
    // Post process
    //===================================================

    struct PostProcessProp
    {
        bool dirtyBit = true;

        // Bloom
        bool   useBloom           = true;
        float  bloomCombineFactor = 0.15f;
        float  bloomBlurRadius    = 0.01f;
        uint32 bloomBlurCount     = 3;

        // Tone Mapping
        bool  useToneMapping = GetRenderer().GetHDR().enableHDR;
        float exposure       = 1.f;
        float gamma          = 2.2f;
    };

    PostProcess     m_postProcess;
    PostProcessProp m_postProcessProp = {};

    Ref<ConstantBuffer<CombineConstant>>     m_postprocessCombineConst;
    Ref<ConstantBuffer<ToneMappingConstant>> m_postprocessToneMappingConst;
};
