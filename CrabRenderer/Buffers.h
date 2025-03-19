#pragma once

#include "RenderUtils.h"

namespace crab
{
class ShaderResourceBuffer;
class ShaderResource;

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
        m_buffer       = ID3D11BufferUtil::CreateBuffer(
            static_cast<uint32>(in_vertices.size() * sizeof(VertexType)),
            D3D11_USAGE_IMMUTABLE,
            eBindFlags_VertexBuffer,
            eCPUAccessFlags_None,
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
    void Init(const std::vector<Index>& in_indices);

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

    void WriteToBuffer(const void* in_writeData, uint32 in_dataByteWidth) const;
    void ReadFromBuffer(void* out_data, OPTIONAL uint32* out_dataByteWidth) const;

    // argument buffer will be DEFAULT_USAGE
    // DYNAMIC_USAGE can use Map/Unmap
    void CopyTo(ID3D11Buffer* in_destBuffer) const;
    void CopyFrom(ID3D11Buffer* in_srcBuffer) const;

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
    void BindSRV(uint32 in_slot, eShaderFlags in_bindFlags) const;

    ID3D11UnorderedAccessView* GetUAV() const { return m_uav.Get(); }
    Ref<ShaderResourceBuffer>  GetShaderResource() const { return m_shaderResource; }

private:
    ComPtr<ID3D11UnorderedAccessView> m_uav;
    Ref<ShaderResourceBuffer>         m_shaderResource;
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

    uint32 GetCapacity() const { return m_capacity; }

protected:
    StructuredBufferBase() = default;

    Ref<StagingBuffer> m_stagingBuffer;

    Ref<ID3D11Buffer>        m_gpuBuffer;
    Ref<UnorderedAccessView> m_uav;

    uint32 m_capacity = 0;
};

template<typename T>
class StructuredBuffer : public StructuredBufferBase
{
public:
    void Init(const std::vector<T>& in_data)
    {
        uint32 byteWidth = static_cast<uint32>(in_data.size() * sizeof(T));

        m_gpuBuffer = ID3D11BufferUtil::CreateBuffer(
            byteWidth,
            D3D11_USAGE_DEFAULT,
            eBindFlags_ShaderResource | eBindFlags_UnorderedAccess,
            eCPUAccessFlags_None,
            in_data.data());

        m_stagingBuffer = CreateRef<StagingBuffer>();
        m_stagingBuffer->Init(static_cast<uint32>(in_data.size() * sizeof(T)),
                              true,
                              true);

        m_uav = CreateRef<UnorderedAccessView>();
        m_uav->Init(m_gpuBuffer.get());

        m_capacity = static_cast<uint32>(in_data.size());
    }

    void WriteToBuffer(const std::vector<T>& in_data)
    {
        uint32 size = static_cast<uint32>(in_data.size()) * sizeof(T);
        size        = std::clamp(size, 0u, static_cast<uint32>(m_capacity * sizeof(T)));

        m_stagingBuffer->WriteToBuffer(in_data, size);
        m_stagingBuffer->CopyTo(m_gpuBuffer.get());
    }

    std::vector<T> ReadFromBuffer()
    {
        m_stagingBuffer->CopyFrom(m_gpuBuffer.get());

        uint32 dataByteWidth;
        void*  dataPtr;
        m_stagingBuffer->ReadFromBuffer(&dataPtr, &dataByteWidth);

        std::vector<T> output;
        output.reserve(m_capacity);

        uint32 elementCount = dataByteWidth / sizeof(T);
        output.insert(output.end(), static_cast<T*>(dataPtr), static_cast<T*>(dataPtr) + elementCount);

        return output;
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
    m_buffer = ID3D11BufferUtil::CreateBuffer(
        sizeof(Ty),
        D3D11_USAGE_DYNAMIC,
        eBindFlags_ConstantBuffer,
        eCPUAccessFlags_Write,
        static_cast<const void*>(&in_data));

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
