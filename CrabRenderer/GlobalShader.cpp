#include "CrabPch.h"

#include "GlobalShader.h"

#include "InputElements.h"
#include "Shaders.h"

namespace crab
{

void GlobalShader::Init(const std::filesystem::path& in_engineDirectory)
{
    // Vertex shader
    InputElements pos3NorTexTan;
    pos3NorTexTan
        .Add("POSITION", 0, eFormat::Float32x3)
        .Add("NORMAL", 0, eFormat::Float32x3)
        .Add("TEXCOORD", 0, eFormat::Float32x2)
        .Add("TANGENT", 0, eFormat::Float32x3);

    InputElements pos2Tex;
    pos2Tex
        .Add("POSITION", 0, eFormat::Float32x2)
        .Add("TEXCOORD", 0, eFormat::Float32x2);

    std::filesystem::path shaderPath = in_engineDirectory / "Shaders";

    m_drawNormalVS = CreateRef<VertexShader>();
    m_drawNormalVS->LoadFromHLSL(shaderPath / "DrawNormalShader.hlsl", "VSmain", pos3NorTexTan);

    m_PBRVS = CreateRef<VertexShader>();
    m_PBRVS->LoadFromHLSL(shaderPath / "PBRShader.hlsl", "VSmain", pos3NorTexTan);

    m_skyboxVS = CreateRef<VertexShader>();
    m_skyboxVS->LoadFromHLSL(shaderPath / "SkyboxShader.hlsl", "VSmain", pos3NorTexTan);

    m_postProcessVS = CreateRef<VertexShader>();
    m_postProcessVS->LoadFromHLSL(shaderPath / "PostProcessVS.hlsl", "main", pos2Tex);

    m_maskingVS = CreateRef<VertexShader>();
    m_maskingVS->LoadFromHLSL(shaderPath / "MaskingShader.hlsl", "VSmain", pos3NorTexTan);

    m_basicShadowCasterVS = CreateRef<VertexShader>();
    m_basicShadowCasterVS->LoadFromHLSL(shaderPath / "ShadowShader.hlsl", "VSmain", pos3NorTexTan, ShaderMacros { { "BASIC_SHADOW", "" } });

    m_casOmniShadowCasterVS = CreateRef<VertexShader>();
    m_casOmniShadowCasterVS->LoadFromHLSL(shaderPath / "ShadowShader.hlsl", "VSmain", pos3NorTexTan);

    // Pixel shader
    m_blurDownPS = CreateRef<PixelShader>();
    m_blurDownPS->LoadFromHLSL(shaderPath / "BlurDownPS.hlsl", "main");

    m_blurUpPS = CreateRef<PixelShader>();
    m_blurUpPS->LoadFromHLSL(shaderPath / "BlurUpPS.hlsl", "main");

    m_combinePS = CreateRef<PixelShader>();
    m_combinePS->LoadFromHLSL(shaderPath / "CombinePS.hlsl", "main");

    m_drawNormalPS = CreateRef<PixelShader>();
    m_drawNormalPS->LoadFromHLSL(shaderPath / "DrawNormalShader.hlsl", "PSmain");

    m_PBRPS = CreateRef<PixelShader>();
    m_PBRPS->LoadFromHLSL(shaderPath / "PBRShader.hlsl", "PSmain");

    m_samplingPS = CreateRef<PixelShader>();
    m_samplingPS->LoadFromHLSL(shaderPath / "SamplingPS.hlsl", "main");

    m_skyboxPS = CreateRef<PixelShader>();
    m_skyboxPS->LoadFromHLSL(shaderPath / "SkyboxShader.hlsl", "PSmain");

    m_toneMapPS = CreateRef<PixelShader>();
    m_toneMapPS->LoadFromHLSL(shaderPath / "ToneMapPS.hlsl", "main");

    m_maskPS = CreateRef<PixelShader>();
    m_maskPS->LoadFromHLSL(shaderPath / "MaskingShader.hlsl", "PSmain");

    m_basicShadowCasterPS = CreateRef<PixelShader>();
    m_basicShadowCasterPS->LoadFromHLSL(shaderPath / "ShadowShader.hlsl", "PSmain", ShaderMacros { { "BASIC_SHADOW", "" } });

    m_cascadeShadowCasterPS = CreateRef<PixelShader>();
    m_cascadeShadowCasterPS->LoadFromHLSL(shaderPath / "ShadowShader.hlsl", "PSmain", ShaderMacros { { "CASCADE_SHADOW", "" } });

    m_omniShadowCasterPS = CreateRef<PixelShader>();
    m_omniShadowCasterPS->LoadFromHLSL(shaderPath / "ShadowShader.hlsl", "PSmain", ShaderMacros { { "OMNI_SHADOW", "" } });

    // Geometry shader
    m_drawNormalGS = CreateRef<GeometryShader>();
    m_drawNormalGS->LoadFromHLSL(shaderPath / "DrawNormalShader.hlsl", "GSmain");

    m_cascadeShadowCasterGS = CreateRef<GeometryShader>();
    m_cascadeShadowCasterGS->LoadFromHLSL(shaderPath / "ShadowShader.hlsl", "GSmain", ShaderMacros { { "CASCADE_SHADOW", "" } });

    m_omniShadowCasterGS = CreateRef<GeometryShader>();
    m_omniShadowCasterGS->LoadFromHLSL(shaderPath / "ShadowShader.hlsl", "GSmain", ShaderMacros { { "OMNI_SHADOW", "" } });
}

}   // namespace crab