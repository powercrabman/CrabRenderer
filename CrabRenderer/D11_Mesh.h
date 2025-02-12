#pragma once

#include "VertexType.h"
#include "D11_Buffers.h"

namespace crab
{

class D11_VertexBuffer;
class D11_IndexBuffer;

//===================================================
// Mesh
//===================================================

class D11_Mesh
{
public:
    template<typename Vertex>
    static Ref<D11_Mesh> Create(const std::vector<Vertex>& in_vertices,
                                const std::vector<uint32>& in_indices);

    static Ref<D11_Mesh> Create(const Ref<D11_VertexBuffer>& in_vb,
                                const Ref<D11_IndexBuffer>&  in_ib);

    void Draw() const;

private:
    Ref<D11_VertexBuffer> m_vertexBuffer;
    Ref<D11_IndexBuffer>  m_indexBuffer;
};

//===================================================
// Inline
//===================================================

template<typename Vertex>
Ref<D11_Mesh> D11_Mesh::Create(const std::vector<Vertex>& in_vertices, const std::vector<uint32>& in_indices)
{
    Ref<D11_VertexBuffer> vb = D11_VertexBuffer::Create(in_vertices);
    Ref<D11_IndexBuffer>  ib = D11_IndexBuffer::Create(in_indices);

    return Create(vb, ib);
}

}   // namespace crab
