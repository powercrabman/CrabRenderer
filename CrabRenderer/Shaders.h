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
    D11_RESOURCE_CTOR(VertexShader);

public:
    //===================================================
    // Factory
    //===================================================

    static Ref<VertexShader> CreateFromHLSL(const std::filesystem::path& in_shaderPath,
                                            const std::string_view       in_entryPoint,
                                            const InputElements&         in_inputElems,
                                            const ShaderMacros&          in_macros);

    static Ref<VertexShader> CreateFromCSO(const std::filesystem::path& in_shaderPath,
                                           const InputElements&         in_inputElems);

    static Ref<VertexShader> CreateFromCode(const std::string_view in_shaderCode,
                                            const std::string_view in_entryPoint,
                                            const InputElements&   in_inputElems,
                                            const ShaderMacros&    in_macros);

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
    D11_RESOURCE_CTOR(PixelShader);

public:
    //===================================================
    // Factory
    //===================================================

    static Ref<PixelShader> CreateFromHLSL(const std::filesystem::path& in_shaderPath,
                                           const std::string_view       in_entryPoint,
                                           const ShaderMacros&          in_macro);

    static Ref<PixelShader> CreateFromCSO(const std::filesystem::path& in_shaderPath);

    static Ref<PixelShader> CreateFromCode(const std::string_view in_shaderCode,
                                           const std::string_view in_entryPoint,
                                           const ShaderMacros&    in_macro);

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
    D11_RESOURCE_CTOR(GeometryShader);

public:
    //===================================================
    // Factory
    //===================================================
    static Ref<GeometryShader> CreateFromHLSL(const std::filesystem::path& in_shaderPath,
                                              const std::string_view       in_entryPoint,
                                              const ShaderMacros&          in_macro);

    static Ref<GeometryShader> CreateFromCSO(const std::filesystem::path& in_shaderPath);

    static Ref<GeometryShader> CreateFromCode(const std::string_view in_shaderCode,
                                              const std::string_view in_entryPoint,
                                              const ShaderMacros&    in_macro);

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
    D11_RESOURCE_CTOR(DomainShader);

public:
    //===================================================
    // Factory
    //===================================================
    static Ref<DomainShader> CreateFromHLSL(const std::filesystem::path& in_shaderPath,
                                            const std::string_view       in_entryPoint,
                                            const ShaderMacros&          in_macro);

    static Ref<DomainShader> CreateFromCSO(const std::filesystem::path& in_shaderPath);

    static Ref<DomainShader> CreateFromCode(const std::string_view in_shaderCode,
                                            const std::string_view in_entryPoint,
                                            const ShaderMacros&    in_macro);

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
    D11_RESOURCE_CTOR(HullShader);

public:
    //===================================================
    // Factory
    //===================================================
    static Ref<HullShader> CreateFromHLSL(const std::filesystem::path& in_shaderPath,
                                          const std::string_view       in_entryPoint,
                                          const ShaderMacros&          in_macro);

    static Ref<HullShader> CreateFromCSO(const std::filesystem::path& in_shaderPath);

    static Ref<HullShader> CreateFromCode(const std::string_view in_shaderCode,
                                          const std::string_view in_entryPoint,
                                          const ShaderMacros&    in_macro);

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
    D11_RESOURCE_CTOR(ComputeShader);

public:
    //===================================================
    // Factory
    //===================================================
    static Ref<ComputeShader> CreateFromHLSL(const std::filesystem::path& in_shaderPath,
                                             const std::string_view       in_entryPoint,
                                             const ShaderMacros&          in_macro);

    static Ref<ComputeShader> CreateFromCSO(const std::filesystem::path& in_shaderPath);

    static Ref<ComputeShader> CreateFromCode(const std::string_view in_shaderCode,
                                             const std::string_view in_entryPoint,
                                             const ShaderMacros&    in_macro);

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
