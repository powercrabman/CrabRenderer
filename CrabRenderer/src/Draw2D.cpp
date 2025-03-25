#include "InternalPCH.h"

#include "Draw2D.h"

#include "Mesh.h"

namespace crab
{

struct Draw2D::Impl
{
    Ref<Mesh> m_fullScreenMesh = nullptr;
};

Scope<Draw2D::Impl> Draw2D::m_impl = CreateScope<Impl>();

void Draw2D::Init()
{
}

void Draw2D::DrawFullScreenMesh()
{
    if (m_impl->m_fullScreenMesh)
    {
        std::vector<Vertex2D> vertices = {
            { Vec2(-1.f, -1.f), Vec2(0.f, 1.f) },
            { Vec2(-1.f, 1.f), Vec2(0.f, 0.f) },
            { Vec2(1.f, 1.f), Vec2(1.f, 0.f) },
            { Vec2(1.f, -1.f), Vec2(1.f, 1.f) },
        };

        std::vector<uint32> indices = { 0, 1, 2, 0, 2, 3 };

        m_impl->m_fullScreenMesh = CreateRef<Mesh>();
        m_impl->m_fullScreenMesh->Init(vertices, indices, eTopology::TriangleList);
    }

    m_impl->m_fullScreenMesh->Draw();
}

}   // namespace crab