#pragma once
#include "BasicPipeline.h"

namespace crab
{
struct CB_DEBUG_NORMAL;

class DebugNormalPipeline : public BasicPipeline
{
public:
    ~DebugNormalPipeline() override;

    void Init(
        const Ref<FrameBuffer>& in_frameBuffer,
        const Viewport&         in_viewport);

    void BindPipeline(const PipelineBindArgs& in_args) override;

    void  SetDebugNormalLineStrength(float in_strength) const;
    float GetDebugNormalLineStrength() const;

private:
    Ref<ConstantBuffer<CB_DEBUG_NORMAL>> m_normal;
};

}   // namespace crab