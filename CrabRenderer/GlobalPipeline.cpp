#include "CrabPch.h"

#include "GlobalPipeline.h"

#include "D11Renderer.h"
#include "GlobalConstant.h"
#include "GlobalShader.h"
#include "GlobalState.h"
#include "PipelineState.h"
#include "RenderFactory.h"

namespace crab
{

void GlobalPipeline::Init()
{
    bool isWireframe = GetRenderer().GetMSAA().enableMSAA;

    ConstantList constants;
    SamplerList  samplers;

    //===================================================
    // Draw Normal
    //===================================================

    {
        constants.Init({
            ConstantNode { GetGlobalConstants()->GetTransform(), 0, eShaderFlags_VertexShader },
            ConstantNode { GetGlobalConstants()->GetCamera(), 1, eShaderFlags_GeometryShader },
        });

        samplers.Init({});

        m_drawNormal = CreatePipelineState(
            GetGlobalShader()->GetDrawNormalVS(),
            GetGlobalShader()->GetDrawNormalPS(),
            GetGlobalShader()->GetDrawNormalGS(),
            nullptr,
            nullptr,
            GetGlobalState()->DepthStencil_DepthTestDrawStencil(),
            GetGlobalState()->Rasterizer_CullBack(isWireframe),
            GetGlobalState()->Blend_AlphaBlendByAlphaValue(isWireframe),
            samplers,
            constants);
    }

    //===================================================
    // PBR
    //===================================================

    {
        // deferred
        constants.Init({
            ConstantNode { GetGlobalConstants()->GetTransform(), 0, eShaderFlags_VertexShader },
            ConstantNode { GetGlobalConstants()->GetCamera(), 1, eShaderFlags_VertexShader },
            ConstantNode { GetGlobalConstants()->GetMaterial(), 4, eShaderFlags_PixelShader },
        });

        samplers.Init(
            { SamplerNode { GetGlobalState()->Sampler_LinearWrap(), 0, eShaderFlags_VertexPixelShader } });

        m_PBRGeometry = CreatePipelineState(
            GetGlobalShader()->GetPBRVS(),
            GetGlobalShader()->GetPBRGeometryPS(),
            nullptr,
            nullptr,
            nullptr,
            GetGlobalState()->DepthStencil_DepthTestDrawStencil(),
            GetGlobalState()->Rasterizer_CullBack(isWireframe),
            GetGlobalState()->Blend_Opaque(isWireframe),
            samplers,
            constants);

        m_PBRGeometryWireframe = m_PBRGeometry->Clone();
        m_PBRGeometryWireframe->SetRasterizerState(GetGlobalState()->Rasterizer_Wireframe(isWireframe));

        constants.Init({
            ConstantNode { GetGlobalConstants()->GetCamera(), 1, eShaderFlags_PixelShader },
            ConstantNode { GetGlobalConstants()->GetLight(), 2, eShaderFlags_PixelShader },
            ConstantNode { GetGlobalConstants()->GetShadowReceiver(), 3, eShaderFlags_PixelShader },
        });

        samplers.Init(
            {
                SamplerNode { GetGlobalState()->Sampler_LinearClamp(), 0, eShaderFlags_PixelShader },
                SamplerNode { GetGlobalState()->Sampler_LinearWrap(), 1, eShaderFlags_PixelShader },
                SamplerNode { GetGlobalState()->Sampler_PointClamp(), 2, eShaderFlags_PixelShader },
                SamplerNode { GetGlobalState()->Sampler_ShadowComparisonLinearClamp(), 3, eShaderFlags_PixelShader },
            });

        m_PBRLighting = m_PBRGeometry->Clone();
        m_PBRLighting->SetPixelShader(GetGlobalShader()->GetPBRLightingPS());
        m_PBRLighting->SetSamplers(samplers);
        m_PBRLighting->SetConstants(constants);
    }

    //===================================================
    // GetSkybox
    //===================================================

    {
        constants.Init({
            ConstantNode { GetGlobalConstants()->GetTransform(), 0, eShaderFlags_VertexShader },
            ConstantNode { GetGlobalConstants()->GetCamera(), 1, eShaderFlags_VertexShader },
            ConstantNode { GetGlobalConstants()->GetSkybox(), 2, eShaderFlags_PixelShader },
        });

        samplers.Init({ SamplerNode { GetGlobalState()->Sampler_LinearWrap(), 0, eShaderFlags_VertexPixelShader } });

        m_skybox = CreatePipelineState(
            GetGlobalShader()->GetSkyboxVS(),
            GetGlobalShader()->GetSkyboxPS(),
            nullptr,
            nullptr,
            nullptr,
            GetGlobalState()->DepthStencil_DepthTestDrawStencil(),
            GetGlobalState()->Rasterizer_CullBack(isWireframe),
            GetGlobalState()->Blend_Opaque(isWireframe),
            samplers,
            constants);
    }

    //===================================================
    // Masking
    //===================================================

    {
        constants.Init({
            ConstantNode { GetGlobalConstants()->GetTransform(), 0, eShaderFlags_VertexShader },
            ConstantNode { GetGlobalConstants()->GetCamera(), 1, eShaderFlags_VertexShader },
        });

        samplers.Init({});

        m_mask = CreatePipelineState(
            GetGlobalShader()->GetMaskVS(),
            GetGlobalShader()->GetMaskPS(),
            nullptr,
            nullptr,
            nullptr,
            GetGlobalState()->DepthStencil_DepthTestDrawStencil(),
            GetGlobalState()->Rasterizer_CullBack(isWireframe),
            GetGlobalState()->Blend_Opaque(isWireframe),
            samplers,
            constants);

        m_maskBackface = m_mask->Clone();
        m_maskBackface->SetRasterizerState(GetGlobalState()->Rasterizer_CullFront(isWireframe));
    }

    //===================================================
    // Shadow Caster
    //===================================================

    {
        constants.Init({
            ConstantNode { GetGlobalConstants()->GetTransform(), 0, eShaderFlags_VertexShader },
            ConstantNode { GetGlobalConstants()->GetShadowCaster(), 1, eShaderFlags_VertexShader },
        });

        samplers.Init({});

        m_basicShadowCaster = CreatePipelineState(
            GetGlobalShader()->GetBasicShadowCasterVS(),
            GetGlobalShader()->GetBasicShadowCasterPS(),
            nullptr,
            nullptr,
            nullptr,
            GetGlobalState()->DepthStencil_DepthTestDrawStencil(),
            GetGlobalState()->Rasterizer_CullFront(isWireframe),
            GetGlobalState()->Blend_AlphaBlendByAlphaValue(isWireframe),
            samplers,
            constants);

        constants.Init({
            ConstantNode { GetGlobalConstants()->GetTransform(), 0, eShaderFlags_VertexShader },
            ConstantNode { GetGlobalConstants()->GetCascadeShadowCaster(), 1, eShaderFlags_GeometryShader },
        });

        m_cascadeShadowCaster = m_basicShadowCaster->Clone();
        m_cascadeShadowCaster->SetVertexShader(GetGlobalShader()->GetCascadeOmniShadowCasterVS());
        m_cascadeShadowCaster->SetGeometryShader(GetGlobalShader()->GetCascadeShadowCasterGS());
        m_cascadeShadowCaster->SetPixelShader(GetGlobalShader()->GetCascadeShadowCasterPS());
        m_cascadeShadowCaster->SetConstants(constants);

        constants.Init({
            ConstantNode { GetGlobalConstants()->GetTransform(), 0, eShaderFlags_VertexShader },
            ConstantNode { GetGlobalConstants()->GetOmniShadowCaster(), 1, eShaderFlags_GeometryShader | eShaderFlags_PixelShader },
        });

        m_omniShadowCaster = m_basicShadowCaster->Clone();
        m_omniShadowCaster->SetVertexShader(GetGlobalShader()->GetCascadeOmniShadowCasterVS());
        m_omniShadowCaster->SetGeometryShader(GetGlobalShader()->GetOmniShadowCasterGS());
        m_omniShadowCaster->SetPixelShader(GetGlobalShader()->GetOmniShadowCasterPS());
        m_omniShadowCaster->SetConstants(constants);
    }
}

}   // namespace crab