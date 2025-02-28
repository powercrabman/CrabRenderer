#pragma once

class GlobalConstants : public Singleton<GlobalConstants>
{
    friend class Singleton<GlobalConstants>;

public:
    void Init();

    void UpdateTransform(const TransformConstant& in_data) const;
    void UpdateCamera(const CameraConstant& in_data) const;
    void UpdateReflectCamera(const CameraConstant& in_data) const;
    void UpdateDrawNormalFactor(const DrawNormalFactor& in_data) const;
    void UpdateSkyboxPS(const SkyboxPSConstant& in_data) const;
    void UpdateShaderCamera(const CameraConstant& in_data) const;
    void UpdateMaterial(const MaterialConstant& in_data) const;
    void UpdateLightAttribute(const LightAttributeConstant& in_data) const;
    void UpdateLightTransform(const LightTransformConstant& in_data) const;

    auto GetTransformConstant() const { return m_transformConstant; }
    auto GetCameraConstant() const { return m_cameraConstant; }
    auto GetReflectCameraConstant() const { return m_mirrorCameraConstant; }
    auto GetDrawNormalFactor() const { return m_drawNormalGSConstant; }
    auto GetSkyboxPSConstant() const { return m_skyboxPSConstant; }
    auto GetShadowCameraConstant() const { return m_shadowCameraConstant; }
    auto GetMaterialConstant() const { return m_materialConstant; }
    auto GetLightAttributeConstant() const { return m_lightAttributeConstant; }
    auto GetLightTransformConstant() const { return m_lightTransformConstant; }

    
private:
    Ref<ConstantBuffer<TransformConstant>> m_transformConstant = nullptr;
    Ref<ConstantBuffer<MaterialConstant>>  m_materialConstant  = nullptr;

    Ref<ConstantBuffer<CameraConstant>> m_cameraConstant       = nullptr;
    Ref<ConstantBuffer<CameraConstant>> m_mirrorCameraConstant = nullptr;
    Ref<ConstantBuffer<CameraConstant>> m_shadowCameraConstant = nullptr;

    Ref<ConstantBuffer<DrawNormalFactor>> m_drawNormalGSConstant = nullptr;

    Ref<ConstantBuffer<SkyboxPSConstant>> m_skyboxPSConstant = nullptr;

    Ref<ConstantBuffer<LightAttributeConstant>> m_lightAttributeConstant = {};
    Ref<ConstantBuffer<LightTransformConstant>> m_lightTransformConstant = {};
};

inline GlobalConstants& GetGlobalConstants()
{
    return GlobalConstants::GetInstance();
}

