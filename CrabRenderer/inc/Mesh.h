#pragma once

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

    uint32 GetIndexCount() const;
    uint32 GetVertexCount() const;
    uint32 GetVertexStride() const;
    uint32 GetIndexStride() const;

    Ref<VertexBuffer> GetVertexBuffer() const;
    Ref<IndexBuffer>  GetIndexBuffer() const;

    void Draw() const;
    void DrawIndices() const;

private:
    Ref<VertexBuffer> m_vertexBuffer;
    Ref<IndexBuffer>  m_indexBuffer;
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
