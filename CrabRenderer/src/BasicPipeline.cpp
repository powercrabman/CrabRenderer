#include "InternalPCH.h"

#include "BasicPipeline.h"

#include "GlobalConstants.h"
#include "RenderFactory.h"

namespace crab
{

BasicPipeline::~BasicPipeline()
{
}

void BasicPipeline::SetCamera(const Mat4& in_view,
                              const Mat4& in_proj,
                              const Vec3& in_cameraPosition) const
{
    Mat4 viewProj = in_view * in_proj;
    GlobalConstants::UpdateCamera(in_view,
                                  in_proj,
                                  viewProj,
                                  in_cameraPosition);
}

void BasicPipeline::SetTransform(const Mat4& in_world) const
{
    m_transformCB->WriteToBuffer(CB_TRANSFORM {
        .cb_transformWorld             = in_world,
        .cb_transformWorldInvTranspose = in_world.Invert().Transpose() });
}

void BasicPipeline::BindPipeline(const PipelineBindArgs& in_args)
{
    PipelineBase::BindPipeline(in_args);

    m_transformCB->Bind(CB_TRANSFORM_SLOT, m_transformCBBindFlags);
    GlobalConstants::GetCameraConstant()->Bind(CB_CAMERA_SLOT, m_cameraCBBindFlags);
}

void BasicPipeline::InitBasicPipeline_Internal(eTopology                     in_topology,
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
                                               eShaderFlags                  in_cameraFlags)
{
    PipelineBase::InitPipelineBase_Internal(
        in_topology,
        in_vs,
        in_ps,
        in_gs,
        in_hs,
        in_ds,
        in_rasterizerState,
        in_depthStencilState,
        in_blendState,
        in_frameBuffer,
        in_viewport);

    m_transformCB = factory::CreateConstantBuffer<CB_TRANSFORM>();

    m_transformCBBindFlags = in_transformFlags;
    m_cameraCBBindFlags    = in_cameraFlags;
}

}   // namespace crab