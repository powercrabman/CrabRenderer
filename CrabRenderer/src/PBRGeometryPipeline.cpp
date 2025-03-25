#include "InternalPCH.h"

#include "PBRGeometryPipeline.h"

#include "GlobalShader.h"
#include "GlobalState.h"
#include "Material.h"
#include "RenderFactory.h"
#include "RenderStates.h"
#include "ShaderResource.h"

namespace crab
{

PBRGeometryPipeline::PBRGeometryPipeline()
{
}

PBRGeometryPipeline::~PBRGeometryPipeline()
{
}

void PBRGeometryPipeline::Init(
    const Ref<RasterizerState>&   in_rasterizerState,
    const Ref<DepthStencilState>& in_depthStencilState,
    const Ref<BlendState>&        in_blendState,
    const Ref<RenderTarget>&      in_positionRenderTarget,
    const Ref<RenderTarget>&      in_normalRenderTarget,
    const Ref<RenderTarget>&      in_emissiveRenderTarget,
    const Ref<RenderTarget>&      in_albedoRoughnessRenderTarget,
    const Ref<RenderTarget>&      in_metallicAORenderTarget,
    const Ref<DepthBuffer>&       in_depthBuffer,
    const Viewport&               in_viewport)
{
    Ref<FrameBuffer> frameBuffer = factory::CreateFrameBuffer(
        RenderTargetArray { in_positionRenderTarget,
                            in_normalRenderTarget,
                            in_emissiveRenderTarget,
                            in_albedoRoughnessRenderTarget,
                            in_metallicAORenderTarget },
        in_depthBuffer);

    BasicPipeline::InitBasicPipeline_Internal(
        eTopology::TriangleList,
        GlobalShader::GetPBRGeometryVS(),
        GlobalShader::GetPBRGeometryPS(),
        nullptr,
        nullptr,
        nullptr,
        in_rasterizerState,
        in_depthStencilState,
        in_blendState,
        frameBuffer,
        in_viewport,
        eShaderFlags_VertexShader,
        eShaderFlags_VertexShader);

    m_materialCB  = factory::CreateConstantBuffer<CB_MATERIAL>();
    m_wrapSampler = GlobalState::Sampler_Linear_Wrap();
}

void PBRGeometryPipeline::BindPipeline(const PipelineBindArgs& in_args)
{
    PipelineBase::BindPipeline(in_args);

    m_materialCB->Bind(CB_MATERIAL_SLOT, eShaderFlags_VertexShader | eShaderFlags_PixelShader);
    m_wrapSampler->Bind(SAMPLER_LINEAR_WRAP_SLOT, eShaderFlags_VertexShader | eShaderFlags_PixelShader);
}

void PBRGeometryPipeline::SetDiffuseTextureCube(const Ref<TextureCube>& in_texture) const
{
    in_texture->Bind(TEXTURE_DIFFUSE_MAP_SLOT, eShaderFlags_PixelShader);
}

void PBRGeometryPipeline::SetSpecularTextureCube(const Ref<TextureCube>& in_texture) const
{
    in_texture->Bind(TEXTURE_SPECULAR_MAP_SLOT, eShaderFlags_PixelShader);
}

void PBRGeometryPipeline::SetBRDFLUTTexture(const Ref<Texture2D>& in_texture) const
{
    in_texture->Bind(TEXTURE_BRDF_LUT_SLOT, eShaderFlags_PixelShader);
}

void PBRGeometryPipeline::SetMaterial(const Ref<Material>& in_material) const
{
    CB_MATERIAL mat;
    mat.cb_materialAlbedo               = in_material->GetAlbedo();
    mat.cb_materialMetallic             = in_material->GetMetallic();
    mat.cb_materialEmissive             = in_material->GetEmissive();
    mat.cb_materialRoughness            = in_material->GetRoughness();
    mat.cb_materialDiffuse              = in_material->GetDiffuse();
    mat.cb_materialDisplacementStrength = in_material->GetDisplacementStrength();
    mat.cb_materialSpecular             = in_material->GetSpecular();
    mat.cb_materialSharpness            = in_material->GetSharpness();
    mat.cb_materialAmbient              = in_material->GetAmbient();
    mat.cb_materialAlpha                = in_material->GetAlpha();
    mat.cb_materialTextureFlags         = MATERIAL_TEXTURE_FLAGS_NONE;

    if (const auto& tex = in_material->GetAlbedoTexture())
    {
        mat.cb_materialTextureFlags |= MATERIAL_TEXTURE_FLAGS_ALBEDO;
        tex->Bind(TEXTURE_ALBEDO_SLOT, eShaderFlags_PixelShader);
    }

    if (const auto& tex = in_material->GetNormalTexture())
    {
        if (in_material->GetNormalMapType() == eNormalMapType::OpenGL)
            mat.cb_materialTextureFlags |= MATERIAL_TEXTURE_FLAGS_NORMAL_GL;
        else
            mat.cb_materialTextureFlags |= MATERIAL_TEXTURE_FLAGS_NORMAL_DX;

        tex->Bind(TEXTURE_NORMAL_SLOT, eShaderFlags_PixelShader);
    }

    if (const auto& tex = in_material->GetAOTexture())
    {
        mat.cb_materialTextureFlags |= MATERIAL_TEXTURE_FLAGS_AO;
        tex->Bind(TEXTURE_AO_SLOT, eShaderFlags_PixelShader);
    }

    if (const auto& tex = in_material->GetMetallicTexture())
    {
        mat.cb_materialTextureFlags |= MATERIAL_TEXTURE_FLAGS_METALLIC;
        tex->Bind(TEXTURE_METALLIC_SLOT, eShaderFlags_PixelShader);
    }

    if (const auto& tex = in_material->GetRoughnessTexture())
    {
        mat.cb_materialTextureFlags |= MATERIAL_TEXTURE_FLAGS_ROUGHNESS;
        tex->Bind(TEXTURE_ROUGHNESS_SLOT, eShaderFlags_PixelShader);
    }

    if (const auto& tex = in_material->GetEmissiveTexture())
    {
        mat.cb_materialTextureFlags |= MATERIAL_TEXTURE_FLAGS_EMISSIVE;
        tex->Bind(TEXTURE_EMISSION_SLOT, eShaderFlags_PixelShader);
    }

    if (const auto& tex = in_material->GetDisplacementTexture())
    {
        mat.cb_materialTextureFlags |= MATERIAL_TEXTURE_FLAGS_DISPLACEMENT;
        tex->Bind(TEXTURE_DISPLACEMENT_SLOT, eShaderFlags_VertexShader);
    }

    m_materialCB->WriteToBuffer(mat);
}

Color3 PBRGeometryPipeline::GetAlbedo() const
{
    return GetConstantData().cb_materialAlbedo;
}

Color3 PBRGeometryPipeline::GetEmissive() const
{
    return GetConstantData().cb_materialEmissive;
}

float PBRGeometryPipeline::GetMetallic() const
{
    return GetConstantData().cb_materialMetallic;
}

float PBRGeometryPipeline::GetRoughness() const
{
    return GetConstantData().cb_materialRoughness;
}

float PBRGeometryPipeline::GetDisplacementScale() const
{
    return GetConstantData().cb_materialDisplacementStrength;
}

Color3 PBRGeometryPipeline::GetAmbient() const
{
    return GetConstantData().cb_materialAmbient;
}

Color3 PBRGeometryPipeline::GetDiffuse() const
{
    return GetConstantData().cb_materialDiffuse;
}

Color3 PBRGeometryPipeline::GetSpecular() const
{
    return GetConstantData().cb_materialSpecular;
}

float PBRGeometryPipeline::GetSharpness() const
{
    return GetConstantData().cb_materialSharpness;
}

float PBRGeometryPipeline::GetAlpha() const
{
    return GetConstantData().cb_materialAlpha;
}

}   // namespace crab
