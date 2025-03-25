#pragma once
#include "BasicPipeline.h"

namespace crab
{
class TextureCube;
class Material;
class SamplerState;

class PBRGeometryPipeline final : public BasicPipeline
{
public:
    PBRGeometryPipeline();
    ~PBRGeometryPipeline() override;

    void Init(const Ref<RasterizerState>&   in_rasterizerState,
              const Ref<DepthStencilState>& in_depthStencilState,
              const Ref<BlendState>&        in_blendState,
              const Ref<RenderTarget>&      in_positionRenderTarget,
              const Ref<RenderTarget>&      in_normalRenderTarget,
              const Ref<RenderTarget>&      in_emissiveRenderTarget,
              const Ref<RenderTarget>&      in_albedoRoughnessRenderTarget,
              const Ref<RenderTarget>&      in_metallicAORenderTarget,
              const Ref<DepthBuffer>&       in_depthBuffer,
              const Viewport&               in_viewport);

    void BindPipeline(const PipelineBindArgs& in_args) override;

    void SetDiffuseTextureCube(const Ref<TextureCube>& in_texture) const;
    void SetSpecularTextureCube(const Ref<TextureCube>& in_texture) const;
    void SetBRDFLUTTexture(const Ref<Texture2D>& in_texture) const;

    void SetMaterial(const Ref<Material>& in_material) const;

    Color3 GetAlbedo() const;
    Color3 GetEmissive() const;
    float  GetMetallic() const;
    float  GetRoughness() const;
    float  GetDisplacementScale() const;
    Color3 GetAmbient() const;
    Color3 GetDiffuse() const;
    Color3 GetSpecular() const;
    float  GetSharpness() const;
    float  GetAlpha() const;

    CB_MATERIAL GetConstantData() const
    {
        return m_materialCB->GetCPUData();
    }

private:
    Ref<SamplerState>                m_wrapSampler;
    Ref<ConstantBuffer<CB_MATERIAL>> m_materialCB;
};

}   // namespace crab