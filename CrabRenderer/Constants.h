#pragma once
#include "CrabEnums.h"

namespace crab
{

cbuffer CameraConstant
{
    Mat4 view;
    Mat4 viewProj;
    Mat4 invViewProj;
    Vec3 cameraPosition;
};

cbuffer TransformConstant
{
    Mat4 world;
    Mat4 worldInvTranspose;
};

cbuffer SkyboxConstant
{
    int textureCubeType;
};

//===================================================
// LightConstantData & Shadow
//===================================================

struct LightConstantData
{
    Vec3       lightPosition;
    eLightType lightType;
    // --------------------------
    Vec3   lightDirection;
    uint32 useShadow;
    // --------------------------
    Vec3  lightRadiance;
    float fallOffStart;
    // --------------------------
    float fallOffEnd;
    float lightStrength;
    float innerConeAngle;   // spotlight
    float outerConeAngle;   // spotlight
};

cbuffer LightConstant
{
    LightConstantData light[MAX_LIGHTS] = {};
};

cbuffer ShadowReceiverConstant
{
    Mat4 basicShadowViewProj[MAX_LIGHTS];
    // --------------------------
    Mat4 cascadeShadowViewProj[MAX_CASCADE_SHADOW_LEVEL];
    // --------------------------
    float cascadeRange1;
    float cascadeRange2;
    float cascadeRange3;
    float pad;
};

//===================================================
// Material
//===================================================

using eMaterialTextureUsingFlags = uint32;

enum eMaterialTextureUsingFlags_ : uint32
{
    eMaterialTextureUsingFlags_None         = 0,
    eMaterialTextureUsingFlags_BaseColor    = BIT(0),
    eMaterialTextureUsingFlags_Normal_GL    = BIT(1),
    eMaterialTextureUsingFlags_Normal_DX    = BIT(2),
    eMaterialTextureUsingFlags_Metallic     = BIT(3),
    eMaterialTextureUsingFlags_Roughness    = BIT(4),
    eMaterialTextureUsingFlags_AO           = BIT(5),
    eMaterialTextureUsingFlags_Emissive     = BIT(6),
    eMaterialTextureUsingFlags_Displacement = BIT(7),
};

cbuffer MaterialConstant
{
    Vec3  baseColor;   // 12
    float metallic;    // 4
    // ----------------------
    Vec3  emissive;    // 12
    float roughness;   // 4
    // ----------------------
    Vec3  diffuse;                // 12
    float displacementStrength;   // 4
    // ----------------------
    Vec3  specular;    // 12
    float sharpness;   // 4
    // ----------------------
    Vec3                       ambient;             // 12
    eMaterialTextureUsingFlags usingTextureFlags;   // 4
    // ----------------------
    float alpha;   // 4
    Vec3  pad;     // 12
};

//===================================================
// Post Process Constant
//===================================================

cbuffer ToneMappingConstant
{
    float exposure;
    float gamma;
};

cbuffer BlurDownConstant
{
    float dx;
    float dy;
};

cbuffer BlurUpConstant
{
    float radius;
};

cbuffer CombineConstant
{
    float combineStrength;
};

cbuffer PostEffectConstant
{
    //--------------------------------------
    Vec3  fogColor    = Vec3::One;
    float fogStrength = 1.f;
    //--------------------------------------
    float fogFallOffStart = 10.f;
    float fogFallOffEnd   = 50.f;
};

//===================================================
// Shadow Caster
//===================================================

cbuffer BasicShadowCasterConstant
{
    Mat4 shadowViewProj;
};

cbuffer CascadeShadowCasterConstant
{
    Mat4 shadowViewProj[MAX_CASCADE_SHADOW_LEVEL];
};

cbuffer OmniShadowCasterConstant
{
    Mat4  shadowViewProj[6];
    Vec3  lightPosition;
    float fallOffEnd;
};

}   // namespace crab