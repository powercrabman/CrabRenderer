#pragma once
#include "D11_Utiles.h"

namespace crab
{
struct D11_ConstantBufferBase
{
    ComPtr<ID3D11Buffer> buffer;
    uint32               dataSize;
};

struct D11_ConstantBufferUtile
{
    static ComPtr<ID3D11Buffer> Create(uint32 in_dataSize);
    static void                 UpdateData(ID3D11Buffer* in_buffer, const void* in_data, uint32 in_dataSize);
};

template<typename Ty>
struct D11_ConstantBuffer : public D11_ConstantBufferBase
{
    static Ref<D11_ConstantBuffer<Ty>> Create()
    {
        auto cb      = CreateRef<D11_ConstantBuffer<Ty>>();
        cb->dataSize = sizeof(Ty);
        cb->buffer   = D11_ConstantBufferUtile::Create(cb->dataSize);
        return cb;
    }

    void UpdateData(const Ty& in_data)
    {
        D11_ConstantBufferUtile::UpdateData(buffer.Get(), &in_data, sizeof(Ty));
    }
};

}   // namespace crab
