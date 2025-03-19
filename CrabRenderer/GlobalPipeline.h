#pragma once

#include "PipelineState.h"

namespace crab
{

class GlobalPipeline
{
public:
    void Init();

    // Begin
    void BeginDrawNormal(const PipelineBindArgument& in_arg) const { m_drawNormal->Bind(in_arg); }

    void BeginPBRGeometry(const PipelineBindArgument& in_arg) const { m_PBRGeometry->Bind(in_arg); }
    void BeginPBRGeometryWireframe(const PipelineBindArgument& in_arg) const { m_PBRGeometryWireframe->Bind(in_arg); }
    void BeginPBRLighting(const PipelineBindArgument& in_arg) const { m_PBRLighting->Bind(in_arg); }

    void BeginSkybox(const PipelineBindArgument& in_arg) const { m_skybox->Bind(in_arg); }

    void BeginMask(const PipelineBindArgument& in_arg) const { m_mask->Bind(in_arg); }
    void BeginMaskBackface(const PipelineBindArgument& in_arg) const { m_maskBackface->Bind(in_arg); }

    void BeginBasicShadowCaster(const PipelineBindArgument& in_arg) const { m_basicShadowCaster->Bind(in_arg); }
    void BeginCascadeShadowCaster(const PipelineBindArgument& in_arg) const { m_cascadeShadowCaster->Bind(in_arg); }
    void BeginOmniShadowCaster(const PipelineBindArgument& in_arg) const { m_omniShadowCaster->Bind(in_arg); }

    // Getter
    NODISCARD Ref<PipelineState> GetDrawNormal() const { return m_drawNormal; }

    NODISCARD Ref<PipelineState> GetPBRGeometry() const { return m_PBRGeometry; }
    NODISCARD Ref<PipelineState> GetPBRGeometryWireframe() const { return m_PBRGeometryWireframe; }
    NODISCARD Ref<PipelineState> GetPBRLighting() const { return m_PBRLighting; }

    NODISCARD Ref<PipelineState> GetSkybox() const { return m_skybox; }

    NODISCARD Ref<PipelineState> GetMask() const { return m_mask; }
    NODISCARD Ref<PipelineState> GetMaskBackface() const { return m_maskBackface; }

    NODISCARD Ref<PipelineState> GetBasicShadowCaster() const { return m_basicShadowCaster; }
    NODISCARD Ref<PipelineState> GetCascadeShadowCaster() const { return m_cascadeShadowCaster; }
    NODISCARD Ref<PipelineState> GetOmniShadowCaster() const { return m_omniShadowCaster; }

private:
    Ref<PipelineState> m_drawNormal;

    Ref<PipelineState> m_PBRGeometry;
    Ref<PipelineState> m_PBRGeometryWireframe;
    Ref<PipelineState> m_PBRLighting;

    Ref<PipelineState> m_skybox;

    Ref<PipelineState> m_mask;
    Ref<PipelineState> m_maskBackface;

    Ref<PipelineState> m_basicShadowCaster;
    Ref<PipelineState> m_cascadeShadowCaster;
    Ref<PipelineState> m_omniShadowCaster;

    // Ref<PipelineState> m_skyboxReflection;
    //  Ref<PipelineState> m_f_reflectPBR;
    //  Ref<PipelineState> m_f_reflectWireframePBR;

    // Ref<PipelineState> m_f_PBROnMask;
    // Ref<PipelineState> m_f_wireframePBROnMask;
};

}   // namespace crab