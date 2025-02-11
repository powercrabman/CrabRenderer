#include "CrabPch.h"

#include "D11_ConstBuffer.h"

#include "D11_Renderer.h"

namespace crab
{

ComPtr<ID3D11Buffer> D11_ConstantBufferUtile::Create(uint32 in_dataStride)
{
    auto d = D11_API->GetDevice();

    D3D11_BUFFER_DESC desc = {};
    desc.Usage             = D3D11_USAGE_DYNAMIC;
    desc.ByteWidth         = in_dataStride;
    desc.BindFlags         = D3D11_BIND_CONSTANT_BUFFER;
    desc.CPUAccessFlags    = D3D11_CPU_ACCESS_WRITE;

    ComPtr<ID3D11Buffer> buffer;
    D11_ASSERT(d->CreateBuffer(&desc, nullptr, buffer.GetAddressOf()), "CreateBuffer Fail.");

    return buffer;
}

void D11_ConstantBufferUtile::UpdateData(ID3D11Buffer* in_buffer, const void* in_data, uint32 in_dataSize)
{
    auto                     d              = D11_API->GetContext();
    D3D11_MAPPED_SUBRESOURCE mappedResource = {};
    D11_ASSERT(d->Map(in_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource), "Map Fail.");
    memcpy(mappedResource.pData, in_data, in_dataSize);
    d->Unmap(in_buffer, 0);
}

void D11_ConstantBufferBase::Bind(uint32 in_slot, eShaderFlags in_flag)
{
    D11_API->SetConstantBuffer(m_buffer.Get(), in_slot, in_flag);
}

void D11_ConstantBufferList::ClearList()
{
    m_nodes.clear();
}

void D11_ConstantBufferList::Bind()
{
    for (auto& node: m_nodes)
        node.buffer->Bind(node.slot, node.flag);
}

D11_ConstantBufferList& D11_ConstantBufferList::Add(Ref<D11_ConstantBufferBase> in_buffer, uint32 in_slot, eShaderFlags in_flag)
{
    m_nodes.push_back({ in_buffer, in_slot, in_flag });
    return *this;
}

Ref<D11_ConstantBufferBase> D11_ConstantBufferList::GetBuffer(uint32 in_index) const
{
    return m_nodes[in_index].buffer;
}

}   // namespace crab
