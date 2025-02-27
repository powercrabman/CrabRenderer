#pragma once

namespace crab
{
constexpr uint32 TEXTURE_SLOT_MAX               = 64;
constexpr uint32 SAMPLER_SLOT_MAX               = 64;
constexpr uint32 CONSTANT_BUFFER_SLOT_MAX       = 64;
constexpr uint32 UAV_SLOT_MAX                   = D3D11_PS_CS_UAV_REGISTER_COUNT;
constexpr int32  MSAA_MAX_QUALITY               = -1;
constexpr uint32 CRAB_SKIN_MAX                  = 8;

//===================================================
// Enums
//===================================================

enum class eTopology
{
    None = D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED,

    PointList = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST,

    LineList = D3D11_PRIMITIVE_TOPOLOGY_LINELIST,
    LineStrip = D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP,

    TriangleList = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
    TriangleStrip = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP,

    PatchList_3 = D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST,
    PatchList_4 = D3D11_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST,
    PatchList_5 = D3D11_PRIMITIVE_TOPOLOGY_5_CONTROL_POINT_PATCHLIST,
    PatchList_6 = D3D11_PRIMITIVE_TOPOLOGY_6_CONTROL_POINT_PATCHLIST,
};

inline const char* ToString(eTopology e)
{
    switch (e)
    {
        case eTopology::None:
            return "None";
        case eTopology::PointList:
            return "PointList";
        case eTopology::LineList:
            return "LineList";
        case eTopology::LineStrip:
            return "LineStrip";
        case eTopology::TriangleList:
            return "TriangleList";
        case eTopology::TriangleStrip:
            return "TriangleStrip";
        case eTopology::PatchList_3:
            return "PatchList_3";
        case eTopology::PatchList_4:
            return "PatchList_4";
        case eTopology::PatchList_5:
            return "PatchList_5";
        case eTopology::PatchList_6:
            return "PatchList_6";
        default:
            assert(false);
            return "unknown";
    }
}



//===================================================
// Error Handling
//===================================================

inline std::string TranslateHRESULT(HRESULT hr)
{
    char* errorMsg = nullptr;

    FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                   nullptr,
                   hr,
                   MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                   reinterpret_cast<LPSTR>(&errorMsg),
                   0,
                   nullptr);

    std::string message = (errorMsg) ? errorMsg : "Unknown error";
    LocalFree(errorMsg);
    return message;
}

//===================================================
// Shader Flags
//===================================================

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

//===================================================
// Data Type
//===================================================

enum class eFormat
{
    Unknown = DXGI_FORMAT_UNKNOWN,

    Float32          = DXGI_FORMAT_R32_FLOAT,
    Float32_Typeless = DXGI_FORMAT_R32_TYPELESS,
    Float32x2        = DXGI_FORMAT_R32G32_FLOAT,
    Float32x3        = DXGI_FORMAT_R32G32B32_FLOAT,
    Float32x4        = DXGI_FORMAT_R32G32B32A32_FLOAT,

    Float16   = DXGI_FORMAT_R16_FLOAT,
    Float16x2 = DXGI_FORMAT_R16G16_FLOAT,
    Float16x4 = DXGI_FORMAT_R16G16B16A16_FLOAT,

    UNorm8   = DXGI_FORMAT_R8_UNORM,
    UNorm8x2 = DXGI_FORMAT_R8G8_UNORM,
    UNorm8x4 = DXGI_FORMAT_R8G8B8A8_UNORM,

    Depth_UNorm16               = DXGI_FORMAT_D16_UNORM,
    Depth_UNorm24_Stencil_UInt8 = DXGI_FORMAT_D24_UNORM_S8_UINT,
    Depth_Float32               = DXGI_FORMAT_D32_FLOAT,
    Depth_Float32_Stencil_UInt8 = DXGI_FORMAT_D32_FLOAT_S8X24_UINT
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

}   // namespace crab