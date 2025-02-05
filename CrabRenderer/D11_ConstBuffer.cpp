#include "CrabPch.h"

#include "D11_ConstBuffer.h"

#include "D11_Renderer.h"

namespace crab
{

crab::ComPtr<ID3D11Buffer> D11_ConstantBufferUtile::Create(uint32 in_dataSize)
{
    auto d = D11_API->GetDevice();

    D3D11_BUFFER_DESC desc = {};
    desc.Usage             = D3D11_USAGE_DYNAMIC;
    desc.ByteWidth         = in_dataSize;
    desc.BindFlags         = D3D11_BIND_CONSTANT_BUFFER;
    desc.CPUAccessFlags    = D3D11_CPU_ACCESS_WRITE;

    ComPtr<ID3D11Buffer> buffer;
    D11_ASSERT(d->CreateBuffer(&desc, nullptr, buffer.GetAddressOf()), "CreateBuffer Fail.");

    return buffer;
}

void D11_ConstantBufferUtile::UpdateData(ID3D11Buffer* in_buffer, const void* in_data, uint32 in_dataSize)
{
    auto                     d = D11_API->GetContext();
    D3D11_MAPPED_SUBRESOURCE mappedResource = {};
    D11_ASSERT(d->Map(in_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource), "Map Fail.");
    memcpy(mappedResource.pData, in_data, in_dataSize);
    d->Unmap(in_buffer, 0);
}


}   // namespace crab
