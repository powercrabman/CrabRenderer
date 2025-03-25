#include "InternalPCH.h"

#include "SkyboxPipeline.h"

#include "GlobalConstants.h"
#include "GlobalShader.h"
#include "GlobalState.h"
#include "Mesh.h"
#include "RenderFactory.h"
#include "RenderStates.h"
#include "Renderer.h"
#include "ShaderResource.h"

namespace crab
{

SkyboxPipeline::SkyboxPipeline()
{
}

SkyboxPipeline::~SkyboxPipeline()
{
}

void SkyboxPipeline::Init(
    const Ref<RasterizerState>&   in_rasterizerState,
    const Ref<DepthStencilState>& in_depthStencilState,
    const Ref<BlendState>&        in_blendState,
    const Ref<FrameBuffer>&       in_frameBuffer,
    const Viewport&               in_viewport)
{
    BasicPipeline::InitBasicPipeline_Internal(eTopology::TriangleList,
                                              GlobalShader::GetSkyboxVS(),
                                              GlobalShader::GetSkyboxPS(),
                                              nullptr,
                                              nullptr,
                                              nullptr,
                                              in_rasterizerState,
                                              in_depthStencilState,
                                              in_blendState,
                                              in_frameBuffer,
                                              in_viewport,
                                              eShaderFlags_VertexShader,
                                              eShaderFlags_VertexShader);

    m_wrapSampler = GlobalState::Sampler_Linear_Wrap();
    m_skyboxCB    = factory::CreateConstantBuffer<CB_SKYBOX>();
}

void SkyboxPipeline::BindPipeline(const PipelineBindArgs& in_args)
{
    BasicPipeline::BindPipeline(in_args);

    m_wrapSampler->Bind(SAMPLER_LINEAR_WRAP_SLOT, eShaderFlags_PixelShader);
    m_skyboxCB->Bind(CB_SKYBOX_SLOT, eShaderFlags_VertexShader);
}

void SkyboxPipeline::SetSkyboxType(eSkyboxType in_type) const
{
    m_skyboxCB->WriteToBuffer(
        CB_SKYBOX {
            .cb_skyboxTextureCubeType = static_cast<SKYBOX_TEXTURE_TYPE>(in_type),
        });
}

eSkyboxType SkyboxPipeline::GetSkyboxType() const
{
    return static_cast<eSkyboxType>(m_skyboxCB->GetCPUData().cb_skyboxTextureCubeType);
}

void SkyboxPipeline::SetEnvironmentTextureCube(const Ref<TextureCube>& in_texture) const
{
    in_texture->Bind(TEXTURE_ENVIRONMENT_MAP_SLOT, eShaderFlags_PixelShader);
}

void SkyboxPipeline::SetDiffuseTextureCube(const Ref<TextureCube>& in_texture) const
{
    in_texture->Bind(TEXTURE_DIFFUSE_MAP_SLOT, eShaderFlags_PixelShader);
}

void SkyboxPipeline::SetSpecularTextureCube(const Ref<TextureCube>& in_texture) const
{
    in_texture->Bind(TEXTURE_SPECULAR_MAP_SLOT, eShaderFlags_PixelShader);
}

}   // namespace crab