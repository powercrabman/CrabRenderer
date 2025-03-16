#pragma once

#include "PipelineState.h"

namespace crab
{

class GlobalPipeline
{
public:
    void Init();

    // Begin
    void BeginDrawNormal(const PipelineBindArgument& in_arg = {}) const;

    void BeginPBR(const PipelineBindArgument& in_arg = {}) const;
    void BeginPBRWireframe(const PipelineBindArgument& in_arg = {}) const;
    void BeginPBRReflection(const PipelineBindArgument& in_arg = {}) const;
    void BeginPBRRefractionWireframe(const PipelineBindArgument& in_arg = {}) const;

    void BeginSkybox(const PipelineBindArgument& in_arg = {}) const;
    void BeginSkyboxReflection(const PipelineBindArgument& in_arg = {}) const;

    void BeginMask(const PipelineBindArgument& in_arg = {}) const;
    void BeginMaskBackface(const PipelineBindArgument& in_arg = {}) const;

    void BeginBasicShadowCaster(const PipelineBindArgument& in_arg = {}) const;
    void BeginCascadeShadowCaster(const PipelineBindArgument& in_arg = {}) const;
    void BeginOmniShadowCaster(const PipelineBindArgument& in_arg = {}) const;

    // Getter
    Ref<PipelineState> GetDrawNormal() const { return m_drawNormal; }

    Ref<PipelineState> GetPBR() const { return m_pbr; }
    Ref<PipelineState> GetPBRWireframe() const { return m_pbrWireframe; }
    Ref<PipelineState> GetPBROnMask() const { return m_pbrOnMask; }
    Ref<PipelineState> GetPBRWireframeOnMask() const { return m_pbrWireframeOnMask; }
    Ref<PipelineState> GetPBRReflection() const { return m_pbrReflection; }
    Ref<PipelineState> GetPBRRefractionWireframe() const { return m_pbrRefractionWireframe; }

    Ref<PipelineState> GetSkybox() const { return m_skybox; }
    Ref<PipelineState> GetSkyboxReflection() const { return m_skyboxReflection; }

    Ref<PipelineState> GetMask() const { return m_mask; }
    Ref<PipelineState> GetMaskBackface() const { return m_maskBackface; }

    Ref<PipelineState> GetBasicShadowCaster() const { return m_basicShadowCaster; }
    Ref<PipelineState> GetCascadeShadowCaster() const { return m_cascadeShadowCaster; }
    Ref<PipelineState> GetOmniShadowCaster() const { return m_omniShadowCaster; }

private:
    Ref<PipelineState> m_drawNormal;

    Ref<PipelineState> m_pbr;
    Ref<PipelineState> m_pbrWireframe;
    Ref<PipelineState> m_pbrOnMask;
    Ref<PipelineState> m_pbrWireframeOnMask;
    Ref<PipelineState> m_pbrReflection;
    Ref<PipelineState> m_pbrRefractionWireframe;

    Ref<PipelineState> m_skybox;
    Ref<PipelineState> m_skyboxReflection;

    Ref<PipelineState> m_mask;
    Ref<PipelineState> m_maskBackface;

    Ref<PipelineState> m_basicShadowCaster;
    Ref<PipelineState> m_cascadeShadowCaster;
    Ref<PipelineState> m_omniShadowCaster;
};

}   // namespace crab