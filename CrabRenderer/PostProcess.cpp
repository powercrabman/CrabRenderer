#include "CrabPch.h"

#include "PostProcess.h"

#include "GlobalState.h"
#include "D11Renderer.h"
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

void PostProcess::Render()
{
    // post process common state
    GetGlobalState()->DepthStencil_DepthNone()->Bind();
    GetGlobalState()->Rasterizer_CullBack(true)->Bind();
    GetGlobalState()->Blend_Opaque(true)->Bind();

    // lazy create mesh
    if (!m_mesh)
    {
        std::vector<PostProcessVertex> vertices;
        vertices.reserve(4);
        vertices.push_back({ Vec2 { -1.f, 1.f }, Vec2 { 0.f, 0.f } });
        vertices.push_back({ Vec2 { 1.f, 1.f }, Vec2 { 1.f, 0.f } });
        vertices.push_back({ Vec2 { 1.f, -1.f }, Vec2 { 1.f, 1.f } });
        vertices.push_back({ Vec2 { -1.f, -1.f }, Vec2 { 0.f, 1.f } });

        std::vector<uint32> indices = { 0, 1, 2, 0, 2, 3 };

        m_mesh = CreateRef<Mesh>();
        m_mesh->Init(vertices, indices, eTopology::TriangleList);
    }

    for (auto& filter: m_filters)
    {
        filter->Bind();
        m_mesh->Draw();
    }
}

}   // namespace crab