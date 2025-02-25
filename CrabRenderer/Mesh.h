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
    D11_RESOURCE_CTOR(Mesh);

public:
    template<typename Vertex>
    static Ref<Mesh> Create(const std::vector<Vertex>& in_vertices,
                            const std::vector<uint32>& in_indices,
                            eTopology                  in_topology);

    static Ref<Mesh> Create(const Ref<VertexBuffer>& in_vb,
                            const Ref<IndexBuffer>&  in_ib,
                            eTopology                in_topology);

    template<typename Vertex>
    static Ref<Mesh> Create(const std::vector<Vertex>& in_vertices, eTopology in_topology);
    static Ref<Mesh> Create(const Ref<VertexBuffer>& in_vb, eTopology in_topology);

    void Draw() const;

private:
    Ref<VertexBuffer> m_vertexBuffer;
    Ref<IndexBuffer>  m_indexBuffer;
    eTopology         m_topology = eTopology::TriangleList;
};

//===================================================
// Inline
//===================================================

template<typename Vertex>
Ref<Mesh> Mesh::Create(const std::vector<Vertex>& in_vertices,
                       const std::vector<uint32>& in_indices,
                       eTopology                  in_topology)
{
    Ref<VertexBuffer> vb = VertexBuffer::Create(in_vertices);
    Ref<IndexBuffer>  ib = IndexBuffer::Create(in_indices);

    return Create(vb, ib, in_topology);
}

template<typename Vertex>
Ref<Mesh> Mesh::Create(const std::vector<Vertex>& in_vertices, eTopology in_topology)
{
    return Create(in_vertices, nullptr, in_topology);
}

}   // namespace crab
