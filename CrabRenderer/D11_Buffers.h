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

struct D11_VertexBuffer : public std::enable_shared_from_this<D11_VertexBuffer>
{
    template<typename VertexType>
    static Ref<D11_VertexBuffer> Create(const std::vector<VertexType>& in_vertices)
    {
        auto vb = CreateRef<D11_VertexBuffer>();

        vb->vertexCount  = (uint32)in_vertices.size();
        vb->vertexStride = sizeof(VertexType);
        vb->offset       = 0;
        vb->buffer       = D11_VertexBufferUtile::Create(in_vertices.data(), vb->vertexCount, vb->vertexStride);
        return vb;
    }

    ComPtr<ID3D11Buffer> buffer;
    uint32               vertexCount;
    uint32               vertexStride;
    uint32               offset;

};

//===================================================
// Index Buffer
//===================================================

struct D11_IndexBuffer
{
    static Ref<D11_IndexBuffer> Create(const std::vector<uint32>& in_indices);

    ComPtr<ID3D11Buffer> buffer;
    uint32               indexCount;
};

}   // namespace crab
