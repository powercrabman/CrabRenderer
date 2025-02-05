#pragma once
#include "D11_Inputlayout.h"

namespace crab
{
struct D11_VertexShader
{
    // - Factory
    static Ref<D11_VertexShader> CreateFromFile(const std::filesystem::path& in_shaderPath,
                                                const std::string_view       in_entryPoint,
                                                const D11_InputElements&     in_inputElems);

    static Ref<D11_VertexShader> CreateFromString(const std::string_view   in_shaderCode,
                                                  const std::string_view   in_entryPoint,
                                                  const D11_InputElements& in_inputElems);

    ComPtr<ID3D11InputLayout>  inputLayout;
    ComPtr<ID3D11VertexShader> vertexShader;
    ComPtr<ID3DBlob>           blob;
};

struct D11_PixelShader
{
    // - Factory
    static Ref<D11_PixelShader> CreateFromFile(const std::filesystem::path& in_shaderPath,
                                               const std::string_view       in_entryPoint);

    static Ref<D11_PixelShader> CreateFromString(const std::string_view in_shaderCode,
                                                 const std::string_view in_entryPoint);

    ComPtr<ID3D11PixelShader> pixelShader;
    ComPtr<ID3DBlob>          blob;
};

struct D11_GeometryShader
{
    // - Factory
    static Ref<D11_GeometryShader> CreateFromFile(const std::filesystem::path& in_shaderPath,
                                                  const std::string_view       in_entryPoint);

    static Ref<D11_GeometryShader> CreateFromString(const std::string_view in_shaderCode,
                                                    const std::string_view in_entryPoint);

    ComPtr<ID3D11GeometryShader> geometryShader;
    ComPtr<ID3DBlob>             blob;
};

struct D11_ComputeShader
{
    // - Factory
    static Ref<D11_ComputeShader> CreateFromFile(const std::filesystem::path& in_shaderPath,
                                                 const std::string_view       in_entryPoint);

    static Ref<D11_ComputeShader> CreateFromString(const std::string_view in_shaderCode,
                                                   const std::string_view in_entryPoint);

    ComPtr<ID3D11ComputeShader> computeShader;
    ComPtr<ID3DBlob>            blob;
};

}   // namespace crab
