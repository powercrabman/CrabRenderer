#pragma once

namespace crab
{

class ConstantBufferBase;
class SamplerState;
class ShaderResource;

struct ConstantNode
{
    Ref<ConstantBufferBase> buffer;
    uint32                  slot;
    eShaderFlags            bindFlags;
};

struct ShaderResourceNode
{
    Ref<ShaderResource> shaderResource;
    uint32              slot;
    eShaderFlags        bindFlags;
};

struct SamplerNode
{
    Ref<SamplerState> sampler;
    uint32            slot;
    eShaderFlags      bindFlags;
};

}   // namespace crab