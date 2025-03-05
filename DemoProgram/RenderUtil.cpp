#include "pch.h"

#include "RenderUtil.h"

struct RenderUtil::Impl
{
    struct FullScreenVertex
    {
        Vec2 position;
        Vec2 uv;

        inline static const InputElements s_elements = []()
        {
            InputElements elems;
            elems.Add("POSITION", 0, eFormat::Float32x2);
            elems.Add("TEXCOORD", 0, eFormat::Float32x2);
            return elems;
        }();
    };

    Ref<Mesh> fullScreenMesh = nullptr;
};

Scope<RenderUtil::Impl> RenderUtil::m_impl = CreateScope<RenderUtil::Impl>();

void RenderUtil::DrawFullScreenQuad()
{
    if (!m_impl->fullScreenMesh)
    {
        using Vertex = RenderUtil::Impl::FullScreenVertex;

        std::vector<Vertex> vertices = {
            { Vec2(-1.f, -1.f), Vec2(0.f, 1.f) },
            { Vec2(-1.f, 1.f), Vec2(0.f, 0.f) },
            { Vec2(1.f, -1.f), Vec2(1.f, 1.f) },
            { Vec2(1.f, 1.f), Vec2(1.f, 0.f) }
        };

        std::vector<uint32> indices = { 0, 1, 2, 2, 1, 3 };

        m_impl->fullScreenMesh = Mesh::Create(
            VertexBuffer::Create(vertices),
            IndexBuffer::Create(indices),
            eTopology::TriangleList);
    }

    m_impl->fullScreenMesh->Draw();
}
