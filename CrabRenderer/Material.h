#pragma once

namespace crab
{
class Image2D;

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

    Ref<Image2D> baseColorImage    = nullptr;
    Ref<Image2D> normalImage       = nullptr;
    Ref<Image2D> metallicImage     = nullptr;
    Ref<Image2D> roughnessImage    = nullptr;
    Ref<Image2D> aoImage           = nullptr;
    Ref<Image2D> emissiveImage     = nullptr;
    Ref<Image2D> displacementImage = nullptr;

    eNormalMapType normalMapType = eNormalMapType::OpenGL;
};

class Material
{
    D11_RESOURCE_CTOR(Material);

public:
    static Ref<Material> Create(const MaterialData& in_materialData);

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
    void SetBaseColorImage(const Ref<Image2D>& in_image) { m_data.baseColorImage = in_image; }
    void SetNormalImage(const Ref<Image2D>& in_image) { m_data.normalImage = in_image; }
    void SetMetallicImage(const Ref<Image2D>& in_image) { m_data.metallicImage = in_image; }
    void SetRoughnessImage(const Ref<Image2D>& in_image) { m_data.roughnessImage = in_image; }
    void SetAOImage(const Ref<Image2D>& in_image) { m_data.aoImage = in_image; }
    void SetEmissiveImage(const Ref<Image2D>& in_image) { m_data.emissiveImage = in_image; }
    void SetHeightImage(const Ref<Image2D>& in_image) { m_data.displacementImage = in_image; }

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

    const Ref<Image2D>& GetBaseColorImage() const { return m_data.baseColorImage; }
    const Ref<Image2D>& GetNormalImage() const { return m_data.normalImage; }
    const Ref<Image2D>& GetMetallicImage() const { return m_data.metallicImage; }
    const Ref<Image2D>& GetRoughnessImage() const { return m_data.roughnessImage; }
    const Ref<Image2D>& GetAOImage() const { return m_data.aoImage; }
    const Ref<Image2D>& GetEmissiveImage() const { return m_data.emissiveImage; }
    const Ref<Image2D>& GetHeightImage() const { return m_data.displacementImage; }

private:
    MaterialData m_data;
};

}   // namespace crab
