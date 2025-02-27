#include "CrabPch.h"

#include "PostProcess.h"

#include "D11Renderer.h"
#include "Mesh.h"
#include "Textures.h"
#include "ImageFilter.h"
#include "RenderStates.h"
#include "CommonState.h"

namespace crab
{

void PostProcess::AddFilter(const Ref<ImageFilter>&   in_filter)
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
    GetCommonState()->DepthStencil_DepthNone()->Bind();
    GetCommonState()->Rasterizer_CullCounterClockwise(true)->Bind();
    GetCommonState()->Blend_Opaque(true)->Bind();

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

        m_mesh = Mesh::Create(vertices, indices, eTopology::TriangleList);
    }

    for (auto& filter: m_filters)
    {
        filter->Bind();
        m_mesh->Draw();
    }
}

}   // namespace crab