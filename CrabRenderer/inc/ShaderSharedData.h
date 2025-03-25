
//===================================================
// hlsl ~ application shared constants
//===================================================

#ifndef CONSTANT_COMMON_H
#define CONSTANT_COMMON_H

#ifdef __cplusplus

namespace crab
{

using MATRIX = Mat4;
using float2 = Vec2;
using float3 = Vec3;
using float4 = Vec4;
using uint   = uint32;

}   // namespace crab

#    define SEMANTIC(x)
#    define CBUFFER(StructName, SlotNumber)                            \
        constexpr inline static uint32 StructName##_SLOT = SlotNumber; \
        struct alignas(16) StructName

namespace crab
{

#else

#    define MATRIX row_major float4x4
#    define SEMANTIC(x) : x
#    define CBUFFER(StructName, SlotNumber) \
        cbuffer StructName : register(b##SlotNumber)

#endif

//===================================================
// Definitions
//===================================================

#define LIGHT_COUNT                4
#define CASCADE_SHADOW_SPLIT_LEVEL 4

#define BIT(x) (1 << x)

#define MATERIAL_TEXTURE_FLAGS              uint
#define MATERIAL_TEXTURE_FLAGS_NONE         0
#define MATERIAL_TEXTURE_FLAGS_ALBEDO       BIT(0)
#define MATERIAL_TEXTURE_FLAGS_NORMAL_GL    BIT(1)
#define MATERIAL_TEXTURE_FLAGS_NORMAL_DX    BIT(2)
#define MATERIAL_TEXTURE_FLAGS_METALLIC     BIT(3)
#define MATERIAL_TEXTURE_FLAGS_ROUGHNESS    BIT(4)
#define MATERIAL_TEXTURE_FLAGS_AO           BIT(5)
#define MATERIAL_TEXTURE_FLAGS_EMISSIVE     BIT(6)
#define MATERIAL_TEXTURE_FLAGS_DISPLACEMENT BIT(7)

#define SKYBOX_TEXTURE_TYPE             uint
#define SKYBOX_TEXTURE_TYPE_ENVIRONMENT 0
#define SKYBOX_TEXTURE_TYPE_DIFFUSE     1
#define SKYBOX_TEXTURE_TYPE_SPECULAR    2

#define LIGHT_TYPE             uint
#define LIGHT_TYPE_NONE        0
#define LIGHT_TYPE_DIRECTIONAL 1
#define LIGHT_TYPE_POINT       2
#define LIGHT_TYPE_SPOT        3

//===================================================
// Constant Buffers
//===================================================

CBUFFER(CB_TRANSFORM, 0)
{
    MATRIX cb_transformWorld;
    MATRIX cb_transformWorldInvTranspose;
};

CBUFFER(CB_CAMERA, 1)
{
    MATRIX cb_cameraView;   // 64
    // --------------------------
    MATRIX cb_cameraViewProj;   // 64
    // --------------------------
    MATRIX cb_cameraInvViewProj;   // 64
    // --------------------------
    float3 cb_cameraPosition;   // 12
    float  cb_camera_pad;       // 4
};

CBUFFER(CB_SKYBOX, 2)
{
    SKYBOX_TEXTURE_TYPE cb_skyboxTextureCubeType;
};

struct LightSubdata
{
    float3     lightPosition;   // 12
    LIGHT_TYPE lightType;       // 4
    // --------------------------
    float3 lightDirection;        // 12
    int    shadowMappingEnable;   // 4
    // --------------------------
    float3 lightRadiance;   // 12
    float  fallOffStart;    // 4
    // --------------------------
    float fallOffEnd;            // 4
    float lightStrength;         // 4
    float lightInnerConeAngle;   // 4
    float lightOuterConeAngle;   // 4
};

CBUFFER(CB_LIGHT, 2)
{
    LightSubdata cb_lights[LIGHT_COUNT];
};

CBUFFER(CB_MATERIAL, 3)
{
    float3 cb_materialAlbedo;     // 12
    float  cb_materialMetallic;   // 4
    // ----------------------
    float3 cb_materialEmissive;    // 12
    float  cb_materialRoughness;   // 4
    // ----------------------
    float3 cb_materialDiffuse;                // 12
    float  cb_materialDisplacementStrength;   // 4
    // ----------------------
    float3 cb_materialSpecular;    // 12
    float  cb_materialSharpness;   // 4
    // ----------------------
    float3                 cb_materialAmbient;        // 12
    MATERIAL_TEXTURE_FLAGS cb_materialTextureFlags;   // 4
    // ----------------------
    float  cb_materialAlpha;   // 4
    float3 cb_material_pad;    // 12
};

//===================================================
// Post Process
//===================================================

CBUFFER(CB_TONE_MAPPING, 0)
{
    float cb_toneMappingExposure;
    float cb_toneMappingGamma;
};

CBUFFER(CB_BLUR_DOWN, 0)
{
    float cb_blurDownDeltaX;
    float cb_blurDownDeltaY;
};

CBUFFER(CB_BLUR_UP, 0)
{
    float cb_blurUpRadius;
};

CBUFFER(CB_COMBINE, 0)
{
    float cb_combineStrength;
};

//===================================================
// Shadow
//===================================================

CBUFFER(CB_SHADOW_RECEIVER, 3)
{
    MATRIX cb_shadowReceiverViewProj[LIGHT_COUNT];
    // --------------------------
    MATRIX cb_cascadeShadowReceiverViewProj[CASCADE_SHADOW_SPLIT_LEVEL];
    // --------------------------
    float cb_cascadeRange1;
    float cb_cascadeRange2;
    float cb_cascadeRange3;
    float cb_shadow_receiver_pad;
};

CBUFFER(CB_SHADOW_CASTER, 1)
{
    MATRIX cb_shadowCasterViewProj;
};

CBUFFER(CB_CASCADE_SHADOW_CASTER, 1)
{
    MATRIX cb_cascadeShadowCastViewProj[CASCADE_SHADOW_SPLIT_LEVEL];
};

CBUFFER(CB_OMNI_SHADOW_CASTER, 1)
{
    MATRIX cb_omniShadowCastViewProj[6];
    float3 cb_omniShadowLightPosition;
    float  cb_omniShadowLightRange;
};

//===================================================
// Debug
//===================================================

CBUFFER(CB_DEBUG_NORMAL, 2)
{
    float  cb_debugNormalLineStrength;
    float3 cb_cb_debug_normal_pad;
};

//===================================================
// Vertex
//===================================================

struct VS_3D_INPUT
{
    float3 positionL SEMANTIC(POSITION);
    float3 normalL   SEMANTIC(NORMAL);
    float2 texCoord  SEMANTIC(TEXCOORD);
    float3 tangentL  SEMANTIC(TANGENT);
};

struct VS_2D_INPUT
{
    float2 positionL SEMANTIC(POSITION);
    float2 texCoord  SEMANTIC(TEXCOORD);
};

#ifdef __cplusplus
}
#endif

#endif   // CONSTANT_COMMON_H