#include "CrabPch.h"

#include "D11_ConstBuffer.h"

#include "D11_Renderer.h"

namespace crab
{

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
