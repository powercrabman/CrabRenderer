#pragma once

namespace crab
{

//===================================================
// Vertex Buffer
//===================================================

struct D11_VertexBufferUtile
{
    static ComPtr<ID3D11Buffer> Create(const void* in_data,
                                       uint32      in_vertexCount,
                                       uint32      in_vertexStride);
};

class D11_VertexBuffer : public std::enable_shared_from_this<D11_VertexBuffer>
{
public:
    template<typename VertexType>
    static Ref<D11_VertexBuffer> Create(const std::vector<VertexType>& in_vertices)
    {
        auto vb = CreateRef<D11_VertexBuffer>();

        vb->m_vertexCount  = (uint32)in_vertices.size();
        vb->m_vertexStride = sizeof(VertexType);
        vb->m_offset       = 0;
        vb->m_buffer       = D11_VertexBufferUtile::Create(in_vertices.data(), vb->m_vertexCount, vb->m_vertexStride);
        return vb;
    }

    void Bind() const;

private:
    ComPtr<ID3D11Buffer> m_buffer;
    uint32               m_vertexCount;
    uint32               m_vertexStride;
    uint32               m_offset;
};

//===================================================
// Index Buffer
//===================================================

class D11_IndexBuffer
{
public:
    static Ref<D11_IndexBuffer> Create(const std::vector<uint32>& in_indices);

    void Bind() const;
    uint32 GetIndexCount() const { return m_indexCount; }

private:
    ComPtr<ID3D11Buffer> m_buffer;
    uint32               m_indexCount;
};

}   // namespace crab
