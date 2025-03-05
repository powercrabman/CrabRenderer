#pragma once

class GlobalShader : public Singleton<GlobalShader>
{
    friend class Singleton<GlobalShader>;

public:
    void Init();

    // Vertex Shader
    [[nodiscard]] Ref<VertexShader> DrawNormalVertexShader() const { return m_drawNormalVS; }
    [[nodiscard]] Ref<VertexShader> SkyboxVertexShader() const { return m_skyboxVS; }
    [[nodiscard]] Ref<VertexShader> PBRVertexShader() const { return m_PBRVS; }
    [[nodiscard]] Ref<VertexShader> PostProcessVertexShader() const { return m_postProcessVS; }
    [[nodiscard]] Ref<VertexShader> MaskingVertexShader() const { return m_maskingVS; }
    [[nodiscard]] Ref<VertexShader> BasicShadowCasterVertexShader() const { return m_basicShadowCasterVS; }
    [[nodiscard]] Ref<VertexShader> CascadeOmniShadowCasterVertexShader() const { return m_casOmniShadowCasterVS; }

    // Pixel Shader
    [[nodiscard]] Ref<PixelShader> BlurDownPixelShader() const { return m_blurDownPS; }
    [[nodiscard]] Ref<PixelShader> BlurUpPixelShader() const { return m_blurUpPS; }
    [[nodiscard]] Ref<PixelShader> CombinePixelShader() const { return m_combinePS; }
    [[nodiscard]] Ref<PixelShader> DrawNormalPixelShader() const { return m_drawNormalPS; }
    [[nodiscard]] Ref<PixelShader> PBRPixelShader() const { return m_PBRPS; }
    [[nodiscard]] Ref<PixelShader> SamplingPixelShader() const { return m_samplingPS; }
    [[nodiscard]] Ref<PixelShader> SkyboxPixelShader() const { return m_skyboxPS; }
    [[nodiscard]] Ref<PixelShader> ToneMappingPixelShader() const { return m_toneMappingPS; }
    [[nodiscard]] Ref<PixelShader> MaskingPixelShader() const { return m_maskingPixelShader; }
    [[nodiscard]] Ref<PixelShader> PostEffectPixelShader() const { return m_postEffectPS; }
    [[nodiscard]] Ref<PixelShader> DebugNormalMapPixelShader() const { return m_debugNormalMapPS; }
    [[nodiscard]] Ref<PixelShader> DepthVisualizePixelShader() const { return m_depthVisualizePS; }
    [[nodiscard]] Ref<PixelShader> CommonShadowCasterPixelShader() const { return m_commonShadowCasterPS; }

    // Geometry Shader
    [[nodiscard]] Ref<GeometryShader> DrawNormalGeometryShader() const { return m_drawNormalGS; }
    [[nodiscard]] Ref<GeometryShader> CascadeShadowCasterGeometryShader() const { return m_cascadeShadowCasterGS; }

    [[nodiscard]] Ref<GeometryShader> OmniShadowCasterGeometryShader() const { return m_omniShadowCasterGS; }
private:
    // Vertex shader
    Ref<VertexShader> m_drawNormalVS;
    Ref<VertexShader> m_skyboxVS;
    Ref<VertexShader> m_PBRVS;
    Ref<VertexShader> m_postProcessVS;
    Ref<VertexShader> m_maskingVS;
    Ref<VertexShader> m_basicShadowCasterVS;
    Ref<VertexShader> m_casOmniShadowCasterVS;

    // Pixel shader
    Ref<PixelShader> m_blurDownPS;
    Ref<PixelShader> m_blurUpPS;
    Ref<PixelShader> m_combinePS;
    Ref<PixelShader> m_drawNormalPS;
    Ref<PixelShader> m_PBRPS;
    Ref<PixelShader> m_samplingPS;
    Ref<PixelShader> m_skyboxPS;
    Ref<PixelShader> m_toneMappingPS;
    Ref<PixelShader> m_maskingPixelShader;
    Ref<PixelShader> m_postEffectPS;
    Ref<PixelShader> m_debugNormalMapPS;
    Ref<PixelShader> m_depthVisualizePS;
    Ref<PixelShader> m_commonShadowCasterPS;

    // Geometry shader
    Ref<GeometryShader> m_drawNormalGS;
    Ref<GeometryShader> m_cascadeShadowCasterGS;
    Ref<GeometryShader> m_omniShadowCasterGS;

    // Domain shader

    // Hull shader

    // Compute shader
};

inline GlobalShader& GetShaderLibrary()
{
    return GlobalShader::GetInstance();
}
