#include "InternalPCH.h"

#include "Mesh.h"

#include "Buffers.h"
#include "Renderer.h"

namespace crab
{

void Mesh::Init(const Ref<VertexBuffer>& in_vb,
                const Ref<IndexBuffer>&  in_ib,
                eTopology                in_topology)
{
    m_vertexBuffer = in_vb;
    m_indexBuffer  = in_ib;
}

uint32 Mesh::GetIndexCount() const
{
    return m_indexBuffer->GetIndexCount();
}

uint32 Mesh::GetVertexCount() const
{
    return m_vertexBuffer->GetVertexCount();
}

uint32 Mesh::GetVertexStride() const
{
    return m_vertexBuffer->GetVertexStride();
}

uint32 Mesh::GetIndexStride() const
{
    return m_indexBuffer->GetIndexStride();
}

Ref<VertexBuffer> Mesh::GetVertexBuffer() const
{
    return m_vertexBuffer;
}

Ref<IndexBuffer> Mesh::GetIndexBuffer() const
{
    return m_indexBuffer;
}

void Mesh::Draw() const
{
    m_vertexBuffer->Bind();
    GetRenderer().Draw(m_vertexBuffer->GetVertexCount());
}

void Mesh::DrawIndices() const
{
    m_vertexBuffer->Bind();
    m_indexBuffer->Bind();
    GetRenderer().DrawIndexed(m_indexBuffer->GetIndexCount());
}

}   // namespace crab