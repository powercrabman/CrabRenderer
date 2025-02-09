#pragma once
#include "D11_Inputlayout.h"

namespace crab
{

class D11_VertexShader
{
public:
    // - Factory
    static Ref<D11_VertexShader> CreateFromFile(const std::filesystem::path& in_shaderPath,
                                                const std::string_view       in_entryPoint,
                                                const D11_InputElements&     in_inputElems);

    static Ref<D11_VertexShader> CreateFromString(const std::string_view   in_shaderCode,
                                                  const std::string_view   in_entryPoint,
                                                  const D11_InputElements& in_inputElems);

    void Bind();

private:
    ComPtr<ID3D11InputLayout>  m_inputlayout;
    ComPtr<ID3D11VertexShader> m_vertexShader;
    ComPtr<ID3DBlob>           m_blob;
};

class D11_PixelShader
{
public:
    // - Factory
    static Ref<D11_PixelShader> CreateFromFile(const std::filesystem::path& in_shaderPath,
                                               const std::string_view       in_entryPoint);

    static Ref<D11_PixelShader> CreateFromString(const std::string_view in_shaderCode,
                                                 const std::string_view in_entryPoint);

    void Bind();

private:
    ComPtr<ID3D11PixelShader> m_pixelShader;
    ComPtr<ID3DBlob>          m_blob;
};

class D11_GeometryShader
{
public:
    // - Factory
    static Ref<D11_GeometryShader> CreateFromFile(const std::filesystem::path& in_shaderPath,
                                                  const std::string_view       in_entryPoint);

    static Ref<D11_GeometryShader> CreateFromString(const std::string_view in_shaderCode,
                                                    const std::string_view in_entryPoint);

    void Bind();

private:
    ComPtr<ID3D11GeometryShader> m_geometryShader;
    ComPtr<ID3DBlob>             m_blob;
};

class D11_ComputeShader
{
public:
    // - Factory
    static Ref<D11_ComputeShader> CreateFromFile(const std::filesystem::path& in_shaderPath,
                                                 const std::string_view       in_entryPoint);

    static Ref<D11_ComputeShader> CreateFromString(const std::string_view in_shaderCode,
                                                   const std::string_view in_entryPoint);

    void Bind();

private:
    ComPtr<ID3D11ComputeShader> m_computeShader;
    ComPtr<ID3DBlob>            m_blob;
};

}   // namespace crab
