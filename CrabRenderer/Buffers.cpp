#include "CrabPch.h"

#include "Buffers.h"

#include "D11Renderer.h"
#include "Textures.h"

namespace crab
{

//===================================================
// Index Buffer
//===================================================

Ref<IndexBuffer> IndexBuffer::Create(const std::vector<uint32>& in_indices)
{
    Ref<IndexBuffer> ib = CreateRef<IndexBuffer>();

    ib->m_buffer = ID3D11BufferUtil::CreateIndexBuffer(
        static_cast<uint32>(in_indices.size()),
        in_indices.data());

    ib->m_indexCount = static_cast<uint32>(in_indices.size());
    return ib;
}

void IndexBuffer::Bind() const
{
    GetRenderer().SetIndexBuffer(m_buffer.Get());
}

Ref<StagingBuffer> StagingBuffer::Create(
    uint32 in_bufferByteWidth,
    bool   in_cpuWrite,
    bool   in_cpuRead)
{
    auto sb               = CreateRef<StagingBuffer>();
    sb->m_buffer          = ID3D11BufferUtil::CreateStagingBuffer(in_bufferByteWidth, in_cpuWrite, in_cpuRead);
    sb->m_bufferByteWidth = in_bufferByteWidth;
    return sb;
}

void StagingBuffer::WriteToBuffer(const void* in_data, uint32 in_dataByteWidth) const
{
    ID3D11BufferUtil::WriteToStagingBuffer(m_buffer.Get(), in_data, in_dataByteWidth);
}

void StagingBuffer::ReadFromBuffer(void* out_data, uint32 in_dataByteWidth) const
{
    in_dataByteWidth = std::min(in_dataByteWidth, m_bufferByteWidth);
    ID3D11BufferUtil::ReadFromBuffer(m_buffer.Get(), out_data, in_dataByteWidth);
}

void StagingBuffer::CopyTo(ID3D11Buffer* in_destBuffer, uint32 in_copyDataByteWidth) const
{
    ID3D11BufferUtil::CopyBetween(m_buffer.Get(), in_destBuffer, in_copyDataByteWidth);
}

void StagingBuffer::CopyFrom(ID3D11Buffer* in_srcBuffer, uint32 in_copyDataByteWidth) const
{
    ID3D11BufferUtil::CopyBetween(in_srcBuffer, m_buffer.Get(), in_copyDataByteWidth);
}

uint32 StagingBuffer::GetBufferByteWidth() const
{
    return m_bufferByteWidth;
}

void VertexBuffer::Bind() const
{
    GetRenderer().SetVertexBuffer(m_buffer.Get(), m_vertexStride, 0);
}

void StructuredBufferBase::BindUAV(uint32 in_slot) const
{
    m_uav->BindUAV(in_slot);
}

void StructuredBufferBase::BindImage(uint32 in_slot, eShaderFlags in_bindFlags) const
{
    m_uav->BindImage(in_slot, in_bindFlags);
}

void ConstantBufferBase::Bind(uint32 in_slot, eShaderFlags in_bindFlags) const
{
    GetRenderer().SetConstantBuffer(m_buffer.Get(), in_slot, in_bindFlags);
}

Ref<UnorderedAccessView> UnorderedAccessView::Create(ID3D11Buffer* in_buffer)
{
    auto uav       = CreateRef<UnorderedAccessView>();
    uav->m_uav     = ID3D11UnorderedAccessViewUtil::CreateUAV(in_buffer);
    uav->m_image2D = Image2D::CreateFromBuffer(in_buffer);
    return uav;
}

void UnorderedAccessView::BindUAV(uint32 in_slot) const
{
    // todo: need conflict handle with Image2D
    GetRenderer().SetUnorderedAccessView(m_uav.Get(), in_slot);
}

void UnorderedAccessView::BindImage(uint32 in_slot, eShaderFlags in_bindFlags) const
{
    m_image2D->Bind(in_slot, in_bindFlags);
}

}   // namespace crab