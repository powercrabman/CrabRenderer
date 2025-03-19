#pragma once
#include "ErrorHandler.h"

namespace crab
{
constexpr uint32 MSAA_SAMPLE_COUNT = 4;

constexpr uint32 SHADER_SRV_SLOT_COUNT           = D3D11_COMMONSHADER_INPUT_RESOURCE_REGISTER_COUNT;
constexpr uint32 SHADER_SAMPLER_SLOT_COUNT       = D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT;
constexpr uint32 SHADER_CONSTANTS_SLOT_COUNT     = D3D11_COMMONSHADER_CONSTANT_BUFFER_HW_SLOT_COUNT;
constexpr uint32 SHADER_UAV_SLOT_COUNT           = D3D11_PS_CS_UAV_REGISTER_COUNT;
constexpr uint32 SHADER_RENDER_TARGET_SLOT_COUNT = D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT;
constexpr uint32 CRAB_SKIN_MAX                   = 8;

// this will modify
constexpr uint32 MAX_LIGHTS               = 4;
constexpr uint32 MAX_CASCADE_SHADOW_LEVEL = 4;
constexpr uint32 SHADOW_MAP_LENGTH        = 2048;

constexpr float LIGHT_NEAR_PLANE = 0.1f;

constexpr uint32 SHADOW_SLOT_OFFSET  = 20;
constexpr uint32 BASIC_SHADOW_SLOT   = SHADOW_SLOT_OFFSET;
constexpr uint32 OMNI_SHADOW_SLOT    = BASIC_SHADOW_SLOT + MAX_LIGHTS;
constexpr uint32 CASCADE_SHADOW_SLOT = OMNI_SHADOW_SLOT + MAX_LIGHTS;

enum class eLightType
{
    None        = 0,
    Directional = 1,
    Point       = 2,
    Spot        = 3,
};

enum class eResourceType
{
    Unknown,
    Buffer,
    Texture2D,
    Texture2DArray,
    TextureCube
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

    Depth_UNorm16               = DXGI_FORMAT_D16_UNORM,
    Depth_UNorm24_Stencil_UInt8 = DXGI_FORMAT_D24_UNORM_S8_UINT,
    Depth_Float32               = DXGI_FORMAT_D32_FLOAT,
    Depth_Float32_Stencil_UInt8 = DXGI_FORMAT_D32_FLOAT_S8X24_UINT,
};

static_assert(magic_enum::enum_count<eFormat>() == 16, "eFormat is not updated.");

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

using eSRVCreationFlags = uint32;

struct MSAA
{
    MSAA() = default;
    MSAA(uint32 in_sampleCount, uint32 in_quality)
        : sampleCount(in_sampleCount)
        , quality(in_quality)
    {
        enableMSAA = sampleCount > 1;
    }

    static MSAA ComputeBestMSAA(bool enableMSAA, ID3D11Device* in_device, DXGI_FORMAT in_format, uint32 in_sampleCount)
    {
        if (enableMSAA)
        {
            uint32 qualityLevels = 0;
            if (d3d::CheckOK(
                    in_device->CheckMultisampleQualityLevels(
                        in_format,
                        in_sampleCount,
                        &qualityLevels),
                    "CheckMultisampleQualityLevels Fail."))
            {
                return DisableMSAA();
            }
        }

        return DisableMSAA();
    }

    static MSAA DisableMSAA()
    {
        return MSAA { 1, 0 };
    }

    bool   enableMSAA  = false;
    uint32 sampleCount = 1;
    uint32 quality     = 0;
};

struct HDR
{
    bool enableHDR = false;
};

struct RendererConfig
{
    MSAA msaa;
    HDR  hdr;

    D3D_FEATURE_LEVEL featureLevel;

    bool enableVSync;
};

}   // namespace crab