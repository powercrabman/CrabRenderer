#pragma once

// maybe MeshRenderer and ModelRenderer can be merged
struct ModelRenderer
{
    Ref<Model> model;
};

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

struct LightComponent
{
    // About Light
    Color3 lightRadiance = color3::WHITE;

    float fallOffStart   = 5.f;
    float fallOffEnd     = 100.f;
    float lightStrength  = 1.f;
    float innerConeAngle = 15.f * DEG2RAD;
    float outerConeAngle = 30.f * DEG2RAD;

    eLightType lightType = eLightType::None;

    // About Shadow
    Ref<DepthMap>    shadowMap;
    float            shadowBias       = 0.001f;
    uint32           shadowKernelSize = 5;
    bool             useShadow        = false;
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
