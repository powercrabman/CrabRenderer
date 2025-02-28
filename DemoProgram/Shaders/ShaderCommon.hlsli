#define MATRIX row_major float4x4
#define BIT(x) (1 << x)

struct VS_Input
{
    float3 posL : POSITION;
    float3 normalL : NORMAL;
    float2 texCoord : TEXCOORD;
    float3 tangentL : TANGENT;
};

struct PS_Input
{
    float4 posH : SV_POSITION;
    float3 posW : POSITION;
    float3 normalW : NORMAL;
    float2 texCoord : TEXCOORD;
    float3 tangentW : TANGENT;
    float3 bitangentW : BITANGENT;
};

//===================================================
// Constant Data
//===================================================

cbuffer TransformConstant : register(b0)
{
    MATRIX g_world;
    MATRIX g_worldInvTranspose;
};

cbuffer CameraConstant : register(b1)
{
    MATRIX g_viewProj;
    //-------------------------------------
    MATRIX g_viewProjInv;
    //-------------------------------------
    float3 g_eyePosW;
};

//===================================================
// Light Constant
//===================================================

static const int MAX_LIGHTS = 3;

#define LIGHT_NONE 0
#define LIGHT_DIRECTIONAL 1
#define LIGHT_POINT 2
#define LIGHT_SPOT 3

#define LIGHT_SHADOW_MAPPING BIT(8)

bool IsShadowMapping(uint in_lightType)
{
    return (in_lightType & LIGHT_SHADOW_MAPPING);
}

bool MatchLightType(uint in_lightType, uint in_lightTypeBit)
{
    uint lightType = in_lightType & 0xFF;
    return (lightType & in_lightTypeBit);
}

struct LightTransform
{
    MATRIX lightViewProj;
    // --------------------------
    float3 lightPosition;
    float pad1;
    // --------------------------
    float3 lightDirection;
    float pad2;
};

struct LightAttribute
{
    float3 lightRadiance;
    float fallOffStart;
    // --------------------------
    float fallOffEnd;
    float spotPower;
    float lightStrength;
    uint lightType; // if 0, light is disabled
};

cbuffer LightTransformConstant : register(b2)
{
    LightTransform g_lightTransform[MAX_LIGHTS];
};

cbuffer LightAttributeConstant : register(b3)
{
    LightAttribute g_lightAttribute[MAX_LIGHTS];
};

//===================================================
// Material Constant 
//===================================================

#define USING_TEXTURE_BASECOLOR            (BIT(0))
#define USING_TEXTURE_NORMAL_GL            (BIT(1))
#define USING_TEXTURE_NORMAL_DX            (BIT(2))
#define USING_TEXTURE_METALLIC             (BIT(3))
#define USING_TEXTURE_ROUGHNESS            (BIT(4))
#define USING_TEXTURE_AMBIENT_OCCLUSION    (BIT(5))
#define USING_TEXTURE_EMISSIVE             (BIT(6))
#define USING_TEXTURE_DISPLACEMENT         (BIT(7))

cbuffer MaterialConstant : register(b4)
{
    float3 g_baseColor; // 12
    float g_metallic; // 4
    // ----------------------
    float3 g_emissive; // 12
    float g_roughness; // 4
    // ----------------------
    float3 g_diffuse; // 12
    float g_displacementStrength; // 4
    // ----------------------
    float3 g_specular; // 12
    float g_sharpness; // 4
    // ----------------------
    float3 g_ambient; // 12
    uint g_usingTextureFlags; // 4
    // ----------------------
    float g_alpha; // 4
    float3 g_pad;
};

//===================================================
// Common Resource
//===================================================

TextureCube g_irradiateCube : register(t11);
TextureCube g_specularCube : register(t12);
Texture2D g_brdfTex : register(t13);

SamplerState g_wrapSampler : register(s0);
SamplerState g_clampSampler : register(s1);
SamplerComparisonState g_shadowSampler : register(s2);

//===================================================
// Common Function
//===================================================

// far: close to 0, near: close to 1
float ComputeAttenuation(float in_distance, float in_fallOffStart, float in_fallOffEnd)
{
    float att = 1.f - saturate((in_distance - in_fallOffStart) / (in_fallOffEnd - in_fallOffStart));
    att = smoothstep(0.f, 1.f, att);
    return att;
}

static const float2 g_poissonDisk[16] =
{
    float2(-0.94201624, -0.39906216), float2(0.94558609, -0.76890725),
    float2(-0.094184101, -0.92938870), float2(0.34495938, 0.29387760),
    float2(-0.91588581, 0.45771432), float2(-0.81544232, -0.87912464),
    float2(-0.38277543, 0.27676845), float2(0.97484398, 0.75648379),
    float2(0.44323325, -0.97511554), float2(0.53742981, -0.47373420),
    float2(-0.26496911, -0.41893023), float2(0.79197514, 0.19090188),
    float2(-0.24188840, 0.99706507), float2(-0.81409955, 0.91437590),
    float2(0.19984126, 0.78641367), float2(0.14383161, -0.14100790)
};

static const float2 g_diskSamples64[64] =
{
    float2(0.0, 0.0),
    float2(-0.12499844227275288, 0.000624042775189866), float2(0.1297518688031755, -0.12006020382326336),
    float2(-0.017851253586055427, 0.21576916541852392), float2(-0.1530983013115895, -0.19763833164521946),
    float2(0.27547541035593626, 0.0473106572479027), float2(-0.257522587854559, 0.16562643733622642),
    float2(0.0842605283808073, -0.3198048832600703), float2(0.1645196099088727, 0.3129429627830483),
    float2(-0.3528833088400373, -0.12687935349026194), float2(0.36462214742013344, -0.1526456341030772),
    float2(-0.17384046457324884, 0.37637015407303087), float2(-0.1316547617859344, -0.4125130588224921),
    float2(0.3910687393754993, 0.2240317858770442), float2(-0.45629121277761536, 0.10270505898899496),
    float2(0.27645268679640483, -0.3974278701387824), float2(0.06673001731984558, 0.49552709793561556),
    float2(-0.39574431915605623, -0.33016879600548193), float2(0.5297612167716342, -0.024557141621887494),
    float2(-0.3842909284448636, 0.3862583103507092), float2(0.0230336562454131, -0.5585422550532486),
    float2(0.36920334463249477, 0.43796562686149154), float2(-0.5814490172413539, -0.07527974727019048),
    float2(0.4903718680780365, -0.3448339179919178), float2(-0.13142003698572613, 0.5981043168868373),
    float2(-0.31344141845114937, -0.540721256470773), float2(0.608184438565748, 0.19068741092811003),
    float2(-0.5882602609696388, 0.27536315179038107), float2(0.25230610046544444, -0.6114259003901626),
    float2(0.23098706800827415, 0.6322736546883326), float2(-0.6076303951666067, -0.31549215975943595),
    float2(0.6720886334230931, -0.1807536135834609), float2(-0.37945598830371974, 0.5966683776943834),
    float2(-0.1251555455510758, -0.7070792667147104), float2(0.5784815570900413, 0.44340623372555477),
    float2(-0.7366710399837763, 0.0647362251696953), float2(0.50655463562529, -0.553084443034271),
    float2(8.672987356252326e-05, 0.760345311340794), float2(-0.5205650355786364, -0.5681215043747359),
    float2(0.7776435491294021, 0.06815798190547596), float2(-0.6273416101921778, 0.48108471615868836),
    float2(0.1393236805531513, -0.7881712453757264), float2(0.4348773806743975, 0.6834703093608201),
    float2(-0.7916014213464706, -0.21270211499241704), float2(0.7357897682897174, -0.38224784745000717),
    float2(-0.2875567908732709, 0.7876776574352392), float2(-0.3235695699691864, -0.7836151691933712),
    float2(0.7762165924462436, 0.3631291803355136), float2(-0.8263007976064866, 0.2592816844184794),
    float2(0.4386452756167397, -0.7571098481588484), float2(0.18988542402304126, 0.8632459242554175),
    float2(-0.7303253445407815, -0.5133224046555819), float2(0.8939004035324556, -0.11593993515830946),
    float2(-0.5863762307291154, 0.6959079795748251), float2(-0.03805753378232556, -0.9177699189461416),
    float2(0.653979655650389, 0.657027860897389), float2(-0.9344208130797295, -0.04310155546401203),
    float2(0.7245109901504777, -0.6047386420191574), float2(-0.12683493131695708, 0.9434844461875473),
    float2(-0.5484582700240663, -0.7880790100251422), float2(0.9446610338564589, 0.2124041692463835),
    float2(-0.8470120123194587, 0.48548496473788055), float2(0.29904134279525085, -0.9377229203230629),
    float2(0.41623562331748715, 0.9006236205438447),
};
