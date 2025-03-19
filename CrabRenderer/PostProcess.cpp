#include "CrabPch.h"

#include "PostProcess.h"

#include "GlobalState.h"
#include "D11Renderer.h"
#include "Draw2D.h"
#include "ImageFilter.h"
#include "Mesh.h"
#include "RenderStates.h"
#include "Textures.h"

namespace crab
{

void PostProcess::AddFilter(const Ref<ImageFilter>& in_filter)
{
    m_filters.push_back(in_filter);
}

Ref<ImageFilter> PostProcess::GetLastFilter() const
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
    GetGlobalState()->DepthStencil_DepthNone()->Bind();
    GetGlobalState()->Rasterizer_CullBack(true)->Bind();
    GetGlobalState()->Blend_Opaque(true)->Bind();

    for (auto& filter: m_filters)
    {
        filter->Bind();
        Draw2D::DrawFullScreenMesh();
    }
}

}   // namespace crab