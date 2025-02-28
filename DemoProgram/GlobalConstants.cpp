#include "pch.h"

#include "GlobalConstants.h"

void GlobalConstants::Init()
{
    m_cameraConstant       = ConstantBuffer<CameraConstant>::Create();
    m_mirrorCameraConstant = ConstantBuffer<CameraConstant>::Create();
    m_drawNormalGSConstant = ConstantBuffer<DrawNormalFactor>::Create();
    m_skyboxPSConstant     = ConstantBuffer<SkyboxPSConstant>::Create();
    m_shadowCameraConstant = ConstantBuffer<CameraConstant>::Create();
    m_transformConstant    = ConstantBuffer<TransformConstant>::Create();
    m_materialConstant     = ConstantBuffer<MaterialConstant>::Create();

    m_lightAttributeConstant = ConstantBuffer<LightAttributeConstant>::Create();
    m_lightTransformConstant = ConstantBuffer<LightTransformConstant>::Create();
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

void GlobalConstants::UpdateDrawNormalFactor(const DrawNormalFactor& in_data) const
{
    CRAB_ASSERT(m_drawNormalGSConstant, "DrawNormalGS constant buffer is not initialized.");
    m_drawNormalGSConstant->WriteToBuffer(in_data);
}

void GlobalConstants::UpdateSkyboxPS(const SkyboxPSConstant& in_data) const
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

void GlobalConstants::UpdateLightAttribute(const LightAttributeConstant& in_data) const
{
    m_lightAttributeConstant->WriteToBuffer(in_data);
}

void GlobalConstants::UpdateLightTransform(const LightTransformConstant& in_data) const
{
    m_lightTransformConstant->WriteToBuffer(in_data);
}