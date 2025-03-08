#include "pch.h"

#include "GlobalConstants.h"

void GlobalConstants::Init()
{
    m_cameraConstant       = ConstantBuffer<CameraConstant>::Create();
    m_mirrorCameraConstant = ConstantBuffer<CameraConstant>::Create();
    m_drawNormalGSConstant = ConstantBuffer<DrawNormalConstant>::Create();
    m_skyboxPSConstant     = ConstantBuffer<SkyboxConstant>::Create();
    m_shadowCameraConstant = ConstantBuffer<CameraConstant>::Create();
    m_transformConstant    = ConstantBuffer<TransformConstant>::Create();
    m_materialConstant     = ConstantBuffer<MaterialConstant>::Create();

    m_lightConstant          = ConstantBuffer<LightConstant>::Create();
    m_cascadeShadowConstant  = ConstantBuffer<CascadeShadowConstant>::Create();
    m_depthVisualizeConstant = ConstantBuffer<DepthVisualizeConstant>::Create();

    m_basicShaderCasterConstant   = ConstantBuffer<BasicShadowCasterConstant>::Create();
    m_cascadeShadowCasterConstant = ConstantBuffer<CascadeShadowCasterConstant>::Create();
    m_omniShadowCasterConstant    = ConstantBuffer<OmniShadowCasterConstant>::Create();
}

void GlobalConstants::UpdateTransform(const TransformConstant& in_data) const
{
    CRAB_ASSERT(m_transformConstant, "Transform constant buffer is not initialized.");
    m_transformConstant->WriteToBuffer(in_data);
}

void GlobalConstants::UpdateCamera(const CameraConstant& in_data) const
{
    CRAB_ASSERT(m_cameraConstant, "Camera constant buffer is not initialized.");
    m_cameraConstant->WriteToBuffer(in_data);
}

void GlobalConstants::UpdateReflectCamera(const CameraConstant& in_data) const
{
    CRAB_ASSERT(m_mirrorCameraConstant, "Mirror camera constant buffer is not initialized.");
    m_mirrorCameraConstant->WriteToBuffer(in_data);
}

void GlobalConstants::UpdateDrawNormalFactor(const DrawNormalConstant& in_data) const
{
    CRAB_ASSERT(m_drawNormalGSConstant, "DrawNormalGS constant buffer is not initialized.");
    m_drawNormalGSConstant->WriteToBuffer(in_data);
}

void GlobalConstants::UpdateSkyboxPS(const SkyboxConstant& in_data) const
{
    CRAB_ASSERT(m_skyboxPSConstant, "SkyboxPS constant buffer is not initialized.");
    m_skyboxPSConstant->WriteToBuffer(in_data);
}

void GlobalConstants::UpdateShaderCamera(const CameraConstant& in_data) const
{
    CRAB_ASSERT(m_shadowCameraConstant, "Shadow camera constant buffer is not initialized.");
    m_shadowCameraConstant->WriteToBuffer(in_data);
}

void GlobalConstants::UpdateMaterial(const MaterialConstant& in_data) const
{
    CRAB_ASSERT(m_materialConstant, "Material constant buffer is not initialized.");
    m_materialConstant->WriteToBuffer(in_data);
}

void GlobalConstants::UpdateLight(const LightConstant& in_data) const
{
    CRAB_ASSERT(m_lightConstant, "Light constant buffer is not initialized.");
    m_lightConstant->WriteToBuffer(in_data);
        
}

void GlobalConstants::UpdateDepthVisualize(const DepthVisualizeConstant& in_data) const
{
    m_depthVisualizeConstant->WriteToBuffer(in_data);
}

void GlobalConstants::UpdateBasicShadowCaster(const BasicShadowCasterConstant& in_data) const
{
    return m_basicShaderCasterConstant->WriteToBuffer(in_data);
}

void GlobalConstants::UpdateCascadeShadowCaster(const CascadeShadowCasterConstant& in_data) const
{
    return m_cascadeShadowCasterConstant->WriteToBuffer(in_data);
}

void GlobalConstants::UpdateOmniShadowCaster(const OmniShadowCasterConstant& in_data) const
{
    return m_omniShadowCasterConstant->WriteToBuffer(in_data);
}

void GlobalConstants::UpdateCascadeShadow(const CascadeShadowConstant& in_data) const
{
    CRAB_ASSERT(m_cascadeShadowConstant, "Cascade shadow constant buffer is not initialized.");
    m_cascadeShadowConstant->WriteToBuffer(in_data);
}