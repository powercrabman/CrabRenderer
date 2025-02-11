#include "CrabPch.h"

#include "PostProcess.h"

#include "D11_Mesh.h"
#include "D11_Texture.h"
#include "ImageFilter.h"

namespace crab
{

Ref<PostProcess> PostProcess::Create()
{
    auto pp = CreateRef<PostProcess>();

    // Mesh
    std::vector<PostProcessVertex> vertices;
    vertices.push_back({ Vec2 { -1.f, 1.f }, Vec2 { 0.f, 0.f } });
    vertices.push_back({ Vec2 { 1.f, 1.f }, Vec2 { 1.f, 0.f } });
    vertices.push_back({ Vec2 { 1.f, -1.f }, Vec2 { 1.f, 1.f } });
    vertices.push_back({ Vec2 { -1.f, -1.f }, Vec2 { 0.f, 1.f } });

    std::vector<uint32> indices = { 0, 1, 2, 0, 2, 3 };

    pp->m_mesh = D11_Mesh::Create(vertices, indices);

    return pp;
}

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
    for (auto& filter: m_filters)
    {
        filter->Bind();
        m_mesh->Draw();
    }
}

}   // namespace crab