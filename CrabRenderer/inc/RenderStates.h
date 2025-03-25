#pragma once

namespace crab
{

//===================================================
// Depth Stencil State
//===================================================

class DepthStencilState
{
public:
    void Init(
        bool             in_depthTestEnable,
        bool             in_depthWriteEnable,
        eComparisonFunc  in_depthFunc,
        bool             in_stencilEnable,
        eBlendWriteFlags in_stencilReadMask,
        eBlendWriteFlags in_stencilWriteMask,
        eComparisonFunc  in_frontFaceStencilFunc,
        eStencilOperator in_frontFaceDepthPassStencilFail,
        eStencilOperator in_frontFaceDepthFailStencilPass,
        eStencilOperator in_frontFaceDepthPassStencilPass,
        eComparisonFunc  in_backFaceStencilFunc,
        eStencilOperator in_backFaceDepthPassStencilFail,
        eStencilOperator in_backFaceDepthFailStencilPass,
        eStencilOperator in_backFaceDepthPassStencilPass);

    void Bind(uint32 in_stencilRef) const;

private:
    ComPtr<ID3D11DepthStencilState> m_depthStencil;

    inline static RenderResourceIDGenerator s_idGenerator;
    DepthStencilStateID                     m_stateID = 0;
};

//===================================================
// Rasterizer State
//===================================================

class RasterizerState
{
public:
    void Init(eFillMode in_fillMode,
              eCullMode in_cullMode,
              int32     in_depthBias,
              float     in_depthBiasClamp,
              float     in_slopeScaledDepthBias,
              bool      in_depthClipEnable,
              bool      in_scissorEnable,
              bool      in_MSAAEnable);

    void Bind() const;

private:
    ComPtr<ID3D11RasterizerState> m_rasterizerState;

    inline static RenderResourceIDGenerator s_idGenerator;
    RasterizerStateID                       m_stateID = 0;
};

//===================================================
// Blend State
//===================================================

class BlendState
{
public:
    void Init(bool             in_alphaToCoverageEnable,
              bool             in_blendEnable,
              eBlendFactor     in_srcBlendFactor,
              eBlendFactor     in_destBlendFactor,
              eBlendOperator   in_blendOperator,
              eBlendFactor     in_srcBlendAlphaFactor,
              eBlendFactor     in_destBlendAlphaFactor,
              eBlendOperator   in_blendAlphaOperator,
              eBlendWriteFlags in_renderTargetWriteMask);

    void Bind(const std::array<float, 4>& in_blendFactors) const;

private:
    ComPtr<ID3D11BlendState> m_blendState;

    inline static RenderResourceIDGenerator s_idGenerator;
    BlendStateID                            m_stateID = 0;
};

//===================================================
// Sampler State
//===================================================

class SamplerState
{
public:
    void Init(eSamplerMode        in_mode,
              eSamplerFilter      in_filter,
              eSamplerAddressMode in_addressU,
              eSamplerAddressMode in_addressV,
              eSamplerAddressMode in_addressW,
              float               in_mipLODBias,
              uint32              in_maxAnisotropy,
              eComparisonFunc     in_comparisonFunc,
              const Color4&       in_borderColor,
              float               in_minLOD,
              float               in_maxLOD);

    void Bind(uint32 in_slot, eShaderFlags in_bindFlags) const;

    ComPtr<ID3D11SamplerState> Get() const { return m_samplerState.Get(); }

private:
    ComPtr<ID3D11SamplerState> m_samplerState;
};

}   // namespace crab