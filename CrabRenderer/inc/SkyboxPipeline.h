#pragma once
#include "BasicPipeline.h"
#include "ShaderSharedData.h"

namespace crab
{
class SamplerState;
class TextureCube;

enum class eSkyboxType
{
    Environment = SKYBOX_TEXTURE_TYPE_ENVIRONMENT,
    Diffuse     = SKYBOX_TEXTURE_TYPE_DIFFUSE,
    Specular    = SKYBOX_TEXTURE_TYPE_SPECULAR,
    Default     = Environment
};

class SkyboxPipeline : public BasicPipeline
{
public:
    SkyboxPipeline();
    ~SkyboxPipeline() override;

    void Init(const Ref<RasterizerState>&   in_rasterizerState,
              const Ref<DepthStencilState>& in_depthStencilState,
              const Ref<BlendState>&        in_blendState,
              const Ref<FrameBuffer>&       in_frameBuffer,
              const Viewport&               in_viewport);

    void BindPipeline(const PipelineBindArgs& in_args) override;

    void        SetSkyboxType(eSkyboxType in_type) const;
    eSkyboxType GetSkyboxType() const;

    void SetEnvironmentTextureCube(const Ref<TextureCube>& in_texture) const;
    void SetDiffuseTextureCube(const Ref<TextureCube>& in_texture) const;
    void SetSpecularTextureCube(const Ref<TextureCube>& in_texture) const;

private:
    Ref<ConstantBuffer<CB_SKYBOX>> m_skyboxCB;
    Ref<SamplerState>              m_wrapSampler;
};

}   // namespace crab