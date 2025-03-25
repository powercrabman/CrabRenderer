#pragma once

namespace crab
{
// ID
struct IDComponent
{
    uint32 id;
};

// Name
struct TagComponent
{
    bool operator==(const TagComponent& in_other) const { return name == in_other.name; }
    bool operator==(const std::string& in_other) const { return name == in_other; }
    bool operator==(const std::string_view in_other) const { return name == in_other; }

    bool operator!=(const TagComponent& in_other) const { return name != in_other.name; }
    bool operator!=(const std::string& in_other) const { return name != in_other; }
    bool operator!=(const std::string_view in_other) const { return name != in_other; }

    std::string name;
};

// TransformComponent
struct TransformComponent
{
    Mat4 GetWorld() const;
    Mat4 GetWorldInvTranspose() const;

    Vec3 Forward() const;
    Vec3 Backward() const;
    Vec3 Right() const;
    Vec3 Left() const;
    Vec3 Up() const;
    Vec3 Down() const;

    void AddRotatePitch(float in_degree);
    void AddRotateYaw(float in_degree);
    void AddRotateRoll(float in_degree);

    void SetRotatePitch(float in_degree);
    void SetRotateYaw(float in_degree);
    void SetRotateRoll(float in_degree);

    Vec3 position = Vec3::Zero;
    Quat rotate   = Quat::Identity;
    Vec3 scale    = Vec3::One;
};

// Camera

struct CameraComponent
{
    static Mat4 GetView(const TransformComponent& in_trans);
    Mat4        GetProj() const;

    Mat4 GetViewProj(const Vec3& in_eyePosition, const Quat& in_quaternion) const;
    Mat4 GetViewProj(const Vec3& in_eyePosition, const Vec3& in_pitchYawRoll) const;
    Mat4 GetViewProj(const TransformComponent& in_trans) const;

    Frustum GetViewFrustumInView() const;
    Frustum GetViewFrustumInWorld(const Vec3& in_position, const Quat& in_quaternion) const;
    Frustum GetViewFrustumInWorld(const Vec3& in_position, const Vec3& in_pitchYawRoll) const;
    Frustum GetViewFrustumInWorld(const TransformComponent& in_trans) const;

    eProjectionType projectionType = eProjectionType::Perspective;
    float           nearZ          = 0.1f;
    float           farZ           = 1000.0f;
    float           fov            = 45.f * DEG2RAD;

    // perspective
    float aspect = 1.f;

    // orthographic
    float width  = 1.f;
    float height = 1.f;
};

class Script;

struct ScriptComponent
{
    Scope<Script> script;
};

template<StringLiteral Tag>
struct RenderGroup
{
};

class Model;

struct ModelRenderer
{
    Ref<Model> model;
};

class Mesh;
class TextureCube;

struct SkyboxRenderer
{
    Ref<Mesh>        mesh;
    Ref<TextureCube> envCubemap;
    Ref<TextureCube> irrCubemap;
    Ref<TextureCube> specCubemap;
    Ref<Texture2D>   brdfImage;

    enum eMappingType
    {
        Environment = 0,
        Irradiance,
        Specular,
    };

    eMappingType mappingType = Environment;
};

class DepthMap;

struct LightComponent
{
    // About LightSubdata
    Color3 lightRadiance = color3::WHITE;

    float fallOffStart   = 5.f;
    float fallOffEnd     = 100.f;
    float lightStrength  = 1.f;
    float innerConeAngle = 15.f * DEG2RAD;
    float outerConeAngle = 30.f * DEG2RAD;

    eLightType lightType = eLightType::None;

    // shadow
    bool          useShadow = false;
    Ref<DepthMap> shadowMap;

    // in cascade shadow
    std::array<float, CASCADE_SHADOW_SPLIT_LEVEL - 1> cascadeRange = { 50.f, 150.f, 300.f };
};

}   // namespace crab