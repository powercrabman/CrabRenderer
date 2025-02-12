#pragma once

#include "D11_Utils.h"

namespace crab
{

//===================================================
// Vertex Buffer
//===================================================

class D11_VertexBuffer
{
public:
    template<typename VertexType>
    static Ref<D11_VertexBuffer> Create(const std::vector<VertexType>& in_vertices)
    {
        auto vb = CreateRef<D11_VertexBuffer>();

        vb->m_buffer = ID3D11BufferUtil::CreateVertexBuffer(
            sizeof(VertexType),
            static_cast<uint32>(in_vertices.size()),
            in_vertices.data());

        vb->m_vertexCount  = static_cast<uint32>(in_vertices.size());
        vb->m_vertexStride = sizeof(VertexType);
        vb->m_offset       = 0;

        return vb;
    }

    uint32 GetVertexCount() const;
    void   Bind() const;

private:
    ComPtr<ID3D11Buffer> m_buffer;
    uint32               m_vertexCount  = 0;
    uint32               m_vertexStride = 0;
    uint32               m_offset       = 0;
};

//===================================================
// Index Buffer
//===================================================

class D11_IndexBuffer
{
public:
    static Ref<D11_IndexBuffer> Create(const std::vector<uint32>& in_indices);

    void   Bind() const;
    uint32 GetIndexCount() const { return m_indexCount; }

private:
    ComPtr<ID3D11Buffer> m_buffer;
    uint32               m_indexCount = 0;
};

}   // namespace crab
