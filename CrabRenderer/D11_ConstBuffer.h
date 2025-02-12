#pragma once
#include "D11_Utils.h"

namespace crab
{

class D11_ConstantBufferBase
{
public:
    virtual              ~D11_ConstantBufferBase() = default;

    void                 Bind(uint32 in_slot, eShaderFlags in_flag);
    ComPtr<ID3D11Buffer> Get() const { return m_buffer; }
    uint32               GetDataSize() const { return m_dataSize; }

    virtual TypeInfo GetTypeInfo() const = 0;

protected:
    ComPtr<ID3D11Buffer> m_buffer;
    uint32               m_dataSize = 0;
};

template<typename Ty>
class D11_ConstantBuffer : public D11_ConstantBufferBase
{
public:
    static Ref<D11_ConstantBuffer<Ty>> Create()
    {
        auto cb        = CreateRef<D11_ConstantBuffer<Ty>>();
        cb->m_buffer   = ID3D11BufferUtil::CreateConstantBuffer(sizeof(Ty));
        cb->m_dataSize = sizeof(Ty);
        return cb;
    }

    void UpdateData(const Ty& in_data)
    {
        ID3D11BufferUtil::WriteToBuffer(
            m_buffer.Get(),
            &in_data,
            sizeof(Ty));
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
