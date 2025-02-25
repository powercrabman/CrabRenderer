#pragma once
#include "Buffers.h"

namespace crab
{

//===================================================
// Constant Buffer List
//===================================================

class ConstantBufferList
{
public:
    ConstantBufferList()  = default;
    ~ConstantBufferList() = default;

    ConstantBufferList& Add(
        const Ref<ConstantBufferBase>& in_constantBuffer,
        uint32                         in_slot,
        eShaderFlags                   in_bindFlags);

    template<typename DataType>
    Ref<ConstantBuffer<DataType>> FindBuffer();

    void   ClearList();
    uint64 GetSize() const { return m_constantBuffers.size(); }
    void   Bind() const;

private:
    struct Node
    {
        Ref<ConstantBufferBase> buffer;
        uint32                  slot;
        eShaderFlags            bindFlags;
    };
    std::vector<Node> m_constantBuffers = {};
};

template<typename DataType>
Ref<ConstantBuffer<DataType>> ConstantBufferList::FindBuffer()
{
    for (const auto& [buffer, slot, bindFlags]: m_constantBuffers)
    {
        if (buffer->GetItemType() == GetTypeInfo<DataType>())
            return std::static_pointer_cast<ConstantBuffer<DataType>>(buffer);
    }
    CRAB_DEBUG_BREAK("Cannot find the buffer. Check the buffer type.");
    return nullptr;
}

//===================================================
// Image2D List
//===================================================

class Image2D;

class Image2DList
{
public:
    Image2DList()  = default;
    ~Image2DList() = default;

    Image2DList& Add(
        const Ref<Image2D>& in_image,
        uint32              in_slot,
        eShaderFlags        in_bindFlags);

    void   ClearList();
    uint64 GetSize() const { return m_images.size(); }
    void   Bind() const;

private:
    struct Node
    {
        Ref<Image2D> image;
        uint32       slot;
        eShaderFlags bindFlags;
    };
    std::vector<Node> m_images = {};
};

//===================================================
// Sampler State List
//===================================================

class SamplerState;

class SamplerStateList
{
public:
    SamplerStateList()  = default;
    ~SamplerStateList() = default;

    SamplerStateList& Add(
        const Ref<SamplerState>& in_sampler,
        uint32                   in_slot,
        eShaderFlags             in_bindFlags);

    void   ClearList();
    uint64 GetSize() const { return m_samplers.size(); }
    void   Bind() const;

private:
    struct Node
    {
        Ref<SamplerState> sampler;
        uint32            slot;
        eShaderFlags      bindFlags;
    };
    std::vector<Node> m_samplers = {};
};

}   // namespace crab