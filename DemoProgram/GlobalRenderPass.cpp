#include "pch.h"

#include "GlobalRenderPass.h"

void GlobalRenderPass::Init()
{
    auto& shader   = GetShaderLibrary();
    auto& constant = GetGlobalConstants();
    auto* state    = GetCommonState();

    // Draw Normal State
    {
        ConstantList cb;
        cb
            .Add(constant.GetTransformConstant(), 0, eShaderFlags_GeometryShader)
            .Add(constant.GetCameraConstant(), 1, eShaderFlags_GeometryShader)
            .Add(constant.GetDrawNormalFactor(), 2, eShaderFlags_GeometryShader);

        m_drawNormalPass = RenderPass::Create(
            shader.DrawNormalVertexShader(),
            shader.DrawNormalPixelShader(),
            shader.DrawNormalGeometryShader(),
            nullptr,
            nullptr,
            state->DepthStencil_DepthTest(),
            state->Rasterizer_CullCounterClockwise(true),
            state->Blend_Opaque(true),
            {},
            cb);
    }

    // PBR State
    {
        SamplerList samplers;
        samplers
            .Add(state->Sampler_LinearWrap(), 0, eShaderFlags_VertexPixelShader)
            .Add(state->Sampler_LinearClamp(), 1, eShaderFlags_PixelShader)
            .Add(state->Sampler_ShadowComparisonPointClamp(), 2, eShaderFlags_PixelShader);

        ConstantList constants;
        constants
            .Add(constant.GetTransformConstant(), 0, eShaderFlags_VertexPixelShader)
            .Add(constant.GetCameraConstant(), 1, eShaderFlags_VertexPixelShader)
            .Add(constant.GetLightConstant(), 2, eShaderFlags_PixelShader)
            .Add(constant.GetCascadeShadowConstant(), 3, eShaderFlags_PixelShader)
            .Add(constant.GetMaterialConstant(), 4, eShaderFlags_VertexPixelShader);

        // PBR
        m_pbrPass = RenderPass::Create(
            shader.PBRVertexShader(),
            shader.PBRPixelShader(),
            nullptr,
            nullptr,
            nullptr,
            state->DepthStencil_DepthTestDrawOnStencil(),
            state->Rasterizer_CullCounterClockwise(true),
            state->Blend_AlphaBlendByAlphaValue(true),
            samplers,
            constants);

        // PBR Wireframe
        m_pbrWireframePass = RenderPass::Create(
            shader.PBRVertexShader(),
            shader.PBRPixelShader(),
            nullptr,
            nullptr,
            nullptr,
            state->DepthStencil_DepthTestDrawOnStencil(),
            state->Rasterizer_Wireframe(true),
            state->Blend_AlphaBlendByAlphaValue(true),
            samplers,
            constants);

        // Mirror
        ConstantList mirrorConstants;
        mirrorConstants
            .Add(constant.GetTransformConstant(), 0, eShaderFlags_VertexPixelShader)
            .Add(constant.GetReflectCameraConstant(), 1, eShaderFlags_VertexPixelShader)
            .Add(constant.GetLightConstant(), 2, eShaderFlags_PixelShader)
            .Add(constant.GetCascadeShadowConstant(), 3, eShaderFlags_PixelShader)
            .Add(constant.GetMaterialConstant(), 4, eShaderFlags_VertexPixelShader);

        m_pbrReflectPass = RenderPass::Create(
            shader.PBRVertexShader(),
            shader.PBRPixelShader(),
            nullptr,
            nullptr,
            nullptr,
            state->DepthStencil_DepthTestDrawOnStencil(),
            state->Rasterizer_CullClockwise(true),
            state->Blend_AlphaBlendByAlphaValue(true),
            samplers,
            mirrorConstants);

        // Mirror Wireframe
        m_pbrReflectWireframePass = RenderPass::Create(
            shader.PBRVertexShader(),
            shader.PBRPixelShader(),
            nullptr,
            nullptr,
            nullptr,
            state->DepthStencil_DepthTestDrawOnStencil(),
            state->Rasterizer_Wireframe(true),
            state->Blend_AlphaBlendByAlphaValue(true),
            samplers,
            mirrorConstants);
    }

    // Skybox State
    {
        SamplerList samplers;
        samplers.Add(state->Sampler_LinearClamp(), 0, eShaderFlags_PixelShader);

        ConstantList constants;
        constants
            .Add(constant.GetTransformConstant(), 0, eShaderFlags_VertexShader)
            .Add(GetGlobalConstants().GetCameraConstant(), 1, eShaderFlags_VertexShader)
            .Add(GetGlobalConstants().GetSkyboxPSConstant(), 2, eShaderFlags_PixelShader);

        // Skybox
        m_skyboxPass = RenderPass::Create(
            shader.SkyboxVertexShader(),
            shader.SkyboxPixelShader(),
            nullptr,
            nullptr,
            nullptr,
            state->DepthStencil_DepthTestDrawOnStencil(),
            state->Rasterizer_CullCounterClockwise(true),
            state->Blend_Opaque(true),
            samplers,
            constants);

        // Mirror
        constants.ClearList();
        constants
            .Add(constant.GetTransformConstant(), 0, eShaderFlags_VertexShader)
            .Add(GetGlobalConstants().GetReflectCameraConstant(), 1, eShaderFlags_VertexShader)
            .Add(GetGlobalConstants().GetSkyboxPSConstant(), 2, eShaderFlags_PixelShader);

        m_skyboxReflectPass = RenderPass::Create(
            shader.SkyboxVertexShader(),
            shader.SkyboxPixelShader(),
            nullptr,
            nullptr,
            nullptr,
            state->DepthStencil_DepthTestDrawOnStencil(),
            state->Rasterizer_CullClockwise(true),
            state->Blend_Opaque(true),
            samplers,
            constants);
    }

    // Depth Only
    {
        ConstantList consts;
        consts
            .Add(constant.GetTransformConstant(), 0, eShaderFlags_VertexShader)
            .Add(constant.GetCameraConstant(), 1, eShaderFlags_VertexShader);

        m_depthOnlyPass = RenderPass::Create(
            shader.MaskingVertexShader(),
            shader.MaskingPixelShader(),
            nullptr,
            nullptr,
            nullptr,
            state->DepthStencil_DepthTest(),
            state->Rasterizer_CullCounterClockwise(true),
            state->Blend_Opaque(true),
            {},
            consts);
    }

    // Masking Pass
    {
        ConstantList cblist;
        cblist
            .Add(GetGlobalConstants().GetTransformConstant(), 0, eShaderFlags_VertexShader)
            .Add(GetGlobalConstants().GetCameraConstant(), 1, eShaderFlags_VertexShader);

        m_mirrorMaskingPass = RenderPass::Create(
            shader.MaskingVertexShader(),
            shader.MaskingPixelShader(),
            nullptr,
            nullptr,
            nullptr,
            state->DepthStencil_DepthTestDrawStencil(),
            state->Rasterizer_CullCounterClockwise(true),
            state->Blend_Opaque(true),
            {},
            cblist);
    }

    // Depth Visualize
    {
        SamplerList samplers;
        samplers.Add(state->Sampler_LinearWrap(), 0, eShaderFlags_PixelShader);

        ConstantList cblist;
        cblist.Add(GetGlobalConstants().GetDepthVisualizeConstant(), 0, eShaderFlags_PixelShader);

        m_depthVisualizePass = RenderPass::Create(
            shader.PostProcessVertexShader(),
            shader.DepthVisualizePixelShader(),
            nullptr,
            nullptr,
            nullptr,
            state->DepthStencil_DepthTest(),
            state->Rasterizer_CullCounterClockwise(true),
            state->Blend_Opaque(true),
            samplers,
            cblist);
    }

    // Shadow Caster
    {
        ConstantList cblist;
        cblist
            .Add(constant.GetTransformConstant(), 0, eShaderFlags_VertexShader)
            .Add(constant.GetBasicShadowCasterConstant(), 1, eShaderFlags_VertexShader);

        m_basicShadowCasterPass = RenderPass::Create(
            shader.BasicShadowCasterVertexShader(),
            shader.BasicShadowCasterPixelShader(),
            nullptr,
            nullptr,
            nullptr,
            state->DepthStencil_DepthTest(),
            state->Rasterizer_CullCounterClockwise(true),
            state->Blend_Opaque(true),
            {},
            cblist);

        cblist.ClearList();
        cblist
            .Add(constant.GetTransformConstant(), 0, eShaderFlags_VertexShader)
            .Add(constant.GetCascadeShadowConstant(), 1, eShaderFlags_GeometryShader | eShaderFlags_PixelShader);

        m_cascadeShadowCasterPass = RenderPass::Create(
            shader.CascadeOmniShadowCasterVertexShader(),
            shader.CascadeShadowCasterPixelShader(),
            shader.CascadeShadowCasterGeometryShader(),
            nullptr,
            nullptr,
            state->DepthStencil_DepthTest(),
            state->Rasterizer_CullCounterClockwise(true),
            state->Blend_Opaque(true),
            {},
            cblist);

        cblist.ClearList();
        cblist
            .Add(constant.GetTransformConstant(), 0, eShaderFlags_VertexShader)
            .Add(constant.GetOmniShadowCasterConstant(), 1, eShaderFlags_GeometryShader | eShaderFlags_PixelShader);

        m_omniShadowCasterPass = RenderPass::Create(
            shader.CascadeOmniShadowCasterVertexShader(),
            shader.OmniShadowCasterPixelShader(),
            shader.OmniShadowCasterGeometryShader(),
            nullptr,
            nullptr,
            state->DepthStencil_DepthTest(),
            state->Rasterizer_CullCounterClockwise(true),
            state->Blend_Opaque(true),
            {},
            cblist);
    }
}

void GlobalRenderPass::BeginDrawNormalPass() const
{
    m_drawNormalPass->BeginPass();
}

void GlobalRenderPass::BeginPBRPass(uint32 in_stencilRef) const
{
    m_pbrPass->BeginPass(in_stencilRef);
}

void GlobalRenderPass::BeginPBRWireframePass(uint32 in_stencilRef) const
{
    m_pbrWireframePass->BeginPass(in_stencilRef);
}

void GlobalRenderPass::BeginPBRReflectWireframePass(uint32 in_stencilRef) const
{
    m_pbrReflectWireframePass->BeginPass(in_stencilRef);
}

void GlobalRenderPass::BeginPBRReflectPass(uint32 in_stencilRef) const
{
    m_pbrReflectPass->BeginPass(in_stencilRef);
}

void GlobalRenderPass::BeginSkyboxPass(uint32 in_stencilRef) const
{
    m_skyboxPass->BeginPass(in_stencilRef);
}

void GlobalRenderPass::BeginSkyboxReflectPass(uint32 in_stencilRef) const
{
    m_skyboxReflectPass->BeginPass(in_stencilRef);
}

void GlobalRenderPass::BeginDepthOnlyPass() const
{
    m_depthOnlyPass->BeginPass();
}

void GlobalRenderPass::BeginMirrorMaskingPass(uint32 in_stencilRef) const
{
    m_mirrorMaskingPass->BeginPass(in_stencilRef);
}

void GlobalRenderPass::BeginDepthVisualizePass() const
{
    m_depthVisualizePass->BeginPass();
}

void GlobalRenderPass::BeginBasicShadowCasterPass() const
{
    m_basicShadowCasterPass->BeginPass();
}

void GlobalRenderPass::BeginCascadeShadowCasterPass() const
{
    m_cascadeShadowCasterPass->BeginPass();
}

void GlobalRenderPass::BeginOmniShadowCasterPass() const
{
    m_omniShadowCasterPass->BeginPass();
}
