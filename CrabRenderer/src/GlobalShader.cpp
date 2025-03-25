#include "InternalPCH.h"

#include "GlobalShader.h"

#include "InputLayout.h"
#include "RenderFactory.h"
#include "Shaders.h"

namespace crab
{

struct GlobalShader::Impl
{
    // Vertex Shader
    Ref<VertexShader> stencilMaskVS;
    Ref<VertexShader> debugNormalVS;
    Ref<VertexShader> screenSpaceVS;
    Ref<VertexShader> PBRGeometryVS;
    Ref<VertexShader> shadowCasterVS;
    Ref<VertexShader> omniCascadeShadowCasterVS;
    Ref<VertexShader> skyboxVS;

    // Pixel Shader
    Ref<PixelShader> stencilMaskPS;
    Ref<PixelShader> debugNormalPS;
    Ref<PixelShader> skyboxPS;
    Ref<PixelShader> shadowCasterPS;
    Ref<PixelShader> cascadeShadowCasterPS;
    Ref<PixelShader> omniShadowCasterPS;
    Ref<PixelShader> samplingPS;
    Ref<PixelShader> blurDownPS;
    Ref<PixelShader> blurUpPS;
    Ref<PixelShader> combinePS;
    Ref<PixelShader> toneMapPS;
    Ref<PixelShader> PBRGeometryPS;

    // Geometry Shader
    Ref<GeometryShader> debugNormalGS;
    Ref<GeometryShader> cascadeShadowCasterGS;
    Ref<GeometryShader> omniShadowCasterGS;
};

Scope<GlobalShader::Impl> GlobalShader::m_impl = nullptr;

//===================================================

void GlobalShader::Init(const std::filesystem::path& in_rendererDefaultDirectory)
{
    using namespace factory;

    m_impl = CreateScope<Impl>();

    //===================================================
    // InputLayout
    //===================================================

    InputLayoutBuilder builder;
    builder
        .AddElement("POSITION", 0, eFormat::Float32_3)
        .AddElement("NORMAL", 0, eFormat::Float32_3)
        .AddElement("TEXCOORD", 0, eFormat::Float32_2)
        .AddElement("TANGENT", 0, eFormat::Float32_3);

    Ref<InputLayout> pos3NormalTexTan = builder.Build();

    builder.Reset();
    builder
        .AddElement("POSITION", 0, eFormat::Float32_2)
        .AddElement("TEXCOORD", 0, eFormat::Float32_2);

    Ref<InputLayout> pos2Tex = builder.Build();

    std::filesystem::path extraDir = in_rendererDefaultDirectory / "HLSL";

    //===================================================
    // Vertex Shader
    //===================================================

    m_impl->stencilMaskVS = CreateVertexShaderFromHLSL(
        extraDir / "StencilMaskShader.hlsl",
        "VSmain",
        ShaderMacros::EmptyMacro(),
        pos3NormalTexTan);

    m_impl->debugNormalVS = CreateVertexShaderFromHLSL(
        extraDir / "DebugNormalShader.hlsl",
        "VSmain",
        ShaderMacros::EmptyMacro(),
        pos3NormalTexTan);

    m_impl->screenSpaceVS = CreateVertexShaderFromHLSL(
        extraDir / "PostProcessVS.hlsl",
        "VSmain",
        ShaderMacros::EmptyMacro(),
        pos2Tex);

    m_impl->PBRGeometryVS = CreateVertexShaderFromHLSL(
        extraDir / "PBRGeometryShader.hlsl",
        "VSmain",
        ShaderMacros::EmptyMacro(),
        pos3NormalTexTan);

    m_impl->shadowCasterVS = CreateVertexShaderFromHLSL(
        extraDir / "ShaderCastShader.hlsl",
        "VSmain",
        ShaderMacros { { "BASIC_SHADOW", "" } },
        pos3NormalTexTan);

    m_impl->omniCascadeShadowCasterVS = CreateVertexShaderFromHLSL(
        extraDir / "ShaderCastShader.hlsl",
        "VSmain",
        ShaderMacros::EmptyMacro(),
        pos3NormalTexTan);

    m_impl->skyboxVS = CreateVertexShaderFromHLSL(
        extraDir / "SkyboxShader.hlsl",
        "VSmain",
        ShaderMacros::EmptyMacro(),
        pos3NormalTexTan);

    //===================================================
    // Pixel Shader
    //===================================================

    m_impl->stencilMaskPS = CreatePixelShaderFromHLSL(
        extraDir / "StencilMaskShader.hlsl",
        "PSmain",
        ShaderMacros::EmptyMacro());

    m_impl->debugNormalPS = CreatePixelShaderFromHLSL(
        extraDir / "DebugNormalShader.hlsl",
        "PSmain",
        ShaderMacros::EmptyMacro());

    m_impl->skyboxPS = CreatePixelShaderFromHLSL(
        extraDir / "SkyboxShader.hlsl",
        "PSmain",
        ShaderMacros::EmptyMacro());

    m_impl->shadowCasterPS = CreatePixelShaderFromHLSL(
        extraDir / "ShaderCastShader.hlsl",
        "PSmain",
        ShaderMacros { { "BASIC_SHADOW", "" } });

    m_impl->cascadeShadowCasterPS = CreatePixelShaderFromHLSL(
        extraDir / "ShaderCastShader.hlsl",
        "PSmain",
        ShaderMacros { { "CASCADE_SHADOW", "" } });

    m_impl->omniShadowCasterPS = CreatePixelShaderFromHLSL(
        extraDir / "ShaderCastShader.hlsl",
        "PSmain",
        ShaderMacros { { "OMNI_SHADOW", "" } });

    m_impl->samplingPS = CreatePixelShaderFromHLSL(
        extraDir / "SamplingPS.hlsl",
        "PSmain",
        ShaderMacros::EmptyMacro());

    m_impl->blurDownPS = CreatePixelShaderFromHLSL(
        extraDir / "BlurDownPS.hlsl",
        "PSmain",
        ShaderMacros::EmptyMacro());

    m_impl->blurUpPS = CreatePixelShaderFromHLSL(
        extraDir / "BlurUpPS.hlsl",
        "PSmain",
        ShaderMacros::EmptyMacro());

    m_impl->combinePS = CreatePixelShaderFromHLSL(
        extraDir / "BlurCombinePS.hlsl",
        "PSmain",
        ShaderMacros::EmptyMacro());

    m_impl->toneMapPS = CreatePixelShaderFromHLSL(
        extraDir / "ToneMapPS.hlsl",
        "PSmain",
        ShaderMacros::EmptyMacro());

    m_impl->PBRGeometryPS = CreatePixelShaderFromHLSL(
        extraDir / "PBRGeometryShader.hlsl",
        "PSmain",
        ShaderMacros::EmptyMacro());

    //===================================================
    // Geometry Shader
    //===================================================

    m_impl->debugNormalGS = CreateGeometryShaderFromHLSL(
        extraDir / "DebugNormalShader.hlsl",
        "GSmain",
        ShaderMacros::EmptyMacro());

    m_impl->cascadeShadowCasterGS = CreateGeometryShaderFromHLSL(
        extraDir / "ShaderCastShader.hlsl",
        "GSmain",
        ShaderMacros { { "CASCADE_SHADOW", "" } });

    m_impl->omniShadowCasterGS = CreateGeometryShaderFromHLSL(
        extraDir / "ShaderCastShader.hlsl",
        "GSmain",
        ShaderMacros { { "OMNI_SHADOW", "" } });
}

Ref<VertexShader> GlobalShader::GetStencilMaskVS()
{
    return m_impl->stencilMaskVS;
}

Ref<VertexShader> GlobalShader::GetDebugNormalVS()
{
    return m_impl->debugNormalVS;
}

Ref<VertexShader> GlobalShader::GetScreenSpaceVS()
{
    return m_impl->screenSpaceVS;
}

Ref<VertexShader> GlobalShader::GetPBRGeometryVS()
{
    return m_impl->PBRGeometryVS;
}

Ref<VertexShader> GlobalShader::GetShadowCasterVS()
{
    return m_impl->shadowCasterVS;
}

Ref<VertexShader> GlobalShader::GetOmniCascadeShadowCasterVS()
{
    return m_impl->omniCascadeShadowCasterVS;
}

Ref<VertexShader> GlobalShader::GetSkyboxVS()
{
    return m_impl->skyboxVS;
}

Ref<PixelShader> GlobalShader::GetStencilMaskPS()
{
    return m_impl->stencilMaskPS;
}

Ref<PixelShader> GlobalShader::GetDebugNormalPS()
{
    return m_impl->debugNormalPS;
}

Ref<PixelShader> GlobalShader::GetSkyboxPS()
{
    return m_impl->skyboxPS;
}

Ref<PixelShader> GlobalShader::GetShadowCasterPS()
{
    return m_impl->shadowCasterPS;
}

Ref<PixelShader> GlobalShader::GetCascadeShadowCasterPS()
{
    return m_impl->cascadeShadowCasterPS;
}

Ref<PixelShader> GlobalShader::GetOmniShadowCasterPS()
{
    return m_impl->omniShadowCasterPS;
}

Ref<PixelShader> GlobalShader::GetSamplingPS()
{
    return m_impl->samplingPS;
}

Ref<PixelShader> GlobalShader::GetBlurDownPS()
{
    return m_impl->blurDownPS;
}

Ref<PixelShader> GlobalShader::GetBlurUpPS()
{
    return m_impl->blurUpPS;
}

Ref<PixelShader> GlobalShader::GetCombinePS()
{
    return m_impl->combinePS;
}

Ref<PixelShader> GlobalShader::GetToneMapPS()
{
    return m_impl->toneMapPS;
}

Ref<PixelShader> GlobalShader::GetPBRGeometryPS()
{
    return m_impl->PBRGeometryPS;
}

Ref<GeometryShader> GlobalShader::GetDebugNormalGS()
{
    return m_impl->debugNormalGS;
}

Ref<GeometryShader> GlobalShader::GetCascadeShadowCasterGS()
{
    return m_impl->cascadeShadowCasterGS;
}

Ref<GeometryShader> GlobalShader::GetOmniShadowCasterGS()
{
    return m_impl->omniShadowCasterGS;
}

}   // namespace crab