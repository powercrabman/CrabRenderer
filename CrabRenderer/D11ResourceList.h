#pragma once
#include "Buffers.h"

namespace crab
{

struct ResourceBindDesc
{
    uint32       slot;
    eShaderFlags bindFlags;
};

//===================================================
// Constant Buffer List
//===================================================

struct ConstantNode
{
    Ref<ConstantBufferBase> buffer;
    ResourceBindDesc        bindDesc;
};

class ConstantList
{
public:
    ConstantList()  = default;
    ~ConstantList() = default;

    void Init(const std::vector<ConstantNode>& in_constants);
    void Bind() const;

    template<typename DataType>
    Ref<ConstantBuffer<DataType>> FindBuffer();

private:
    std::vector<ConstantNode> m_constantBuffers = {};
};

template<typename DataType>
Ref<ConstantBuffer<DataType>> ConstantList::FindBuffer()
{
    for (const auto& [buffer, slot, bindFlags]: m_constantBuffers)
    {
        if (buffer->GetItemType() == TypeInfo::Get<DataType>())
            return std::static_pointer_cast<ConstantBuffer<DataType>>(buffer);
    }
    DEBUG_BREAK("Cannot find the buffer. Check the buffer type.");
    return nullptr;
}

//===================================================
// Texture2D List
//===================================================

class ShaderResource;

struct TextureNode
{
    Ref<ShaderResource> texture;
    ResourceBindDesc bindDesc;
};

class TextureList
{
public:
    TextureList()  = default;
    TextureList(const std::vector<TextureNode>& in_textures) { Init(in_textures); }
    ~TextureList() = default;

    void Init(const std::vector<TextureNode>& in_textures);
    void Bind() const;

private:
    std::vector<TextureNode> m_textures = {};
};

//===================================================
// Sampler State List
//===================================================

class SamplerState;

struct SamplerNode
{
    Ref<SamplerState> sampler;
    ResourceBindDesc  bindDesc;
};

class SamplerList
{
public:
    SamplerList() = default;
    SamplerList(const std::vector<SamplerNode>& in_samplers) { Init(in_samplers); }
    ~SamplerList() = default;

    void Init(const std::vector<SamplerNode>& in_samplers);
    void Bind() const;

private:
    std::vector<SamplerNode> m_samplers = {};
};

}   // namespace crab