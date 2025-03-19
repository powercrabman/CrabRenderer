#include "CrabPch.h"

#include "GlobalShader.h"

#include "InputElements.h"
#include "RenderFactory.h"
#include "Shaders.h"

namespace crab
{

void GlobalShader::Init(const std::filesystem::path& in_engineDirectory)
{
    // Vertex shader
    InputElements pos3NorTexTan;
    pos3NorTexTan
        .Add("POSITION", 0, eFormat::Float32_3)
        .Add("NORMAL", 0, eFormat::Float32_3)
        .Add("TEXCOORD", 0, eFormat::Float32_2)
        .Add("TANGENT", 0, eFormat::Float32_3);

    InputElements pos2Tex;
    pos2Tex
        .Add("POSITION", 0, eFormat::Float32_2)
        .Add("TEXCOORD", 0, eFormat::Float32_2);

    std::filesystem::path shaderPath = in_engineDirectory / "Shaders";

    m_drawNormalVS              = RenderFactory::CreateVertexShaderFromHLSL(shaderPath / "DrawNormalShader.hlsl", "VSmain", pos3NorTexTan);
    m_skyboxVS                  = RenderFactory::CreateVertexShaderFromHLSL(shaderPath / "SkyboxShader.hlsl", "VSmain", pos3NorTexTan);
    m_maskVS                    = RenderFactory::CreateVertexShaderFromHLSL(shaderPath / "MaskShader.hlsl", "VSmain", pos3NorTexTan);
    m_basicShadowCasterVS       = RenderFactory::CreateVertexShaderFromHLSL(shaderPath / "ShaderCastShader.hlsl", "VSmain", pos3NorTexTan, ShaderMacros { { "BASIC_SHADOW", "" } });
    m_cascadeOmniShadowCasterVS = RenderFactory::CreateVertexShaderFromHLSL(shaderPath / "ShaderCastShader.hlsl", "VSmain", pos3NorTexTan);
    m_screenSpaceVS             = RenderFactory::CreateVertexShaderFromHLSL(shaderPath / "PostProcessVS.hlsl", "VSmain", pos2Tex);
    m_PBRVS                     = RenderFactory::CreateVertexShaderFromHLSL(shaderPath / "PBRGeometryShader.hlsl", "VSmain", pos3NorTexTan);

    // Pixel shader
    m_blurDownPS   = RenderFactory::CreatePixelShaderFromHLSL(shaderPath / "BlurDownPS.hlsl", "main");
    m_blurUpPS     = RenderFactory::CreatePixelShaderFromHLSL(shaderPath / "BlurUpPS.hlsl", "main");
    m_combinePS    = RenderFactory::CreatePixelShaderFromHLSL(shaderPath / "CombinePS.hlsl", "main");
    m_drawNormalPS = RenderFactory::CreatePixelShaderFromHLSL(shaderPath / "DrawNormalShader.hlsl", "PSmain");
    m_samplingPS   = RenderFactory::CreatePixelShaderFromHLSL(shaderPath / "SamplingPS.hlsl", "main");
    m_skyboxPS     = RenderFactory::CreatePixelShaderFromHLSL(shaderPath / "SkyboxShader.hlsl", "PSmain");
    m_toneMapPS    = RenderFactory::CreatePixelShaderFromHLSL(shaderPath / "ToneMapPS.hlsl", "main");
    m_maskPS       = RenderFactory::CreatePixelShaderFromHLSL(shaderPath / "MaskShader.hlsl", "PSmain");

    m_basicShadowCasterPS   = RenderFactory::CreatePixelShaderFromHLSL(shaderPath / "ShaderCastShader.hlsl", "PSmain", ShaderMacros { { "BASIC_SHADOW", "" } });
    m_cascadeShadowCasterPS = RenderFactory::CreatePixelShaderFromHLSL(shaderPath / "ShaderCastShader.hlsl", "PSmain", ShaderMacros { { "CASCADE_SHADOW", "" } });
    m_omniShadowCasterPS    = RenderFactory::CreatePixelShaderFromHLSL(shaderPath / "ShaderCastShader.hlsl", "PSmain", ShaderMacros { { "OMNI_SHADOW", "" } });

    // Geometry shader
    m_drawNormalGS          = RenderFactory::CreateGeometryShaderFromHLSL(shaderPath / "ShaderCastShader.hlsl", "GSmain");
    m_cascadeShadowCasterGS = RenderFactory::CreateGeometryShaderFromHLSL(shaderPath / "ShaderCastShader.hlsl", "GSmain", ShaderMacros { { "CASCADE_SHADOW", "" } });
    m_omniShadowCasterGS    = RenderFactory::CreateGeometryShaderFromHLSL(shaderPath / "ShaderCastShader.hlsl", "GSmain", ShaderMacros { { "OMNI_SHADOW", "" } });
}

}   // namespace crab