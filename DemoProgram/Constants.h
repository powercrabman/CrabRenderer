#pragma once

#include "CrabTypes.h"

cbuffer CameraConstant
{
    Mat4 viewProj;
    Mat4 viewProjInv;
    Vec3 eyePosition;
};

cbuffer DrawNormalFactor
{
    float normalStrength;
};

struct LightTransform
{
    Mat4 lightViewProj;
    // --------------------------
    Vec3  lightPosition;
    float pad1;
    // --------------------------
    Vec3  lightDirection;
    float pad2;
};

struct LightAttribute
{
    Vec3  lightRadiance;
    float fallOffStart;
    // --------------------------
    float      fallOffEnd;
    float      spotPower;
    float      lightStrength;
    eLightType lightType;
};

cbuffer LightTransformConstant
{
    LightTransform lightTransform[MAX_LIGHTS];
};

cbuffer LightAttributeConstant
{
    LightAttribute lightAttribute[MAX_LIGHTS];
};

cbuffer TransformConstant
{
    Mat4 world;
    Mat4 worldInvTranspose;
};

cbuffer SkyboxPSConstant
{
    int textureCubeType;
};

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

cbuffer ToneMapConstant
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
