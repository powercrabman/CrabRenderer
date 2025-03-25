#include "InternalPCH.h"

#include "DebugNormalPipeline.h"

#include "GlobalConstants.h"
#include "GlobalShader.h"
#include "GlobalState.h"
#include "RenderFactory.h"
#include "Renderer.h"

namespace crab
{

DebugNormalPipeline::~DebugNormalPipeline()
{
}

void DebugNormalPipeline::Init(
    const Ref<FrameBuffer>& in_frameBuffer,
    const Viewport&         in_viewport)
{

    bool msaa = GetRenderer().IsEnableMSAA();

    BasicPipeline::InitBasicPipeline_Internal(
        eTopology::TriangleList,
        GlobalShader::GetDebugNormalVS(),
        GlobalShader::GetDebugNormalPS(),
        nullptr,
        nullptr,
        nullptr,
        GlobalState::Rasterizer_Solid_CullNone(msaa),
        GlobalState::DepthStencil_Depth_NoStencil(),
        GlobalState::Blend_NoBlend(msaa),
        in_frameBuffer,
        in_viewport,
        eShaderFlags_VertexShader,
        eShaderFlags_VertexShader | eShaderFlags_GeometryShader);

    m_normal = factory::CreateConstantBuffer<CB_DEBUG_NORMAL>();
}

void DebugNormalPipeline::BindPipeline(const PipelineBindArgs& in_args)
{
    BasicPipeline::BindPipeline(in_args);

    m_normal->Bind(CB_DEBUG_NORMAL_SLOT, eShaderFlags_GeometryShader);
}

void DebugNormalPipeline::SetDebugNormalLineStrength(float in_strength) const
{
    m_normal->WriteToBuffer(CB_DEBUG_NORMAL {
        .cb_debugNormalLineStrength = in_strength });
}

float DebugNormalPipeline::GetDebugNormalLineStrength() const
{
    return m_normal->GetCPUData().cb_debugNormalLineStrength;
}

}   // namespace crab