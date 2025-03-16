#include "CrabPch.h"

#include "D11Renderer.h"
#include "D11ResourceList.h"
#include "RenderStates.h"
#include "Textures.h"

namespace crab
{

//===================================================
// Constant Buffer State
//===================================================

void ConstantList::Init(const std::vector<ConstantNode>& in_constants)
{
    m_constantBuffers = std::move(in_constants);
}


void ConstantList::Bind() const
{
    for (const auto& [buffer, slot, bindFlags]: m_constantBuffers)
        buffer->Bind(slot, bindFlags);
}

//===================================================
// TextureList
//===================================================

void TextureList::Init(const std::vector<TextureNode>& in_constants)
{
    m_textures = std::move(in_constants);
}

void TextureList::Bind() const
{
    for (const auto& [image, slot, bindFlags]: m_textures)
        image->Bind(slot, bindFlags);
}

//===================================================
// Sampler List
//===================================================

void SamplerList::Init(const std::vector<SamplerNode>& in_samplers)
{
    m_samplers = std::move(in_samplers);
}

void SamplerList::Bind() const
{
    for (const auto& [sampler, slot, bindFlags]: m_samplers)
        sampler->Bind(slot, bindFlags);
}

}   // namespace crab