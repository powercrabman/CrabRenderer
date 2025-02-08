#pragma once
#include "D11_Utiles.h"

namespace crab
{
struct D11_ConstantBufferUtile
{
    static ComPtr<ID3D11Buffer> Create(uint32 in_dataStride);
    static void                 UpdateData(ID3D11Buffer* in_buffer, const void* in_data, uint32 in_dataSize);
};

class D11_ConstantBufferBase
{
public:
    void Bind();

protected:
    ComPtr<ID3D11Buffer> m_buffer;
    uint32               m_dataStride;
    eShaderFlags         m_bindFlags;
    uint32               m_bindSlot;
};

template<typename Ty>
class D11_ConstantBuffer : public D11_ConstantBufferBase
{
public:
    static Ref<D11_ConstantBuffer<Ty>> Create(uint32 in_bindSlot, eShaderFlags in_bindFlags)
    {
        auto cb          = CreateRef<D11_ConstantBuffer<Ty>>();
        cb->m_dataStride = sizeof(Ty);
        cb->m_buffer     = D11_ConstantBufferUtile::Create(cb->m_dataStride);
        cb->m_bindFlags  = in_bindFlags;
        cb->m_bindSlot   = in_bindSlot;
        return cb;
    }

    void UpdateData(const Ty& in_data)
    {
        D11_ConstantBufferUtile::UpdateData(m_buffer.Get(), &in_data, sizeof(Ty));
    }
};

}   // namespace crab
