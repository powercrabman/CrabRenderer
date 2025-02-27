#pragma once

#include "D11Utils.h"

namespace crab
{

//===================================================
// Vertex Buffer
//===================================================

class VertexBuffer
{
    D11_RESOURCE_CTOR(VertexBuffer);

public:
    template<typename VertexType>
    static Ref<VertexBuffer> Create(const std::vector<VertexType>& in_vertices)
    {
        auto vb = CreateRef<VertexBuffer>();

        vb->m_buffer = ID3D11BufferUtil::CreateVertexBuffer(
            sizeof(VertexType),
            static_cast<uint32>(in_vertices.size()),
            in_vertices.data());

        vb->m_vertexCount  = static_cast<uint32>(in_vertices.size());
        vb->m_vertexStride = sizeof(VertexType);

        return vb;
    }

    uint32 GetVertexStride() const { return m_vertexStride; }
    uint32 GetVertexCount() const { return m_vertexCount; }
    void   Bind() const;

private:
    ComPtr<ID3D11Buffer> m_buffer;
    uint32               m_vertexCount  = 0;
    uint32               m_vertexStride = 0;
    // uint32 m_offset       = 0;
};

//===================================================
// Index Buffer
//===================================================

class IndexBuffer
{
    D11_RESOURCE_CTOR(IndexBuffer);

public:
    static Ref<IndexBuffer> Create(const std::vector<uint32>& in_indices);

    void   Bind() const;
    uint32 GetIndexCount() const { return m_indexCount; }

private:
    ComPtr<ID3D11Buffer> m_buffer;
    uint32               m_indexCount = 0;
};

//===================================================
// Staging Buffer
//===================================================

class StagingBuffer
{
    D11_RESOURCE_CTOR(StagingBuffer);

public:
    static Ref<StagingBuffer> Create(uint32 in_bufferByteWidth, bool in_cpuWrite, bool in_cpuRead);

    void WriteToBuffer(const void* in_data, uint32 in_dataByteWidth) const;
    void ReadFromBuffer(void* out_data, uint32 in_dataByteWidth) const;

    // argument buffer will be DEFAULT_USAGE
    // DYNAMIC_USAGE can use Map/Unmap
    void CopyTo(ID3D11Buffer* in_destBuffer, uint32 in_copyDataByteWidth) const;
    void CopyFrom(ID3D11Buffer* in_srcBuffer, uint32 in_copyDataByteWidth) const;

    uint32 GetBufferByteWidth() const;

private:
    ComPtr<ID3D11Buffer> m_buffer;
    uint32               m_bufferByteWidth = 0;
};

//===================================================
// Unordered Access View
//===================================================
class Image2D;

class UnorderedAccessView
{
    D11_RESOURCE_CTOR(UnorderedAccessView);

public:
    static Ref<UnorderedAccessView> Create(ID3D11Buffer* in_buffer);

    void BindUAV(uint32 in_slot) const;
    void BindImage(uint32 in_slot, eShaderFlags in_bindFlags) const;

    ID3D11UnorderedAccessView* GetUAV() const { return m_uav.Get(); }
    Ref<Image2D>               GetImage2D() const { return m_image2D; }

private:
    ComPtr<ID3D11UnorderedAccessView> m_uav;
    Ref<Image2D>                      m_image2D;
};

//===================================================
// Structured Buffer
//===================================================

class StructuredBufferBase
{
public:
    ~StructuredBufferBase() = default;

    void BindUAV(uint32 in_slot) const;
    void BindImage(uint32 in_slot, eShaderFlags in_bindFlags) const;

    Ref<UnorderedAccessView> GetUAV() const { return m_uav; }

    uint32 GetItemMaxCount() const { return m_itemMaxCount; }

protected:
    StructuredBufferBase() = default;

    ComPtr<ID3D11Buffer> m_buffer;
    Ref<StagingBuffer>   m_stagingBuffer;

    Ref<UnorderedAccessView> m_uav;

    uint32 m_itemMaxCount = 0;
};

template<typename T>
class StructuredBuffer : public StructuredBufferBase
{
    D11_RESOURCE_CTOR(StructuredBuffer);

public:
    static Ref<StructuredBuffer<T>> Create(const std::vector<T>& in_data)
    {
        auto sb = CreateRef<StructuredBuffer<T>>();

        sb->m_buffer = ID3D11BufferUtil::CreateStructuredBuffer(
            static_cast<uint32>(in_data.size()),
            sizeof(T),
            in_data.data());

        sb->m_stagingBuffer = StagingBuffer::Create(
            static_cast<uint32>(in_data.size() * sizeof(T)),
            true,
            true);

        sb->m_uav          = UnorderedAccessView::Create(sb->m_buffer.Get());
        sb->m_itemMaxCount = static_cast<uint32>(in_data.size());
        return sb;
    }

    void WriteToBuffer(const std::vector<T>& in_data)
    {
        uint32 size = static_cast<uint32>(in_data.size()) * sizeof(T);
        size        = std::clamp(size, 0u, static_cast<uint32>(m_itemMaxCount * sizeof(T)));

        m_stagingBuffer->WriteToBuffer(in_data.data(), size);
        m_stagingBuffer->CopyTo(m_buffer.Get(), size);
    }

    std::vector<T> ReadFromBuffer()
    {
        std::vector<T> data(m_itemMaxCount);

        m_stagingBuffer->CopyFrom(m_buffer.Get(),
                                  static_cast<uint32>(data.size() * sizeof(T)));

        m_stagingBuffer->ReadFromBuffer(data.data(),
                                        static_cast<uint32>(data.size() * sizeof(T)));
        return data;
    }
};

//===================================================
// Constant Buffer
//===================================================

class ConstantBufferBase
{
public:
    void Bind(uint32 in_slot, eShaderFlags in_bindFlags) const;

    ComPtr<ID3D11Buffer> Get() const { return m_buffer; }
    uint32               GetItemByteStride() const { return m_itemByteStride; }
    TypeInfo             GetItemType() const { return m_itemType; }

protected:
    ConstantBufferBase() = default;

    ComPtr<ID3D11Buffer> m_buffer;
    uint32               m_itemByteStride = 0;

    TypeInfo m_itemType = {};
};

template<typename Ty>
class ConstantBuffer : public ConstantBufferBase
{
    D11_RESOURCE_CTOR(ConstantBuffer);

public:
    static Ref<ConstantBuffer<Ty>> Create();
    static Ref<ConstantBuffer<Ty>> Create(const Ty& in_data);

    void WriteToBuffer(const Ty& in_data);

private:
    Ty in_cpuData = {};
};

template<typename Ty>
Ref<ConstantBuffer<Ty>> ConstantBuffer<Ty>::Create()
{
    auto cb              = CreateRef<ConstantBuffer<Ty>>();
    cb->m_buffer         = ID3D11BufferUtil::CreateConstantBuffer(sizeof(Ty));
    cb->m_itemByteStride = sizeof(Ty);
    cb->m_itemType       = TypeInfo::Get<Ty>();
    return cb;
}

template<typename Ty>
Ref<ConstantBuffer<Ty>> ConstantBuffer<Ty>::Create(const Ty& in_data)
{
    auto cb = ConstantBuffer<Ty>::Create();
    cb->WriteToBuffer(in_data);   // this code can be optimized, but it's okay for now
    return cb;
}

template<typename Ty>
void ConstantBuffer<Ty>::WriteToBuffer(const Ty& in_data)
{
    {
        ID3D11BufferUtil::WriteToDynamicBuffer(
            m_buffer.Get(),
            &in_data,
            sizeof(Ty));

        in_cpuData = in_data;
    }

}

}   // namespace crab
