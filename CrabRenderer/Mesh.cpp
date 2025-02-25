#include "CrabPch.h"

#include "Mesh.h"

#include "Buffers.h"
#include "D11Renderer.h"
#include "Model.h"

namespace crab
{

Ref<Mesh> Mesh::Create(const Ref<VertexBuffer>& in_vb,
                       const Ref<IndexBuffer>&  in_ib,
                       eTopology                in_topology)
{
    auto mesh            = CreateRef<Mesh>();
    mesh->m_vertexBuffer = in_vb;
    mesh->m_indexBuffer  = in_ib;
    mesh->m_topology     = in_topology;
    return mesh;
}

Ref<Mesh> Mesh::Create(const Ref<VertexBuffer>& in_vb, eTopology in_topology)
{
    return Create(in_vb, nullptr, in_topology);
}

void Mesh::Draw() const
{
    GetRenderer().SetTopology(m_topology);

    m_vertexBuffer->Bind();

    if (m_indexBuffer)
        m_indexBuffer->Bind();

    if (m_indexBuffer)
        GetRenderer().DrawIndexed(m_indexBuffer->GetIndexCount());
    else
        GetRenderer().Draw(m_vertexBuffer->GetVertexCount());
}

}   // namespace crab