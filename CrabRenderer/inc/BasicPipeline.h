#pragma once
#include "PipelineBase.h"

namespace crab
{
struct CB_TRANSFORM;

class BasicPipeline : public PipelineBase
{
public:
    ~BasicPipeline() override;

    void SetCamera(const Mat4& in_view,
                   const Mat4& in_proj,
                   const Vec3& in_cameraPosition) const;

    void SetTransform(const Mat4& in_world) const;

    void BindPipeline(const PipelineBindArgs& in_args) override;

protected:
    auto InitBasicPipeline_Internal(eTopology                     in_topology,
                                    const Ref<VertexShader>&      in_vs,
                                    const Ref<PixelShader>&       in_ps,
                                    const Ref<GeometryShader>&    in_gs,
                                    const Ref<HullShader>&        in_hs,
                                    const Ref<DomainShader>&      in_ds,
                                    const Ref<RasterizerState>&   in_rasterizerState,
                                    const Ref<DepthStencilState>& in_depthStencilState,
                                    const Ref<BlendState>&        in_blendState,
                                    const Ref<FrameBuffer>&       in_frameBuffer,
                                    const Viewport&               in_viewport,
                                    eShaderFlags                  in_transformFlags,
                                    eShaderFlags                  in_cameraFlags) -> void;

    enum
    {
        TEXTURE_ENVIRONMENT_MAP_SLOT = 10,
        TEXTURE_DIFFUSE_MAP_SLOT     = 11,
        TEXTURE_SPECULAR_MAP_SLOT    = 12,
        TEXTURE_BRDF_LUT_SLOT        = 13,

        SAMPLER_LINEAR_WRAP_SLOT           = 0,
        SAMPLER_LINEAR_CLAMP_SLOT          = 1,
        SAMPLER_POINT_CLAMP_SLOT           = 2,
        SAMPLER_COMPRISON_LINEAR_WRAP_SLOT = 3,

        TEXTURE_DISPLACEMENT_SLOT = 0,
        TEXTURE_ALBEDO_SLOT       = 0,
        TEXTURE_NORMAL_SLOT       = 1,
        TEXTURE_AO_SLOT           = 2,
        TEXTURE_METALLIC_SLOT     = 3,
        TEXTURE_ROUGHNESS_SLOT    = 4,
        TEXTURE_EMISSION_SLOT     = 5,
    };

private:
    Ref<ConstantBuffer<CB_TRANSFORM>> m_transformCB;
    eShaderFlags                      m_transformCBBindFlags;
    eShaderFlags                      m_cameraCBBindFlags;
};

}   // namespace crab