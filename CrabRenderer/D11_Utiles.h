#pragma once

namespace crab
{

constexpr uint32 TEXTURE_SLOT_MAX         = 32;
constexpr uint32 SAMPLER_SLOT_MAX         = 32;
constexpr uint32 CONSTANT_BUFFER_SLOT_MAX = 32;

//===================================================
// Enums
//===================================================

enum class eTopology
{
    None          = D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED,
    PointList     = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST,
    LineList      = D3D11_PRIMITIVE_TOPOLOGY_LINELIST,
    LineStrip     = D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP,
    TriangleList  = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
    TriangleStrip = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP,
    Patchlist_3   = D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST,
    Patchlist_4   = D3D11_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST,
    Patchlist_5   = D3D11_PRIMITIVE_TOPOLOGY_5_CONTROL_POINT_PATCHLIST,
    Patchlist_6   = D3D11_PRIMITIVE_TOPOLOGY_6_CONTROL_POINT_PATCHLIST,
};

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
    eShaderFlags_PixelShader    = BIT(1),
    eShaderFlags_ComputeShader  = BIT(2),
    eShaderFlags_GeometryShader = BIT(3),
};

using eShaderFlags = uint32;

enum class eShader
{
    VertexShader,
    PixelShader,
    ComputeShader
};

//===================================================
// Data Type
//===================================================

enum class eDataFormat
{
    Float,
    Float2,
    Float3,
    Float4
};

inline DXGI_FORMAT ToDXGI_FORMAT(eDataFormat in_datatype)
{
    switch (in_datatype)
    {
        case eDataFormat::Float:
            return DXGI_FORMAT_R32_FLOAT;

        case eDataFormat::Float2:
            return DXGI_FORMAT_R32G32_FLOAT;

        case eDataFormat::Float3:
            return DXGI_FORMAT_R32G32B32_FLOAT;

        case eDataFormat::Float4:
            return DXGI_FORMAT_R32G32B32A32_FLOAT;

        default:
            CRAB_DEBUG_BREAK("Unknown Data Type.");
            return DXGI_FORMAT_UNKNOWN;
    }
}

//===================================================
// Macro
//===================================================

#define cbuffer struct alignas(16)

//===================================================
// Texture
//===================================================

ComPtr<ID3D11Texture2D> CreateID3D11Texture2D(const D3D11_TEXTURE2D_DESC& in_desc);

}   // namespace crab