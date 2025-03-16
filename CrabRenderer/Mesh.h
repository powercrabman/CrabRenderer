#pragma once

#include "Buffers.h"

namespace crab
{

class VertexBuffer;
class IndexBuffer;

//===================================================
// Mesh
//===================================================

class Mesh
{
public:
    template<typename Vertex>
    void Init(const std::vector<Vertex>& in_vertices,
              const std::vector<uint32>& in_indices,
              eTopology                  in_topology);

    void Init(const Ref<VertexBuffer>& in_vb,
              const Ref<IndexBuffer>&  in_ib,
              eTopology                in_topology);

    eTopology GetTopology() const { return m_topology; }
    uint32    GetIndexCount() const { return m_indexBuffer->GetIndexCount(); }
    uint32    GetIndexStride() const { return sizeof(UINT); }
    uint32    GetVertexCount() const { return m_vertexBuffer->GetVertexCount(); }
    uint32    GetVertexStride() const { return m_vertexBuffer->GetVertexStride(); }

    void Draw(eTopology in_topology) const;   // if you want to use another topology, use this
    void Draw() const { Draw(m_topology); }

private:
    Ref<VertexBuffer> m_vertexBuffer;
    Ref<IndexBuffer>  m_indexBuffer;
    eTopology         m_topology = eTopology::TriangleList;
};

//===================================================
// Inline
//===================================================

template<typename Vertex>
void Mesh::Init(const std::vector<Vertex>& in_vertices,
                const std::vector<uint32>& in_indices,
                eTopology                  in_topology)
{
    Ref<VertexBuffer> vb = CreateRef<VertexBuffer>();
    vb->Init(in_vertices);

    Ref<IndexBuffer> ib = CreateRef<IndexBuffer>();
    ib->Init(in_indices);

    return Init(vb, ib, in_topology);
}

}   // namespace crab
