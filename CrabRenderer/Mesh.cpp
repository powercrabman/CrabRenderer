#include "CrabPch.h"

#include "Mesh.h"

#include "Buffers.h"
#include "D11Renderer.h"
#include "Model.h"

namespace crab
{

void Mesh::Init(const Ref<VertexBuffer>& in_vb,
                const Ref<IndexBuffer>&  in_ib,
                eTopology                in_topology)
{
    m_vertexBuffer = in_vb;
    m_indexBuffer  = in_ib;
    m_topology     = in_topology;
}

void Mesh::Draw() const
{
    GetRenderer().SetTopology(m_topology);

    m_vertexBuffer->Bind();

    if (m_indexBuffer)
        m_indexBuffer->Bind();

    switch (m_topology)
    {
        case eTopology::LineList:
        case eTopology::LineStrip:
        case eTopology::TriangleList:
        case eTopology::TriangleStrip:
            GetRenderer().DrawIndexed(m_indexBuffer->GetIndexCount());
            break;

        case eTopology::PointList:
        case eTopology::PatchList_3:
        case eTopology::PatchList_4:
        case eTopology::PatchList_5:
        case eTopology::PatchList_6:
            GetRenderer().Draw(m_vertexBuffer->GetVertexCount());
            break;

        default:
            CRAB_DEBUG_BREAK("Unknown topology");
            break;
    }
}

}   // namespace crab