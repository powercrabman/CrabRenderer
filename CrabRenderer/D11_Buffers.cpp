#include "CrabPch.h"

#include "D11_Buffers.h"

#include "D11_Renderer.h"

#include "D11_Utiles.h"

namespace crab
{

//===================================================
// Vertex Buffer
//===================================================

ComPtr<ID3D11Buffer> D11_VertexBufferUtile::Create(const void* in_data, uint32 in_vertexCount, uint32 in_vertexStride)
{
    auto d = D11_API->GetDevice();

    D3D11_BUFFER_DESC desc = {};
    desc.Usage             = D3D11_USAGE_DEFAULT;
    desc.ByteWidth         = in_vertexStride * in_vertexCount;
    desc.BindFlags         = D3D11_BIND_VERTEX_BUFFER;
    desc.CPUAccessFlags    = 0;

    D3D11_SUBRESOURCE_DATA data = {};
    data.pSysMem                = in_data;

    ComPtr<ID3D11Buffer> buffer;
    D11_ASSERT(d->CreateBuffer(&desc, &data, buffer.GetAddressOf()), "CreateBuffer Fail.");

    return buffer;
}

//===================================================
// Index Buffer
//===================================================

Ref<D11_IndexBuffer> D11_IndexBuffer::Create(const std::vector<uint32>& in_indices)
{
    Ref<D11_IndexBuffer> ib = CreateRef<D11_IndexBuffer>();
    auto                 d  = D11_API->GetDevice();

    D3D11_BUFFER_DESC desc = {};
    desc.Usage             = D3D11_USAGE_DEFAULT;
    desc.ByteWidth         = sizeof(uint32) * (uint32)in_indices.size();
    desc.BindFlags         = D3D11_BIND_INDEX_BUFFER;
    desc.CPUAccessFlags    = 0;

    D3D11_SUBRESOURCE_DATA data = {};
    data.pSysMem                = in_indices.data();

    D11_ASSERT(d->CreateBuffer(&desc, &data, ib->m_buffer.GetAddressOf()), "CreateBuffer Fail.");
    ib->m_indexCount = (uint32)in_indices.size();

    return ib;
}

void D11_IndexBuffer::Bind() const
{
    D11_API->SetIndexBuffer(m_buffer.Get());
}

void D11_VertexBuffer::Bind() const
{
    D11_API->SetVertexBuffer(m_buffer.Get(), m_vertexStride, m_offset);
}

}   // namespace crab