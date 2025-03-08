#pragma once

class GlobalConstants : public Singleton<GlobalConstants>
{
    friend class Singleton<GlobalConstants>;

public:
    void Init();

    void UpdateTransform(const TransformConstant& in_data) const;
    void UpdateCamera(const CameraConstant& in_data) const;
    void UpdateReflectCamera(const CameraConstant& in_data) const;
    void UpdateDrawNormalFactor(const DrawNormalConstant& in_data) const;
    void UpdateSkyboxPS(const SkyboxConstant& in_data) const;
    void UpdateShaderCamera(const CameraConstant& in_data) const;
    void UpdateMaterial(const MaterialConstant& in_data) const;
    void UpdateLight(const LightConstant& in_data) const;
    void UpdateCascadeShadow(const CascadeShadowConstant& in_data) const;
    void UpdateDepthVisualize(const DepthVisualizeConstant& in_data) const;
    void UpdateBasicShadowCaster(const BasicShadowCasterConstant& in_data) const;
    void UpdateCascadeShadowCaster(const CascadeShadowCasterConstant& in_data) const;
    void UpdateOmniShadowCaster(const OmniShadowCasterConstant& in_data) const;

    auto GetTransformConstant() const { return m_transformConstant; }
    auto GetCameraConstant() const { return m_cameraConstant; }
    auto GetReflectCameraConstant() const { return m_mirrorCameraConstant; }
    auto GetDrawNormalFactor() const { return m_drawNormalGSConstant; }
    auto GetSkyboxPSConstant() const { return m_skyboxPSConstant; }
    auto GetShadowCameraConstant() const { return m_shadowCameraConstant; }
    auto GetMaterialConstant() const { return m_materialConstant; }
    auto GetLightConstant() const { return m_lightConstant; }
    auto GetCascadeShadowConstant() const { return m_cascadeShadowConstant; }
    auto GetDepthVisualizeConstant() const { return m_depthVisualizeConstant; }
    auto GetBasicShadowCasterConstant() const { return m_basicShaderCasterConstant; }
    auto GetCascadeShadowCasterConstant() const { return m_cascadeShadowCasterConstant; }
    auto GetOmniShadowCasterConstant() const { return m_omniShadowCasterConstant; }

private:
    Ref<ConstantBuffer<TransformConstant>> m_transformConstant = nullptr;
    Ref<ConstantBuffer<MaterialConstant>>  m_materialConstant  = nullptr;

    Ref<ConstantBuffer<CameraConstant>> m_cameraConstant       = nullptr;
    Ref<ConstantBuffer<CameraConstant>> m_mirrorCameraConstant = nullptr;
    Ref<ConstantBuffer<CameraConstant>> m_shadowCameraConstant = nullptr;

    Ref<ConstantBuffer<DrawNormalConstant>> m_drawNormalGSConstant = nullptr;

    Ref<ConstantBuffer<SkyboxConstant>> m_skyboxPSConstant = nullptr;

    Ref<ConstantBuffer<LightConstant>>         m_lightConstant         = nullptr;
    Ref<ConstantBuffer<CascadeShadowConstant>> m_cascadeShadowConstant = nullptr;

    Ref<ConstantBuffer<DepthVisualizeConstant>> m_depthVisualizeConstant = nullptr;

    Ref<ConstantBuffer<BasicShadowCasterConstant>>   m_basicShaderCasterConstant   = nullptr;
    Ref<ConstantBuffer<CascadeShadowCasterConstant>> m_cascadeShadowCasterConstant = nullptr;
    Ref<ConstantBuffer<OmniShadowCasterConstant>>    m_omniShadowCasterConstant    = nullptr;
};



inline GlobalConstants& GetGlobalConstants()
{
    return GlobalConstants::GetInstance();
}
