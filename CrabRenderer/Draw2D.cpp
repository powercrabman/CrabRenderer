#include "CrabPch.h"

#include "Draw2D.h"

#include "AppWindow.h"
#include "GeometryFactory.h"
#include "Mesh.h"
#include "RenderFactory.h"

namespace crab
{

struct Draw2D::Impl
{
    Ref<Mesh> m_fullScreenMesh           = nullptr;
    Int2      m_fullScreenMeshResolution = { 0, 0 };
};

Scope<Draw2D::Impl> Draw2D::m_impl = CreateScope<Impl>();

void Draw2D::Init()
{
}

void Draw2D::DrawFullScreenMesh()
{
    Int2 resolution = GetAppWindow().GetResolution();
    if (m_impl->m_fullScreenMeshResolution != resolution || !m_impl->m_fullScreenMesh)
    {
        m_impl->m_fullScreenMeshResolution = resolution;

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