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
    void                 Bind(uint32 in_slot, eShaderFlags in_flag);
    ComPtr<ID3D11Buffer> Get() const { return m_buffer; }
    uint32               GetStride() const { return m_dataStride; }

    virtual TypeInfo GetTypeInfo() const = 0;

protected:
    ComPtr<ID3D11Buffer> m_buffer;
    uint32               m_dataStride;
};

template<typename Ty>
class D11_ConstantBuffer : public D11_ConstantBufferBase
{
public:
    static Ref<D11_ConstantBuffer<Ty>> Create()
    {
        auto cb          = CreateRef<D11_ConstantBuffer<Ty>>();
        cb->m_dataStride = sizeof(Ty);
        cb->m_buffer     = D11_ConstantBufferUtile::Create(cb->m_dataStride);
        return cb;
    }

    void UpdateData(const Ty& in_data)
    {
        D11_ConstantBufferUtile::UpdateData(m_buffer.Get(), &in_data, sizeof(Ty));
    }

    TypeInfo GetTypeInfo() const override
    {
        return crab::GetTypeInfo<Ty>();
    }
};

//===================================================
//               Constant Buffer List
//===================================================

class D11_ConstantBufferList
{
public:
    void ClearList();
    void Bind();

    D11_ConstantBufferList& Add(Ref<D11_ConstantBufferBase> in_buffer, uint32 in_slot, eShaderFlags in_flag);

    Ref<D11_ConstantBufferBase> GetBuffer(uint32 in_index) const;

private:
    struct ConstantBufferNode
    {
        Ref<D11_ConstantBufferBase> buffer;
        uint32                      slot;
        eShaderFlags                flag;
    };
    std::vector<ConstantBufferNode> m_nodes;
};

}   // namespace crab
