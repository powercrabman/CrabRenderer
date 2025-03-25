#pragma once

namespace crab
{

class SamplerState;
class DepthStencilState;
class RasterizerState;
class BlendState;

class GlobalState
{
public:
    static void Init();

    //===================================================
    // Sampler State
    //===================================================

    static Ref<SamplerState> Sampler_Point_Wrap();
    static Ref<SamplerState> Sampler_Point_Clamp();
    static Ref<SamplerState> Sampler_Linear_Wrap();
    static Ref<SamplerState> Sampler_Linear_Clamp();
    static Ref<SamplerState> Sampler_Comparison_Linear_Wrap();
    static Ref<SamplerState> Sampler_Comparison_Linear_Clamp();

    //===================================================
    // Depth Stencil State
    //===================================================

    static Ref<DepthStencilState> DepthStencil_NoDepth_NoStencil();
    static Ref<DepthStencilState> DepthStencil_Depth_NoStencil();
    static Ref<DepthStencilState> DepthStencil_Depth_WriteStencil();
    static Ref<DepthStencilState> DepthStencil_Depth_DrawOnStencil();

    //===================================================
    // Rasterizer State
    //===================================================

    static Ref<RasterizerState> Rasterizer_Solid_CullNone(bool in_MSAAEnable);
    static Ref<RasterizerState> Rasterizer_Solid_CullFront(bool in_MSAAEnable);
    static Ref<RasterizerState> Rasterizer_Solid_CullBack(bool in_MSAAEnable);

    static Ref<RasterizerState> Rasterizer_Wireframe_CullNone(bool in_MSAAEnable);
    static Ref<RasterizerState> Rasterizer_Wireframe_CullFront(bool in_MSAAEnable);
    static Ref<RasterizerState> Rasterizer_Wireframe_CullBack(bool in_MSAAEnable);

    //===================================================
    // Blend State
    //===================================================

    static Ref<BlendState> Blend_NoBlend(bool in_MSAAEnable);
    static Ref<BlendState> Blend_Blend_BlendFactor(bool in_MSAAEnable);
    static Ref<BlendState> Blend_Blend_AlphaValue(bool in_MSAAEnable);
    static Ref<BlendState> Blend_Add_BlendFactor(bool in_MSAAEnable);
    static Ref<BlendState> Blend_Add_AlphaValue(bool in_MSAAEnable);

private:
    struct Impl;
    static Scope<Impl> s_impl;
};

}   // namespace crab
