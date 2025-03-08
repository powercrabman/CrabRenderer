#include "pch.h"

#include "GlobalShader.h"

void GlobalShader::Init()
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

    m_drawNormalVS          = VertexShader::CreateFromHLSL("DrawNormalShader.hlsl", "VSmain", pos3NorTexTan);
    m_PBRVS                 = VertexShader::CreateFromHLSL("Shaders\\PBRShader.hlsl", "VSmain", pos3NorTexTan);
    m_skyboxVS              = VertexShader::CreateFromHLSL("Shaders\\SkyboxShader.hlsl", "VSmain", pos3NorTexTan);
    m_postProcessVS         = VertexShader::CreateFromHLSL("Shaders\\PostProcessVS.hlsl", "main", pos2Tex);
    m_maskingVS             = VertexShader::CreateFromHLSL("Shaders\\MaskingShader.hlsl", "VSmain", pos3NorTexTan);

    m_basicShadowCasterVS   = VertexShader::CreateFromHLSL("ShadowShader.hlsl", "VSmain", pos3NorTexTan, ShaderMacros { { "BASIC_SHADOW", "" } });
    m_casOmniShadowCasterVS = VertexShader::CreateFromHLSL("ShadowShader.hlsl", "VSmain", pos3NorTexTan);

    // Pixel shader
    m_blurDownPS           = PixelShader::CreateFromHLSL("Shaders\\BlurDownPS.hlsl", "main");
    m_blurUpPS             = PixelShader::CreateFromHLSL("Shaders\\BlurUpPS.hlsl", "main");
    m_combinePS            = PixelShader::CreateFromHLSL("Shaders\\CombinePS.hlsl", "main");
    m_drawNormalPS         = PixelShader::CreateFromHLSL("DrawNormalShader.hlsl", "PSmain");
    m_PBRPS                = PixelShader::CreateFromHLSL("Shaders\\PBRShader.hlsl", "PSmain");
    m_samplingPS           = PixelShader::CreateFromHLSL("Shaders\\SamplingPS.hlsl", "main");
    m_skyboxPS             = PixelShader::CreateFromHLSL("Shaders\\SkyboxShader.hlsl", "PSmain");
    m_toneMappingPS        = PixelShader::CreateFromHLSL("Shaders\\ToneMapPS.hlsl", "main");
    m_maskingPixelShader   = PixelShader::CreateFromHLSL("Shaders\\MaskingShader.hlsl", "PSmain");
    m_depthVisualizePS     = PixelShader::CreateFromHLSL("DepthVisualizePS.hlsl", "main");
    m_basicShadowCasterPS  = PixelShader::CreateFromHLSL("ShadowShader.hlsl", "PSmain", ShaderMacros { { "BASIC_SHADOW", "" } });
    m_cascadeShadowCasterPS = PixelShader::CreateFromHLSL("ShadowShader.hlsl", "PSmain", ShaderMacros { { "CASCADE_SHADOW", "" } });
    m_omniShadowCasterPS    = PixelShader::CreateFromHLSL("ShadowShader.hlsl", "PSmain", ShaderMacros { { "OMNI_SHADOW", "" } });

    // Geometry shader
    m_drawNormalGS = GeometryShader::CreateFromHLSL("DrawNormalShader.hlsl", "GSmain");
    m_cascadeShadowCasterGS = GeometryShader::CreateFromHLSL("ShadowShader.hlsl", "GSmain", ShaderMacros { { "CASCADE_SHADOW", "" } });
    m_omniShadowCasterGS    = GeometryShader::CreateFromHLSL("ShadowShader.hlsl", "GSmain", ShaderMacros { { "OMNI_SHADOW", "" } });
}