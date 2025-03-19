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
    NODISCARD Ref<VertexShader> GetDrawNormalVS() const { return m_drawNormalVS; }
    NODISCARD Ref<VertexShader> GetSkyboxVS() const { return m_skyboxVS; }
    NODISCARD Ref<VertexShader> GetScreenSpaceVS() const { return m_screenSpaceVS; }
    NODISCARD Ref<VertexShader> GetPBRVS() const { return m_PBRVS; }
    NODISCARD Ref<VertexShader> GetMaskVS() const { return m_maskVS; }
    NODISCARD Ref<VertexShader> GetBasicShadowCasterVS() const { return m_basicShadowCasterVS; }
    NODISCARD Ref<VertexShader> GetCascadeOmniShadowCasterVS() const { return m_cascadeOmniShadowCasterVS; }

    // Pixel Shader
    NODISCARD Ref<PixelShader> GetBlurDownPS() const { return m_blurDownPS; }
    NODISCARD Ref<PixelShader> GetBlurUpPS() const { return m_blurUpPS; }
    NODISCARD Ref<PixelShader> GetCombinePS() const { return m_combinePS; }
    NODISCARD Ref<PixelShader> GetDrawNormalPS() const { return m_drawNormalPS; }
    NODISCARD Ref<PixelShader> GetSamplingPS() const { return m_samplingPS; }
    NODISCARD Ref<PixelShader> GetSkyboxPS() const { return m_skyboxPS; }
    NODISCARD Ref<PixelShader> GetToneMapPS() const { return m_toneMapPS; }
    NODISCARD Ref<PixelShader> GetMaskPS() const { return m_maskPS; }
    NODISCARD Ref<PixelShader> GetPBRGeometryPS() const { return m_PBRGeometryPS; }
    NODISCARD Ref<PixelShader> GetPBRLightingPS() const { return m_PBRLightingPS; }

    NODISCARD Ref<PixelShader> GetBasicShadowCasterPS() const { return m_basicShadowCasterPS; }
    NODISCARD Ref<PixelShader> GetCascadeShadowCasterPS() const { return m_cascadeShadowCasterPS; }
    NODISCARD Ref<PixelShader> GetOmniShadowCasterPS() const { return m_omniShadowCasterPS; }

    // Geometry Shader
    NODISCARD Ref<GeometryShader> GetDrawNormalGS() const { return m_drawNormalGS; }
    NODISCARD Ref<GeometryShader> GetCascadeShadowCasterGS() const { return m_cascadeShadowCasterGS; }
    NODISCARD Ref<GeometryShader> GetOmniShadowCasterGS() const { return m_omniShadowCasterGS; }

private:
    // Vertex shader
    Ref<VertexShader> m_drawNormalVS;
    Ref<VertexShader> m_skyboxVS;
    Ref<VertexShader> m_screenSpaceVS;
    Ref<VertexShader> m_PBRVS;
    Ref<VertexShader> m_maskVS;
    Ref<VertexShader> m_basicShadowCasterVS;
    Ref<VertexShader> m_cascadeOmniShadowCasterVS;

    // Pixel shader
    Ref<PixelShader> m_blurDownPS;
    Ref<PixelShader> m_blurUpPS;
    Ref<PixelShader> m_combinePS;
    Ref<PixelShader> m_drawNormalPS;
    Ref<PixelShader> m_samplingPS;
    Ref<PixelShader> m_skyboxPS;
    Ref<PixelShader> m_toneMapPS;
    Ref<PixelShader> m_maskPS;
    Ref<PixelShader> m_PBRGeometryPS;
    Ref<PixelShader> m_PBRLightingPS;

    Ref<PixelShader> m_basicShadowCasterPS;
    Ref<PixelShader> m_cascadeShadowCasterPS;
    Ref<PixelShader> m_omniShadowCasterPS;

    // Geometry shader
    Ref<GeometryShader> m_drawNormalGS;
    Ref<GeometryShader> m_cascadeShadowCasterGS;
    Ref<GeometryShader> m_omniShadowCasterGS;
};

}   // namespace crab