#pragma once

namespace crab
{
class Texture2D;

enum class eNormalMapType
{
    OpenGL,
    DirectX
};

struct MaterialData
{
    Vec3  baseColor            = Vec3(1.f, 1.f, 1.f);
    Vec3  emissive             = Vec3(0.f, 0.f, 0.f);
    float metallic             = 1.f;
    float roughness            = 1.f;
    float displacementStrength = 1.f;
    Vec3  diffuse              = Vec3(1.f, 1.f, 1.f);   // Phong
    Vec3  specular             = Vec3(1.f, 1.f, 1.f);   // Phong
    Vec3  ambient              = Vec3(1.f, 1.f, 1.f);   // Phong
    float shininess            = 32.f;                  // Phong
    float alpha                = 1.f;

    Ref<Texture2D> baseColorTex    = nullptr;
    Ref<Texture2D> normalTex       = nullptr;
    Ref<Texture2D> metallicTex     = nullptr;
    Ref<Texture2D> roughnessTex    = nullptr;
    Ref<Texture2D> aoTex           = nullptr;
    Ref<Texture2D> emissiveTex     = nullptr;
    Ref<Texture2D> displacementTex = nullptr;

    eNormalMapType normalMapType = eNormalMapType::OpenGL;
};

class Material
{
public:
    void Init(const MaterialData& in_data);

    // color
    void SetBaseColor(const Vec3& in_color) { m_data.baseColor = in_color; }
    void SetEmissive(const Vec3& in_color) { m_data.emissive = in_color; }
    void SetMetallic(float in_value) { m_data.metallic = in_value; }
    void SetRoughness(float in_value) { m_data.roughness = in_value; }
    void SetDiffuse(const Vec3& in_color) { m_data.diffuse = in_color; }
    void SetSpecular(const Vec3& in_color) { m_data.specular = in_color; }
    void SetAmbient(const Vec3& in_color) { m_data.ambient = in_color; }
    void SetSharpness(float in_value) { m_data.shininess = in_value; }
    void SetDisplacementStrength(float in_value) { m_data.displacementStrength = in_value; }
    void SetAlpha(float in_value) { m_data.alpha = in_value; }
    void SetNormalMapType(eNormalMapType in_type) { m_data.normalMapType = in_type; }

    // image
    void SetBaseColorTexture(const Ref<Texture2D>& in_image) { m_data.baseColorTex = in_image; }
    void SetNormalTexture(const Ref<Texture2D>& in_image) { m_data.normalTex = in_image; }
    void SetMetallicTexture(const Ref<Texture2D>& in_image) { m_data.metallicTex = in_image; }
    void SetRoughnessTexture(const Ref<Texture2D>& in_image) { m_data.roughnessTex = in_image; }
    void SetAOTexture(const Ref<Texture2D>& in_image) { m_data.aoTex = in_image; }
    void SetEmissiveTexture(const Ref<Texture2D>& in_image) { m_data.emissiveTex = in_image; }
    void SetDisplacementTexture(const Ref<Texture2D>& in_image) { m_data.displacementTex = in_image; }

    // getter
    const MaterialData& GetMaterialData() const { return m_data; }
    const Vec3&         GetBaseColor() const { return m_data.baseColor; }
    const Vec3&         GetEmissive() const { return m_data.emissive; }
    float               GetMetallic() const { return m_data.metallic; }
    float               GetRoughness() const { return m_data.roughness; }
    const Vec3&         GetDiffuse() const { return m_data.diffuse; }
    const Vec3&         GetSpecular() const { return m_data.specular; }
    const Vec3&         GetAmbient() const { return m_data.ambient; }
    float               GetSharpness() const { return m_data.shininess; }
    float               GetDisplacementStrength() const { return m_data.displacementStrength; }
    float               GetAlpha() const { return m_data.alpha; }
    eNormalMapType      GetNormalMapType() const { return m_data.normalMapType; }

    const Ref<Texture2D>& GetBaseColorTexture() const { return m_data.baseColorTex; }
    const Ref<Texture2D>& GetNormalTexture() const { return m_data.normalTex; }
    const Ref<Texture2D>& GetMetallicTexture() const { return m_data.metallicTex; }
    const Ref<Texture2D>& GetRoughnessTexture() const { return m_data.roughnessTex; }
    const Ref<Texture2D>& GetAOTexture() const { return m_data.aoTex; }
    const Ref<Texture2D>& GetEmissiveTexture() const { return m_data.emissiveTex; }
    const Ref<Texture2D>& GetDisplacementTexture() const { return m_data.displacementTex; }

private:
    MaterialData m_data;
};

}   // namespace crab
