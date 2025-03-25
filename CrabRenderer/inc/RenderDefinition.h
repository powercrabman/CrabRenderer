#pragma once

namespace crab
{

class RenderTarget;

constexpr uint32 MSAA_SAMPLE_COUNT = 4;

constexpr uint32 SHADER_SRV_SLOT_COUNT           = D3D11_COMMONSHADER_INPUT_RESOURCE_REGISTER_COUNT;
constexpr uint32 SHADER_SAMPLER_SLOT_COUNT       = D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT;
constexpr uint32 SHADER_CONSTANTS_SLOT_COUNT     = D3D11_COMMONSHADER_CONSTANT_BUFFER_HW_SLOT_COUNT;
constexpr uint32 SHADER_UAV_SLOT_COUNT           = D3D11_PS_CS_UAV_REGISTER_COUNT;
constexpr uint32 SHADER_RENDER_TARGET_SLOT_COUNT = D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT;
constexpr uint32 SKIN_COUNT                      = 8;

using ResourceIDType = uint32;
static_assert(std::is_integral_v<ResourceIDType>, "ResourceIDType must be integral type.");

using FrameBufferID       = ResourceIDType;
using VertexBufferID      = ResourceIDType;
using IndexBufferID       = ResourceIDType;
using InputLayoutID       = ResourceIDType;
using VertexShaderID      = ResourceIDType;
using PixelShaderID       = ResourceIDType;
using GeometryShaderID    = ResourceIDType;
using HullShaderID        = ResourceIDType;
using DomainShaderID      = ResourceIDType;
using RasterizerStateID   = ResourceIDType;
using DepthStencilStateID = ResourceIDType;
using BlendStateID        = ResourceIDType;

enum : uint32
{
    RENDERER_RESOURCE_UNKNOWN_ID = UINT32_MAX_VALUE - 1,
    RENDERER_RESOURCE_NULL_ID    = UINT32_MAX_VALUE
};

using RenderTargetArray      = std::array<Ref<RenderTarget>, SHADER_RENDER_TARGET_SLOT_COUNT>;
using D3D11RenderTargetArray = std::array<ID3D11RenderTargetView*, SHADER_RENDER_TARGET_SLOT_COUNT>;
using BlendFactors           = std::array<float, 4>;

using Microsoft::WRL::ComPtr;

enum class eProjectionType
{
    Orthographic,
    Perspective
};

enum class eNormalMapType
{
    OpenGL,
    DirectX
};

enum class eResourceType
{
    Unknown,
    Buffer,
    Texture2D,
    Texture2DArray,
    TextureCube
};

enum class eComparisonFunc
{
    AlwaysFail = D3D11_COMPARISON_NEVER,
    AlwaysPass = D3D11_COMPARISON_ALWAYS,

    Less      = D3D11_COMPARISON_LESS,
    LessEqual = D3D11_COMPARISON_LESS_EQUAL,

    Equal    = D3D11_COMPARISON_EQUAL,
    NotEqual = D3D11_COMPARISON_NOT_EQUAL,

    Greater      = D3D11_COMPARISON_GREATER,
    GreaterEqual = D3D11_COMPARISON_GREATER_EQUAL,
};

enum class eStencilOperator
{
    Keep = D3D11_STENCIL_OP_KEEP,
    Zero = D3D11_STENCIL_OP_ZERO,

    Replace = D3D11_STENCIL_OP_REPLACE,

    Increase_Saturate = D3D11_STENCIL_OP_INCR_SAT,
    Decrease_Saturate = D3D11_STENCIL_OP_DECR_SAT,

    Increase = D3D11_STENCIL_OP_INCR,
    Decrease = D3D11_STENCIL_OP_DECR,

    Invert = D3D11_STENCIL_OP_INVERT,
};

enum class eCullMode
{
    None  = D3D11_CULL_NONE,
    Front = D3D11_CULL_FRONT,
    Back  = D3D11_CULL_BACK,
};

enum class eFillMode
{
    Solid     = D3D11_FILL_SOLID,
    Wireframe = D3D11_FILL_WIREFRAME,
};

enum class eBlendFactor
{
    Zero             = D3D11_BLEND_ZERO,
    One              = D3D11_BLEND_ONE,
    SrcColor         = D3D11_BLEND_SRC_COLOR,
    InvSrcColor      = D3D11_BLEND_INV_SRC_COLOR,
    SrcAlpha         = D3D11_BLEND_SRC_ALPHA,
    InvSrcAlpha      = D3D11_BLEND_INV_SRC_ALPHA,
    DestAlpha        = D3D11_BLEND_DEST_ALPHA,
    InvDestAlpha     = D3D11_BLEND_INV_DEST_ALPHA,
    DestColor        = D3D11_BLEND_DEST_COLOR,
    InvDestColor     = D3D11_BLEND_INV_DEST_COLOR,
    SrcAlphaSaturate = D3D11_BLEND_SRC_ALPHA_SAT,
    BlendFactor      = D3D11_BLEND_BLEND_FACTOR,
    InvBlendFactor   = D3D11_BLEND_INV_BLEND_FACTOR,
};

enum class eBlendOperator
{
    Add        = D3D11_BLEND_OP_ADD,
    Subtract   = D3D11_BLEND_OP_SUBTRACT,
    ReverseSub = D3D11_BLEND_OP_REV_SUBTRACT,
    Min        = D3D11_BLEND_OP_MIN,
    Max        = D3D11_BLEND_OP_MAX,
};

using eBlendWriteFlags = uint32;

enum eBlendWriteFlags_
{
    eBlendWriteFlags_None  = 0,
    eBlendWriteFlags_Red   = D3D11_COLOR_WRITE_ENABLE_RED,
    eBlendWriteFlags_Green = D3D11_COLOR_WRITE_ENABLE_GREEN,
    eBlendWriteFlags_Blue  = D3D11_COLOR_WRITE_ENABLE_BLUE,
    eBlendWriteFlags_Alpha = D3D11_COLOR_WRITE_ENABLE_ALPHA,
    eBlendWriteFlags_All   = D3D11_COLOR_WRITE_ENABLE_ALL
};

enum class eTopology
{
    None = D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED,

    PointList = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST,

    LineList  = D3D11_PRIMITIVE_TOPOLOGY_LINELIST,
    LineStrip = D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP,

    TriangleList  = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
    TriangleStrip = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP,

    PatchList_3 = D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST,
    PatchList_4 = D3D11_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST,
    PatchList_5 = D3D11_PRIMITIVE_TOPOLOGY_5_CONTROL_POINT_PATCHLIST,
    PatchList_6 = D3D11_PRIMITIVE_TOPOLOGY_6_CONTROL_POINT_PATCHLIST,
};

enum eShaderFlags_
{
    eShaderFlags_None           = 0,
    eShaderFlags_VertexShader   = BIT(0),
    eShaderFlags_HullShader     = BIT(1),
    eShaderFlags_DomainShader   = BIT(2),
    eShaderFlags_GeometryShader = BIT(3),
    eShaderFlags_PixelShader    = BIT(4),
    eShaderFlags_ComputeShader  = BIT(5),

    eShaderFlags_VertexPixelShader = eShaderFlags_VertexShader | eShaderFlags_PixelShader
};

using eShaderFlags = uint32;

enum class eShaderStage
{
    Unknown,
    VertexShader,
    HullShader,
    DomainShader,
    GeometryShader,
    PixelShader,
    ComputeShader,
    Count
};

enum
{
    eShaderType_Count = static_cast<uint32>(eShaderStage::Count)
};

using eTextureLoadFlags = uint32;

enum eTextureLoadFlags_
{
    eTextureLoadFlags_None            = 0,
    eTextureLoadFlags_GenerateMips    = BIT(0),   // generate mipmap
    eTextureLoadFlags_DisableMipmaps  = BIT(1),   // mipmap levels fix 1
    eTextureLoadFlags_ReverseTone     = BIT(2),   // reverse tone mapping
    eTextureLoadFlags_GenerateCubeMap = BIT(3)    // cube map
};

enum class eFormat
{
    Unknown = DXGI_FORMAT_UNKNOWN,

    Typeless32_1 = DXGI_FORMAT_R32_TYPELESS,

    Float32_1 = DXGI_FORMAT_R32_FLOAT,
    Float32_2 = DXGI_FORMAT_R32G32_FLOAT,
    Float32_3 = DXGI_FORMAT_R32G32B32_FLOAT,
    Float32_4 = DXGI_FORMAT_R32G32B32A32_FLOAT,

    Float16_1 = DXGI_FORMAT_R16_FLOAT,
    Float16_2 = DXGI_FORMAT_R16G16_FLOAT,
    Float16_4 = DXGI_FORMAT_R16G16B16A16_FLOAT,

    UNorm8_1 = DXGI_FORMAT_R8_UNORM,
    UNorm8_2 = DXGI_FORMAT_R8G8_UNORM,
    UNorm8_4 = DXGI_FORMAT_R8G8B8A8_UNORM,

    UInt32_1 = DXGI_FORMAT_R32_UINT,
    UInt32_2 = DXGI_FORMAT_R32G32_UINT,
    UInt32_4 = DXGI_FORMAT_R32G32B32A32_UINT,

    SInt32_1 = DXGI_FORMAT_R32_SINT,
    SInt32_2 = DXGI_FORMAT_R32G32_SINT,
    SInt32_4 = DXGI_FORMAT_R32G32B32A32_SINT,

    Depth_UNorm16               = DXGI_FORMAT_D16_UNORM,
    Depth_UNorm24_Stencil_UInt8 = DXGI_FORMAT_D24_UNORM_S8_UINT,
    Depth_Float32               = DXGI_FORMAT_D32_FLOAT,
    Depth_Float32_Stencil_UInt8 = DXGI_FORMAT_D32_FLOAT_S8X24_UINT,
};

using eBindFlags = uint32;

enum eBindFlags_
{
    eBindFlags_None            = 0,
    eBindFlags_VertexBuffer    = D3D11_BIND_VERTEX_BUFFER,
    eBindFlags_IndexBuffer     = D3D11_BIND_INDEX_BUFFER,
    eBindFlags_ConstantBuffer  = D3D11_BIND_CONSTANT_BUFFER,
    eBindFlags_ShaderResource  = D3D11_BIND_SHADER_RESOURCE,
    eBindFlags_StreamOutput    = D3D11_BIND_STREAM_OUTPUT,
    eBindFlags_RenderTarget    = D3D11_BIND_RENDER_TARGET,
    eBindFlags_DepthStencil    = D3D11_BIND_DEPTH_STENCIL,
    eBindFlags_UnorderedAccess = D3D11_BIND_UNORDERED_ACCESS,
};

using eCPUAccessFlags = uint32;

enum eCPUAccessFlags_
{
    eCPUAccessFlags_None      = 0,
    eCPUAccessFlags_Write     = D3D11_CPU_ACCESS_WRITE,
    eCPUAccessFlags_Read      = D3D11_CPU_ACCESS_READ,
    eCPUAccessFlags_ReadWrite = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE,
};

using eTextureCreationFlags = uint32;

enum eTextureCreationFlags_
{
    eTextureCreationFlags_None            = NULL,
    eTextureCreationFlags_CubeMap         = D3D11_RESOURCE_MISC_TEXTURECUBE,
    eTextureCreationFlags_StructureBuffer = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED,
    eTextureCreationFlags_RawBuffer       = D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS
};

enum class eSamplerMode
{
    Default,
    Comparison,
    Minimum,
    Maximum,
};

enum class eSamplerFilter
{
    MinMagMipPoint,
    MinMagPoint_MipLinear,
    MinPoint_MagLinear_MipPoint,
    MinPoint_MagMipLinear,
    MinLinear_MagMipPoint,
    MinLinear_MagPoint_MipLinear,
    MinMagLinear_MipPoint,
    MinMagMipLinear,
    Anisotropic,
};

enum class eSamplerAddressMode
{
    Wrap       = D3D11_TEXTURE_ADDRESS_WRAP,
    Mirror     = D3D11_TEXTURE_ADDRESS_MIRROR,
    Clamp      = D3D11_TEXTURE_ADDRESS_CLAMP,
    Border     = D3D11_TEXTURE_ADDRESS_BORDER,
    MirrorOnce = D3D11_TEXTURE_ADDRESS_MIRROR_ONCE,
};

enum class eLightType
{
    None        = 0,
    Directional = 1,
    Point       = 2,
    Spot        = 3,
};

}   // namespace crab