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

    m_drawNormalVS  = VertexShader::CreateFromHLSL("Shaders\\DrawNormalVS.hlsl", "main", pos3NorTexTan);
    m_PBRVS         = VertexShader::CreateFromHLSL("Shaders\\PBRVS.hlsl", "main", pos3NorTexTan);
    m_skyboxVS      = VertexShader::CreateFromHLSL("Shaders\\SkyboxVS.hlsl", "main", pos3NorTexTan);
    m_postProcessVS = VertexShader::CreateFromHLSL("Shaders\\PostProcessVS.hlsl", "main", pos2Tex);
    m_maskingVS   = VertexShader::CreateFromHLSL("Shaders\\MaskingVS.hlsl", "main", pos3NorTexTan);

    // Pixel shader
    m_blurDownPS       = PixelShader::CreateFromHLSL("Shaders\\BlurDownPS.hlsl", "main");
    m_blurUpPS         = PixelShader::CreateFromHLSL("Shaders\\BlurUpPS.hlsl", "main");
    m_combinePS        = PixelShader::CreateFromHLSL("Shaders\\CombinePS.hlsl", "main");
    m_drawNormalPS     = PixelShader::CreateFromHLSL("Shaders\\DrawNormalPS.hlsl", "main");
    m_PBRPS            = PixelShader::CreateFromHLSL("Shaders\\PBRPS.hlsl", "main");
    m_samplingPS       = PixelShader::CreateFromHLSL("Shaders\\SamplingPS.hlsl", "main");
    m_skyboxPS         = PixelShader::CreateFromHLSL("Shaders\\SkyboxPS.hlsl", "main");
    m_toneMappingPS    = PixelShader::CreateFromHLSL("Shaders\\ToneMapPS.hlsl", "main");
    m_maskingPixelShader      = PixelShader::CreateFromHLSL("Shaders\\MaskingPS.hlsl", "main");
    m_postEffectPS     = PixelShader::CreateFromHLSL("Shaders\\PostEffectPS.hlsl", "main");
    m_debugNormalMapPS = PixelShader::CreateFromHLSL("Shaders\\DebugNormalMappingPS.hlsl", "main");

    // Geometry shader
    m_drawNormalGS = GeometryShader::CreateFromHLSL("Shaders\\DrawNormalGS.hlsl", "main");
}