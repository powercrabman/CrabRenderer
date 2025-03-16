#pragma once

namespace crab
{

class VertexShader;
class PixelShader;
class GeometryShader;

class GlobalShader
{
public:
    void Init(const std::filesystem::path& in_engineDirectory);

    // Vertex Shader
    [[nodiscard]] Ref<VertexShader> DrawNormalVS() const { return m_drawNormalVS; }
    [[nodiscard]] Ref<VertexShader> SkyboxVS() const { return m_skyboxVS; }
    [[nodiscard]] Ref<VertexShader> PBRVS() const { return m_PBRVS; }
    [[nodiscard]] Ref<VertexShader> PostProcessVS() const { return m_postProcessVS; }
    [[nodiscard]] Ref<VertexShader> MaskVS() const { return m_maskingVS; }
    [[nodiscard]] Ref<VertexShader> BasicShadowCasterVS() const { return m_basicShadowCasterVS; }
    [[nodiscard]] Ref<VertexShader> CascadeOmniShadowCasterVS() const { return m_casOmniShadowCasterVS; }

    // Pixel Shader
    [[nodiscard]] Ref<PixelShader> BlurDownPS() const { return m_blurDownPS; }
    [[nodiscard]] Ref<PixelShader> BlurUpPS() const { return m_blurUpPS; }
    [[nodiscard]] Ref<PixelShader> CombinePS() const { return m_combinePS; }
    [[nodiscard]] Ref<PixelShader> DrawNormalPS() const { return m_drawNormalPS; }
    [[nodiscard]] Ref<PixelShader> PBRPS() const { return m_PBRPS; }
    [[nodiscard]] Ref<PixelShader> SamplingPS() const { return m_samplingPS; }
    [[nodiscard]] Ref<PixelShader> SkyboxPS() const { return m_skyboxPS; }
    [[nodiscard]] Ref<PixelShader> ToneMappingPS() const { return m_toneMapPS; }
    [[nodiscard]] Ref<PixelShader> MaskPS() const { return m_maskPS; }
    [[nodiscard]] Ref<PixelShader> BasicShadowCasterPS() const { return m_basicShadowCasterPS; }
    [[nodiscard]] Ref<PixelShader> CascadeShadowCasterPS() const { return m_cascadeShadowCasterPS; }
    [[nodiscard]] Ref<PixelShader> OmniShadowCasterPS() const { return m_omniShadowCasterPS; }

    // Geometry Shader
    [[nodiscard]] Ref<GeometryShader> DrawNormalGS() const { return m_drawNormalGS; }
    [[nodiscard]] Ref<GeometryShader> CascadeShadowCasterGS() const { return m_cascadeShadowCasterGS; }
    [[nodiscard]] Ref<GeometryShader> OmniShadowCasterGS() const { return m_omniShadowCasterGS; }

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
    Ref<PixelShader> m_toneMapPS;
    Ref<PixelShader> m_maskPS;

    Ref<PixelShader> m_basicShadowCasterPS;
    Ref<PixelShader> m_cascadeShadowCasterPS;
    Ref<PixelShader> m_omniShadowCasterPS;

    // Geometry shader
    Ref<GeometryShader> m_drawNormalGS;
    Ref<GeometryShader> m_cascadeShadowCasterGS;
    Ref<GeometryShader> m_omniShadowCasterGS;
};

}   // namespace crab