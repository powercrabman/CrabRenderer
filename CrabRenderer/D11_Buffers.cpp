#include "CrabPch.h"

#include "D11_Buffers.h"

#include "D11_Renderer.h"

namespace crab
{

//===================================================
// Index Buffer
//===================================================

Ref<D11_IndexBuffer> D11_IndexBuffer::Create(const std::vector<uint32>& in_indices)
{
    Ref<D11_IndexBuffer> ib = CreateRef<D11_IndexBuffer>();

    ib->m_buffer = ID3D11BufferUtil::CreateIndexBuffer(
        static_cast<uint32>(in_indices.size()),
        in_indices.data()
    );

    ib->m_indexCount = static_cast<uint32>(in_indices.size());
    return ib;
}

void D11_IndexBuffer::Bind() const
{
    D11_API->SetIndexBuffer(m_buffer.Get());
}

crab::uint32 D11_VertexBuffer::GetVertexCount() const
{
    return m_vertexCount;
}

void D11_VertexBuffer::Bind() const
{
    D11_API->SetVertexBuffer(m_buffer.Get(), m_vertexStride, m_offset);
}

}   // namespace crab