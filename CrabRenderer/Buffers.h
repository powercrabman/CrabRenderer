#pragma once

#include "D11Utils.h"

namespace crab
{

//===================================================
// Vertex Buffer
//===================================================

class VertexBuffer
{
public:
    VertexBuffer()  = default;
    ~VertexBuffer() = default;

    template<typename VertexType>
    void Init(const std::vector<VertexType>& in_vertices)
    {
        m_vertexCount  = static_cast<uint32>(in_vertices.size());
        m_vertexStride = sizeof(VertexType);
        m_buffer       = ID3D11BufferUtil::CreateVertexBuffer(
            static_cast<uint32>(in_vertices.size()),
            sizeof(VertexType),
            in_vertices.data());
    }

    uint32 GetVertexStride() const { return m_vertexStride; }
    uint32 GetVertexCount() const { return m_vertexCount; }
    void   Bind() const;

private:
    ComPtr<ID3D11Buffer> m_buffer;
    uint32               m_vertexCount  = 0;
    uint32               m_vertexStride = 0;
};

//===================================================
// Index Buffer
//===================================================

class IndexBuffer
{
public:
    void Init(const std::vector<uint32>& in_indices);

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
public:
    void Init(uint32 in_bufferByteWidth, bool in_cpuWrite, bool in_cpuRead);

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
class Texture2D;

class UnorderedAccessView
{
public:
    void Init(ID3D11Buffer* in_buffer);

    void BindUAV(uint32 in_slot) const;
    void BindImage(uint32 in_slot, eShaderFlags in_bindFlags) const;

    ID3D11UnorderedAccessView* GetUAV() const { return m_uav.Get(); }
    Ref<Texture2D>             GetImage2D() const { return m_texture; }

private:
    ComPtr<ID3D11UnorderedAccessView> m_uav;
    Ref<Texture2D>                    m_texture;
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
public:
    void Init(const std::vector<T>& in_data)
    {
        m_buffer = ID3D11BufferUtil::CreateStructuredBuffer(
            static_cast<uint32>(in_data.size()),
            sizeof(T),
            in_data.data());

        m_stagingBuffer = CreateRef<StagingBuffer>();
        m_stagingBuffer->Init(
            static_cast<uint32>(in_data.size() * sizeof(T)),
            true,
            true);

        m_uav = CreateRef<UnorderedAccessView>();
        m_uav->Init(m_buffer.Get());

        m_itemMaxCount = static_cast<uint32>(in_data.size());
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
public:
    void Init();
    void Init(const Ty& in_data);

    void WriteToBuffer(const Ty& in_data);

private:
    Ty in_cpuData = {};
};

template<typename Ty>
void ConstantBuffer<Ty>::Init()
{
    Init({});
}

template<typename Ty>
void ConstantBuffer<Ty>::Init(const Ty& in_data)
{
    m_buffer         = ID3D11BufferUtil::CreateConstantBuffer(sizeof(Ty));
    m_itemByteStride = sizeof(Ty);
    m_itemType       = TypeInfo::Get<Ty>();
    ZeroMemory(&in_cpuData, sizeof(Ty));

    WriteToBuffer(in_data);
}

template<typename Ty>
void ConstantBuffer<Ty>::WriteToBuffer(const Ty& in_data)
{
    if (std::memcmp(&in_cpuData, &in_data, sizeof(Ty)) != 0)
    {
        ID3D11BufferUtil::WriteToDynamicBuffer(
            m_buffer.Get(),
            &in_data,
            sizeof(Ty));

        in_cpuData = in_data;
    }
}

}   // namespace crab
