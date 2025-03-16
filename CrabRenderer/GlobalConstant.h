#pragma once
#include "Buffers.h"
#include "Constants.h"

namespace crab
{

class GlobalConstant
{
public:
    void Init();

    //===================================================
    // Update
    //===================================================

    void UpdateTransform(const TransformConstant& in_data) const { m_transform->WriteToBuffer(in_data); }
    void UpdateCamera(const CameraConstant& in_data) const { m_camera->WriteToBuffer(in_data); }
    void UpdateReflectCamera(const CameraConstant& in_data) const { m_reflect->WriteToBuffer(in_data); }
    void UpdateLight(const LightConstant& in_data) const { m_light->WriteToBuffer(in_data); }
    void UpdateSkybox(const SkyboxConstant& in_data) const { m_skybox->WriteToBuffer(in_data); }
    void UpdateShadowReceiver(const ShadowReceiverConstant& in_data) const { m_shadow->WriteToBuffer(in_data); }

    void UpdateMaterial(const MaterialConstant& in_data) const { m_material->WriteToBuffer(in_data); }

    void UpdateBasicShadowCaster(const BasicShadowCasterConstant& in_data) const { m_shadowCaster->WriteToBuffer(in_data); }
    void UpdateOmniShadowCaster(const OmniShadowCasterConstant& in_data) const { m_omniShadowCaster->WriteToBuffer(in_data); }
    void UpdateCascadeShadowCaster(const CascadeShadowCasterConstant& in_data) const { m_cascadeShadowCaster->WriteToBuffer(in_data); }

    //===================================================
    // Getter
    //===================================================

    Ref<ConstantBuffer<TransformConstant>>      GetTransform() const { return m_transform; }
    Ref<ConstantBuffer<CameraConstant>>         GetCamera() const { return m_camera; }
    Ref<ConstantBuffer<CameraConstant>>         GetReflectCamera() const { return m_reflect; }

    Ref<ConstantBuffer<LightConstant>>          GetLight() const { return m_light; }
    Ref<ConstantBuffer<SkyboxConstant>>         GetSkybox() const { return m_skybox; }
    Ref<ConstantBuffer<ShadowReceiverConstant>> GetShadowReceiver() const { return m_shadow; }

    Ref<ConstantBuffer<MaterialConstant>> GetMaterial() const { return m_material; }

    Ref<ConstantBuffer<BasicShadowCasterConstant>>   GetShadowCaster() const { return m_shadowCaster; }
    Ref<ConstantBuffer<OmniShadowCasterConstant>>    GetOmniShadowCaster() const { return m_omniShadowCaster; }
    Ref<ConstantBuffer<CascadeShadowCasterConstant>> GetCascadeShadowCaster() const { return m_cascadeShadowCaster; }

private:
    Ref<ConstantBuffer<TransformConstant>>      m_transform;
    Ref<ConstantBuffer<CameraConstant>>         m_camera;
    Ref<ConstantBuffer<CameraConstant>>         m_reflect;
    Ref<ConstantBuffer<LightConstant>>          m_light;
    Ref<ConstantBuffer<SkyboxConstant>>         m_skybox;
    Ref<ConstantBuffer<ShadowReceiverConstant>> m_shadow;

    Ref<ConstantBuffer<MaterialConstant>> m_material;

    Ref<ConstantBuffer<BasicShadowCasterConstant>>   m_shadowCaster;
    Ref<ConstantBuffer<OmniShadowCasterConstant>>    m_omniShadowCaster;
    Ref<ConstantBuffer<CascadeShadowCasterConstant>> m_cascadeShadowCaster;
};

}   // namespace crab