#include "CrabPch.h"

#include "GlobalConstant.h"

#include "RenderFactory.h"

namespace crab
{

void GlobalConstant::Init()
{
    m_transform = CreateConstantBuffer<TransformConstant>();
    m_camera    = CreateConstantBuffer<CameraConstant>();
    m_reflect   = CreateConstantBuffer<CameraConstant>();

    m_light     = CreateConstantBuffer<LightConstant>();
    m_skybox    = CreateConstantBuffer<SkyboxConstant>();
    m_shadow    = CreateConstantBuffer<ShadowReceiverConstant>();

    m_material       = CreateConstantBuffer<MaterialConstant>();

    m_shadowCaster        = CreateConstantBuffer<BasicShadowCasterConstant>();
    m_omniShadowCaster    = CreateConstantBuffer<OmniShadowCasterConstant>();
    m_cascadeShadowCaster = CreateConstantBuffer<CascadeShadowCasterConstant>();
}

}   // namespace crab