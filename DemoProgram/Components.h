#pragma once

// maybe MeshRenderer and ModelRenderer can be merged
struct ModelRenderer
{
    Ref<Model> model;
};

struct SkyboxRenderer
{
    Ref<Mesh>         mesh;
    Ref<CubemapImage> envCubemap;
    Ref<CubemapImage> irrCubemap;
    Ref<CubemapImage> specCubemap;
    Ref<Image2D>      brdfImage;

    enum eMappingType
    {
        Environment = 0,
        Irradiance,
        Specular,
    };

    eMappingType mappingType = Environment;
};

struct LightComponent
{
    Vec3 lightRadiance = Vec3::One;

    float fallOffStart  = 5.f;
    float fallOffEnd    = 100.f;
    float spotPower     = 1.f;
    float lightStrength = 1.f;

    eLightType type = eLightType::None;
};

struct CameraControlComponent
{
    float moveSpeed = 1.f;
    float rotSpeed  = 15.f * DEG2RAD;
};

struct PlanarMirrorComponent
{
    Ref<Mesh>     mirrorMesh;   // this is a plane (quad)
    Ref<Material> mirrorMaterial;
};

struct ShadowMapComponent
{
    Ref<DepthBuffer>  shadowDepthBuffer;
    Ref<SamplerState> shadowSampler;
};