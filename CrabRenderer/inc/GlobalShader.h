#pragma once

namespace crab
{

class VertexShader;
class PixelShader;
class GeometryShader;

class GlobalShader
{
public:
    static void Init(const std::filesystem::path& in_rendererDefaultDirectory);

    // Vertex Shader
    static Ref<VertexShader> GetStencilMaskVS();
    static Ref<VertexShader> GetDebugNormalVS();
    static Ref<VertexShader> GetScreenSpaceVS();
    static Ref<VertexShader> GetPBRGeometryVS();
    static Ref<VertexShader> GetShadowCasterVS();
    static Ref<VertexShader> GetOmniCascadeShadowCasterVS();
    static Ref<VertexShader> GetSkyboxVS();

    // Pixel Shader
    static Ref<PixelShader> GetStencilMaskPS();
    static Ref<PixelShader> GetDebugNormalPS();
    static Ref<PixelShader> GetSkyboxPS();
    static Ref<PixelShader> GetShadowCasterPS();
    static Ref<PixelShader> GetCascadeShadowCasterPS();
    static Ref<PixelShader> GetOmniShadowCasterPS();
    static Ref<PixelShader> GetSamplingPS();
    static Ref<PixelShader> GetBlurDownPS();
    static Ref<PixelShader> GetBlurUpPS();
    static Ref<PixelShader> GetCombinePS();
    static Ref<PixelShader> GetToneMapPS();
    static Ref<PixelShader> GetPBRGeometryPS();

    // Geometry Shader
    static Ref<GeometryShader> GetDebugNormalGS();
    static Ref<GeometryShader> GetCascadeShadowCasterGS();
    static Ref<GeometryShader> GetOmniShadowCasterGS();

private:
    struct Impl;
    static Scope<Impl> m_impl;
};

}   // namespace crab