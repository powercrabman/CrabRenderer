#include "InternalPCH.h"


#include "PostProcess.h"

#include "Draw2D.h"
#include "GlobalState.h"
#include "ImageFilterBase.h"
#include "RenderStates.h"

namespace crab
{

void PostProcess::AddFilter(const Ref<ImageFilterBase>& in_filter)
{
    m_filters.push_back(in_filter);
}

Ref<ImageFilterBase> PostProcess::GetLastFilter() const
{
    return m_filters.back();
}

void PostProcess::ClearFilterList()
{
    m_filters.clear();
}

void PostProcess::Render() const
{
    // post process common state
    GlobalState::DepthStencil_NoDepth_NoStencil()->Bind(0);
    GlobalState::Rasterizer_Solid_CullFront(true)->Bind();
    GlobalState::Blend_NoBlend(true)->Bind({ 0.f, 0.f, 0.f, 0.f });

    for (auto& filter: m_filters)
    {
        filter->Bind();
        Draw2D::DrawFullScreenMesh();
    }
}

}   // namespace crab