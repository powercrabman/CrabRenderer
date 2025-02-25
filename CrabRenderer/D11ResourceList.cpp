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

ConstantBufferList& ConstantBufferList::Add(
    const Ref<ConstantBufferBase>& in_constantBuffer,
    uint32                         in_slot,
    eShaderFlags                   in_bindFlags)
{
    m_constantBuffers.emplace_back(in_constantBuffer, in_slot, in_bindFlags);
    return *this;
}

void ConstantBufferList::ClearList()
{
    m_constantBuffers.clear();
}

void ConstantBufferList::Bind() const
{
    for (const auto& [buffer, slot, bindFlags]: m_constantBuffers)
        buffer->Bind(slot, bindFlags);
}

//===================================================
// Image2D List
//===================================================

Image2DList& Image2DList::Add(const Ref<Image2D>& in_image, uint32 in_slot, eShaderFlags in_bindFlags)
{
    m_images.emplace_back(in_image, in_slot, in_bindFlags);
    return *this;
}

void Image2DList::ClearList()
{
    m_images.clear();
}

void Image2DList::Bind() const
{
    for (const auto& [image, slot, bindFlags]: m_images)
        image->Bind(slot, bindFlags);
}

//===================================================
// Sampler List
//===================================================

SamplerStateList& SamplerStateList::Add(const Ref<SamplerState>& in_sampler, uint32 in_slot, eShaderFlags in_bindFlags)
{
    m_samplers.emplace_back(in_sampler, in_slot, in_bindFlags);
    return *this;
}

void SamplerStateList::ClearList()
{
    m_samplers.clear();
}

void SamplerStateList::Bind() const
{
    for (const auto& [sampler, slot, bindFlags]: m_samplers)
        sampler->Bind(slot, bindFlags);
}

}   // namespace crab