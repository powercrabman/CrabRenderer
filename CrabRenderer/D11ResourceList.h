#pragma once
#include "Buffers.h"

namespace crab
{

//===================================================
// Constant Buffer List
//===================================================

class ConstantList
{
public:
    ConstantList()  = default;
    ~ConstantList() = default;

    ConstantList& Add(
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
Ref<ConstantBuffer<DataType>> ConstantList::FindBuffer()
{
    for (const auto& [buffer, slot, bindFlags]: m_constantBuffers)
    {
        if (buffer->GetItemType() == TypeInfo::Get<DataType>())
            return std::static_pointer_cast<ConstantBuffer<DataType>>(buffer);
    }
    CRAB_DEBUG_BREAK("Cannot find the buffer. Check the buffer type.");
    return nullptr;
}

//===================================================
// Texture2D List
//===================================================

class Texture;

class TextureList
{
public:
    TextureList()  = default;
    ~TextureList() = default;

    TextureList& Add(
        const Ref<Texture>& in_texture,
        uint32              in_slot,
        eShaderFlags        in_bindFlags);

    void   ClearList();
    uint64 GetSize() const { return m_images.size(); }
    void   Bind() const;

private:
    struct Node
    {
        Ref<Texture>   image;
        uint32         slot;
        eShaderFlags   bindFlags;
    };
    std::vector<Node> m_images = {};
};

//===================================================
// Sampler State List
//===================================================

class SamplerState;

class SamplerList
{
public:
    SamplerList()  = default;
    ~SamplerList() = default;

    SamplerList& Add(
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