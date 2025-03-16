#include "CrabPch.h"

#include "GlobalPipeline.h"

#include "D11Renderer.h"
#include "GlobalConstant.h"
#include "GlobalShader.h"
#include "GlobalState.h"
#include "PipelineState.h"
#include "RenderResourceFactory.h"

namespace crab
{

void GlobalPipeline::Init()
{
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
            GetGlobalShader()->DrawNormalVS(),
            GetGlobalShader()->DrawNormalPS(),
            GetGlobalShader()->DrawNormalGS(),
            nullptr,
            nullptr,
            GetGlobalState()->DepthStencil_DepthTestDrawStencil(),
            GetGlobalState()->Rasterizer_CullBack(true),
            GetGlobalState()->Blend_AlphaBlendByAlphaValue(true),
            samplers,
            constants);
    }

    //===================================================
    // PBR
    //===================================================

    {
        constants.Init({
            ConstantNode { GetGlobalConstants()->GetTransform(), 0, eShaderFlags_VertexShader },
            ConstantNode { GetGlobalConstants()->GetCamera(), 1, eShaderFlags_VertexPixelShader },
            ConstantNode { GetGlobalConstants()->GetLight(), 2, eShaderFlags_PixelShader },
            ConstantNode { GetGlobalConstants()->GetShadowReceiver(), 3, eShaderFlags_PixelShader },
            ConstantNode { GetGlobalConstants()->GetMaterial(), 4, eShaderFlags_PixelShader },
        });

        samplers.Init(
            { SamplerNode { GetGlobalState()->Sampler_LinearWrap(), 0, eShaderFlags_VertexPixelShader },
              SamplerNode { GetGlobalState()->Sampler_LinearClamp(), 1, eShaderFlags_PixelShader },
              SamplerNode { GetGlobalState()->Sampler_ShadowComparisonLinearClamp(), 2, eShaderFlags_PixelShader } });

        m_pbr = CreatePipelineState(
            GetGlobalShader()->PBRVS(),
            GetGlobalShader()->PBRPS(),
            nullptr,
            nullptr,
            nullptr,
            GetGlobalState()->DepthStencil_DepthTestDrawStencil(),
            GetGlobalState()->Rasterizer_CullBack(true),
            GetGlobalState()->Blend_AlphaBlendByAlphaValue(true),
            samplers,
            constants);

        m_pbrWireframe = m_pbr->Clone();
        m_pbrWireframe->SetRasterizerState(GetGlobalState()->Rasterizer_Wireframe(true));

        constants.Init({
            ConstantNode { GetGlobalConstants()->GetTransform(), 0, eShaderFlags_VertexShader },
            ConstantNode { GetGlobalConstants()->GetReflectCamera(), 1, eShaderFlags_VertexPixelShader },
            ConstantNode { GetGlobalConstants()->GetLight(), 2, eShaderFlags_PixelShader },
            ConstantNode { GetGlobalConstants()->GetShadowReceiver(), 3, eShaderFlags_PixelShader },
            ConstantNode { GetGlobalConstants()->GetMaterial(), 4, eShaderFlags_PixelShader },
        });
        m_pbrReflection = m_pbr->Clone();
        m_pbrReflection->SetConstants(constants);
        m_pbrReflection->SetDepthStencilState(GetGlobalState()->DepthStencil_DepthTestDrawOnStencil());
        m_pbrReflection->SetRasterizerState(GetGlobalState()->Rasterizer_CullFront(true));

        m_pbrRefractionWireframe = m_pbrReflection->Clone();
        m_pbrRefractionWireframe->SetDepthStencilState(GetGlobalState()->DepthStencil_DepthTestDrawStencil());
        m_pbrRefractionWireframe->SetRasterizerState(GetGlobalState()->Rasterizer_Wireframe(true));

        m_pbrOnMask = m_pbr->Clone();
        m_pbrOnMask->SetDepthStencilState(GetGlobalState()->DepthStencil_DepthTestDrawOnStencil());

        m_pbrWireframeOnMask = m_pbrOnMask->Clone();
        m_pbrWireframeOnMask->SetRasterizerState(GetGlobalState()->Rasterizer_Wireframe(true));
    }

    //===================================================
    // Skybox
    //===================================================

    {
        constants.Init({
            ConstantNode { GetGlobalConstants()->GetTransform(), 0, eShaderFlags_VertexShader },
            ConstantNode { GetGlobalConstants()->GetCamera(), 1, eShaderFlags_VertexShader },
            ConstantNode { GetGlobalConstants()->GetSkybox(), 2, eShaderFlags_PixelShader },
        });

        samplers.Init({ SamplerNode { GetGlobalState()->Sampler_LinearWrap(), 0, eShaderFlags_VertexPixelShader } });

        m_skybox = CreatePipelineState(
            GetGlobalShader()->SkyboxVS(),
            GetGlobalShader()->SkyboxPS(),
            nullptr,
            nullptr,
            nullptr,
            GetGlobalState()->DepthStencil_DepthTestDrawStencil(),
            GetGlobalState()->Rasterizer_CullBack(true),
            GetGlobalState()->Blend_Opaque(true),
            samplers,
            constants);

        constants.Init({
            ConstantNode { GetGlobalConstants()->GetTransform(), 0, eShaderFlags_VertexShader },
            ConstantNode { GetGlobalConstants()->GetReflectCamera(), 1, eShaderFlags_VertexShader },
            ConstantNode { GetGlobalConstants()->GetSkybox(), 2, eShaderFlags_PixelShader },
        });

        m_skyboxReflection = m_skybox->Clone();
        m_skyboxReflection->SetConstants(constants);
        m_skyboxReflection->SetDepthStencilState(GetGlobalState()->DepthStencil_DepthTestDrawOnStencil());
        m_skyboxReflection->SetRasterizerState(GetGlobalState()->Rasterizer_CullFront(true));
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
            GetGlobalShader()->MaskVS(),
            GetGlobalShader()->MaskPS(),
            nullptr,
            nullptr,
            nullptr,
            GetGlobalState()->DepthStencil_DepthTestDrawStencil(),
            GetGlobalState()->Rasterizer_CullBack(true),
            GetGlobalState()->Blend_Opaque(true),
            samplers,
            constants);

        m_maskBackface = m_mask->Clone();
        m_maskBackface->SetRasterizerState(GetGlobalState()->Rasterizer_CullFront(true));
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
            GetGlobalShader()->BasicShadowCasterVS(),
            GetGlobalShader()->BasicShadowCasterPS(),
            nullptr,
            nullptr,
            nullptr,
            GetGlobalState()->DepthStencil_DepthTestDrawStencil(),
            GetGlobalState()->Rasterizer_CullFront(true),
            GetGlobalState()->Blend_AlphaBlendByAlphaValue(true),
            samplers,
            constants);

        constants.Init({
            ConstantNode { GetGlobalConstants()->GetTransform(), 0, eShaderFlags_VertexShader },
            ConstantNode { GetGlobalConstants()->GetCascadeShadowCaster(), 1, eShaderFlags_GeometryShader },
        });

        m_cascadeShadowCaster = m_basicShadowCaster->Clone();
        m_cascadeShadowCaster->SetVertexShader(GetGlobalShader()->CascadeOmniShadowCasterVS());
        m_cascadeShadowCaster->SetGeometryShader(GetGlobalShader()->CascadeShadowCasterGS());
        m_cascadeShadowCaster->SetPixelShader(GetGlobalShader()->CascadeShadowCasterPS());
        m_cascadeShadowCaster->SetConstants(constants);

        constants.Init({
            ConstantNode { GetGlobalConstants()->GetTransform(), 0, eShaderFlags_VertexShader },
            ConstantNode { GetGlobalConstants()->GetOmniShadowCaster(), 1, eShaderFlags_GeometryShader | eShaderFlags_PixelShader },
        });

        m_omniShadowCaster = m_basicShadowCaster->Clone();
        m_omniShadowCaster->SetVertexShader(GetGlobalShader()->CascadeOmniShadowCasterVS());
        m_omniShadowCaster->SetGeometryShader(GetGlobalShader()->OmniShadowCasterGS());
        m_omniShadowCaster->SetPixelShader(GetGlobalShader()->OmniShadowCasterPS());
        m_omniShadowCaster->SetConstants(constants);
    }
}

void GlobalPipeline::BeginDrawNormal(const PipelineBindArgument& in_arg) const
{
    m_drawNormal->Bind(in_arg);
}

void GlobalPipeline::BeginPBR(const PipelineBindArgument& in_arg) const
{
    m_pbr->Bind(in_arg);
}

void GlobalPipeline::BeginPBRWireframe(const PipelineBindArgument& in_arg) const
{
    m_pbrWireframe->Bind(in_arg);
}

void GlobalPipeline::BeginPBRReflection(const PipelineBindArgument& in_arg) const
{
    m_pbrReflection->Bind(in_arg);
}

void GlobalPipeline::BeginPBRRefractionWireframe(const PipelineBindArgument& in_arg) const
{
    m_pbrRefractionWireframe->Bind(in_arg);
}

void GlobalPipeline::BeginSkyboxReflection(const PipelineBindArgument& in_arg) const
{
    m_skyboxReflection->Bind(in_arg);
}

void GlobalPipeline::BeginSkybox(const PipelineBindArgument& in_arg) const
{
    m_skybox->Bind(in_arg);
}

void GlobalPipeline::BeginMask(const PipelineBindArgument& in_arg) const
{
    m_mask->Bind(in_arg);
}

void GlobalPipeline::BeginBasicShadowCaster(const PipelineBindArgument& in_arg) const
{
    m_basicShadowCaster->Bind(in_arg);
}

void GlobalPipeline::BeginCascadeShadowCaster(const PipelineBindArgument& in_arg) const
{
    m_cascadeShadowCaster->Bind(in_arg);
}

void GlobalPipeline::BeginOmniShadowCaster(const PipelineBindArgument& in_arg) const
{
    m_omniShadowCaster->Bind(in_arg);
}
void GlobalPipeline::BeginMaskBackface(const PipelineBindArgument& in_arg) const
{
    m_maskBackface->Bind(in_arg);
}
}   // namespace crab