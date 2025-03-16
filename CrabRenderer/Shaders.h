#pragma once
#include "D11Utils.h"
#include "InputElements.h"

namespace crab
{

class ConstantBufferBase;

//===================================================
// Vertex Shader
//===================================================

class VertexShader
{
public:
    //===================================================
    // Factory
    //===================================================

    void LoadFromHLSL(const std::filesystem::path& in_shaderPath,
                      const std::string_view       in_entryPoint,
                      const InputElements&         in_inputElems,
                      const ShaderMacros&          in_macro = {});

    void LoadFromCSO(const std::filesystem::path& in_shaderPath,
                    const InputElements&         in_inputElems);

    void InitFromCode(const std::string_view in_shaderCode,
                      const std::string_view in_entryPoint,
                      const InputElements&   in_inputElems,
                      const ShaderMacros&    in_macro = {});

    //===================================================
    // Member
    //===================================================

    static constexpr eShaderStage GetType() { return eShaderStage::VertexShader; }

    void Bind() const;

private:
    void _CreateVertexShader(
        const ComPtr<ID3DBlob>& in_blob,
        const InputElements&    in_inputElems);

    ComPtr<ID3D11InputLayout>  m_inputLayout;
    ComPtr<ID3D11VertexShader> m_vertexShader;
    ComPtr<ID3DBlob>           m_blob;
};

//===================================================
// Pixel Shader
//===================================================

class PixelShader
{
public:
    //===================================================
    // Factory
    //===================================================

    void LoadFromHLSL(const std::filesystem::path& in_shaderPath,
                                         const std::string_view       in_entryPoint,
                                         const ShaderMacros&          in_macro = {});

    void LoadFromCSO(const std::filesystem::path& in_shaderPath);

    void InitFromCode(const std::string_view in_shaderCode,
                                         const std::string_view in_entryPoint,
                                         const ShaderMacros&    in_macro = {});

    //===================================================
    // Member
    //===================================================

    static constexpr eShaderStage GetType() { return eShaderStage::PixelShader; }

    void Bind() const;

private:
    void _CreatePixelShader(
        const ComPtr<ID3DBlob>& in_blob);

    ComPtr<ID3D11PixelShader> m_pixelShader;
    ComPtr<ID3DBlob>          m_blob;
};

//===================================================
// Geometry Shader
//===================================================

class GeometryShader
{
public:
    //===================================================
    // Factory
    //===================================================
    void LoadFromHLSL(const std::filesystem::path& in_shaderPath,
                                            const std::string_view       in_entryPoint,
                                            const ShaderMacros&          in_macro = {});

    void LoadFromCSO(const std::filesystem::path& in_shaderPath);

    void InitFromCode(const std::string_view in_shaderCode,
                                            const std::string_view in_entryPoint,
                                            const ShaderMacros&    in_macro = {});

    //===================================================
    // Member
    //===================================================
    static constexpr eShaderStage GetType() { return eShaderStage::GeometryShader; }

    void Bind() const;

private:
    void _CreateGeometryShader(
        const ComPtr<ID3DBlob>& in_blob);

    ComPtr<ID3D11GeometryShader> m_geometryShader;
    ComPtr<ID3DBlob>             m_blob;
};

//===================================================
// Domain Shader
//===================================================

class DomainShader
{
public:
    //===================================================
    // Factory
    //===================================================
    void LoadFromHLSL(const std::filesystem::path& in_shaderPath,
                                          const std::string_view       in_entryPoint,
                                          const ShaderMacros&          in_macro = {});

    void LoadFromCSO(const std::filesystem::path& in_shaderPath);

    void InitFromCode(const std::string_view in_shaderCode,
                                          const std::string_view in_entryPoint,
                                          const ShaderMacros&    in_macro = {});

    //===================================================
    // Member
    //===================================================
    static constexpr eShaderStage GetType()
    {
        return eShaderStage::DomainShader;
    }

    void Bind() const;

private:
    void _CreateDomainShader(
        const ComPtr<ID3DBlob>& in_blob);

    ComPtr<ID3D11DomainShader> m_domainShader;
    ComPtr<ID3DBlob>           m_blob;
};

//===================================================
// Hull Shader
//===================================================

class HullShader
{
public:
    //===================================================
    // Factory
    //===================================================
    void LoadFromHLSL(const std::filesystem::path& in_shaderPath,
                                        const std::string_view       in_entryPoint,
                                        const ShaderMacros&          in_macro = {});

    void LoadFromCSO(const std::filesystem::path& in_shaderPath);

    void InitFromCode(const std::string_view in_shaderCode,
                                        const std::string_view in_entryPoint,
                                        const ShaderMacros&    in_macro = {});

    //===================================================
    // Member
    //===================================================
    static constexpr eShaderStage GetType() { return eShaderStage::HullShader; }

    void Bind() const;

private:
    void _CreateHullShader(
        const ComPtr<ID3DBlob>& in_blob);

    ComPtr<ID3D11HullShader> m_hullShader;
    ComPtr<ID3DBlob>         m_blob;
};

//===================================================
// Compute Shader
//===================================================

class ComputeShader
{
public:
    //===================================================
    // Factory
    //===================================================
    void LoadFromHLSL(const std::filesystem::path& in_shaderPath,
                                           const std::string_view       in_entryPoint,
                                           const ShaderMacros&          in_macro = {});

    void LoadFromCSO(const std::filesystem::path& in_shaderPath);

    void InitFromCode(const std::string_view in_shaderCode,
                                           const std::string_view in_entryPoint,
                                           const ShaderMacros&    in_macro = {});

    //===================================================
    // Member
    //===================================================
    static constexpr eShaderStage GetType() { return eShaderStage::ComputeShader; }

    void Dispatch(uint32 in_x, uint32 in_y, uint32 in_z) const;

private:
    void _CreateComputeShader(const ComPtr<ID3DBlob>& in_blob);

    ComPtr<ID3D11ComputeShader> m_computeShader;
    ComPtr<ID3DBlob>            m_blob;
};

}   // namespace crab
