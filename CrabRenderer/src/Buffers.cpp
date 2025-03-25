#include "InternalPCH.h"

#include "Buffers.h"

#include "Renderer.h"
#include "ShaderResource.h"

namespace crab
{

//===================================================
// Index Buffer
//===================================================

void IndexBuffer::Init(const std::vector<Index>& in_indices)
{
    uint32 byteWidth = static_cast<uint32>(in_indices.size() * sizeof(Index));

    m_indexCount = static_cast<uint32>(in_indices.size());
    m_id         = s_idGenerator.GenerateID();
    m_buffer     = d3d11::buffer::CreateBuffer(byteWidth,
                                           D3D11_USAGE_IMMUTABLE,
                                           eBindFlags_IndexBuffer,
                                           eCPUAccessFlags_None,
                                           in_indices.data());
}

void IndexBuffer::Bind() const
{
    GetRenderer().SetIndexBuffer(m_buffer.Get(), m_id);
}

void StagingBuffer::Init(
    uint32 in_bufferByteWidth,
    bool   in_cpuWrite,
    bool   in_cpuRead)
{
    eCPUAccessFlags cpuAccessFlags = eCPUAccessFlags_None;

    if (in_cpuWrite)
        cpuAccessFlags |= eCPUAccessFlags_Write;

    if (in_cpuRead)
        cpuAccessFlags |= eCPUAccessFlags_Read;

    m_bufferByteWidth = in_bufferByteWidth;
    m_buffer          = d3d11::buffer::CreateBuffer(m_bufferByteWidth,
                                           D3D11_USAGE_STAGING,
                                           eBindFlags_None,
                                           cpuAccessFlags,
                                           nullptr);
}

void StagingBuffer::WriteToBuffer(
    const void* in_writeData,
    uint32      in_dataByteWidth) const
{
    d3d11::buffer::WriteToStagingBuffer(m_buffer.Get(), in_writeData, in_dataByteWidth);
}

void StagingBuffer::ReadFromBuffer(void* out_data, uint32* out_dataByteWidth) const
{
    *out_dataByteWidth = std::min(*out_dataByteWidth, m_bufferByteWidth);
    d3d11::buffer::ReadFromBuffer(m_buffer.Get(), out_data, out_dataByteWidth);
}

void StagingBuffer::CopyTo(ID3D11Buffer* in_destBuffer) const
{
    d3d11::buffer::CopyBetween(m_buffer.Get(), in_destBuffer);
}

void StagingBuffer::CopyFrom(ID3D11Buffer* in_srcBuffer) const
{
    d3d11::buffer::CopyBetween(in_srcBuffer, m_buffer.Get());
}

uint32 StagingBuffer::GetBufferByteWidth() const
{
    return m_bufferByteWidth;
}

void VertexBuffer::Bind() const
{
    GetRenderer().SetVertexBuffer(m_buffer.Get(), m_vertexStride, 0, m_id);
}

void StructuredBufferBase::BindUAV(uint32 in_slot) const
{
    m_uav->BindUAV(in_slot);
}

void StructuredBufferBase::BindImage(uint32 in_slot, eShaderFlags in_bindFlags) const
{
    m_uav->BindSRV(in_slot, in_bindFlags);
}

void ConstantBufferBase::Bind(uint32 in_slot, eShaderFlags in_bindFlags) const
{
    GetRenderer().SetConstantBuffer(m_buffer.Get(), in_slot, in_bindFlags);
}

void UnorderedAccessView::Init(ID3D11Buffer* in_buffer)
{
    m_uav = d3d11::views::CreateUAV(in_buffer);

    m_shaderResource = CreateRef<BufferShaderResource>();
    m_shaderResource->Init(in_buffer);
}

void UnorderedAccessView::BindUAV(uint32 in_slot) const
{
    // todo: need conflict handle with Texture2D
    GetRenderer().SetUnorderedAccessView(m_uav.Get(), in_slot);
}

void UnorderedAccessView::BindSRV(uint32 in_slot, eShaderFlags in_bindFlags) const
{
    m_shaderResource->Bind(in_slot, in_bindFlags);
}

}   // namespace crab