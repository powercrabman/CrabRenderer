#include "CrabPch.h"

#include "Shaders.h"

#include "D11Renderer.h"

namespace crab
{

//===================================================
// Vertex Shader
//===================================================

void VertexShader::LoadFromHLSL(
    const std::filesystem::path& in_shaderPath,
    const std::string_view       in_entryPoint,
    const InputElements&         in_inputElems,
    const ShaderMacros&          in_macros)
{
    auto blob = ShaderUtil::LoadShaderFile(in_shaderPath, in_entryPoint, "vs_5_0", in_macros);
    _CreateVertexShader(blob, in_inputElems);
}

void VertexShader::LoadFromCSO(
    const std::filesystem::path& in_shaderPath,
    const InputElements&         in_inputElems)
{
    auto blob = ShaderUtil::LoadShaderFile(in_shaderPath, "", "");
    _CreateVertexShader(blob, in_inputElems);
}

void VertexShader::InitFromCode(
    const std::string_view in_shaderCode,
    const std::string_view in_entryPoint,
    const InputElements&   in_inputElems,
    const ShaderMacros&    in_macros)
{
    auto blob = ShaderUtil::LoadShaderCode(in_shaderCode, in_entryPoint, "vs_5_0", in_macros);
    _CreateVertexShader(blob, in_inputElems);
}

void VertexShader::Bind() const
{
    GetRenderer().SetVertexShader(m_vertexShader.Get(), m_inputLayout.Get());
}

void VertexShader::_CreateVertexShader(
    const ComPtr<ID3DBlob>& in_blob,
    const InputElements&    in_inputElems)
{
    auto d = GetRenderer().GetDevice();

    // - Create Vertex Shader
    CheckD3D11Result(d->CreateVertexShader(in_blob->GetBufferPointer(),
                                           in_blob->GetBufferSize(),
                                           nullptr,
                                           m_vertexShader.GetAddressOf()),
                     "CreateVertexShader Fail.");

    // - Create Input Layout
    CheckD3D11Result(d->CreateInputLayout(in_inputElems.Get(),
                                          in_inputElems.GetSize(),
                                          in_blob->GetBufferPointer(),
                                          in_blob->GetBufferSize(),
                                          m_inputLayout.GetAddressOf()),
                     "CreateInputLayout Fail.");

    m_blob = in_blob;
}

//===================================================
// Pixel Shader
//===================================================

void PixelShader::LoadFromHLSL(
    const std::filesystem::path& in_shaderPath,
    const std::string_view       in_entryPoint,
    const ShaderMacros&          in_macro)
{
    auto blob = ShaderUtil::LoadShaderFile(in_shaderPath, in_entryPoint, "ps_5_0", in_macro);
    _CreatePixelShader(blob);
}

void PixelShader::LoadFromCSO(const std::filesystem::path& in_shaderPath)
{
    auto blob = ShaderUtil::LoadShaderFile(in_shaderPath, "", "");
    _CreatePixelShader(blob);
}

void PixelShader::InitFromCode(
    const std::string_view in_shaderCode,
    const std::string_view in_entryPoint,
    const ShaderMacros&    in_macro)
{
    auto blob = ShaderUtil::LoadShaderCode(in_shaderCode, in_entryPoint, "ps_5_0", in_macro);
    _CreatePixelShader(blob);
}

void PixelShader::Bind() const
{
    GetRenderer().SetPixelShader(m_pixelShader.Get());
}

void PixelShader::_CreatePixelShader(const ComPtr<ID3DBlob>& in_blob)
{
    auto d = GetRenderer().GetDevice();

    // - Create Pixel Shader
    CheckD3D11Result(d->CreatePixelShader(in_blob->GetBufferPointer(),
                                          in_blob->GetBufferSize(),
                                          nullptr,
                                          m_pixelShader.GetAddressOf()),
                     "CreatePixelShader Fail.");
}

//===================================================
// Geometry Shader
//===================================================

void GeometryShader::LoadFromHLSL(
    const std::filesystem::path& in_shaderPath,
    const std::string_view       in_entryPoint,
    const ShaderMacros&          in_macro)
{
    auto blob = ShaderUtil::LoadShaderFile(in_shaderPath, in_entryPoint, "gs_5_0", in_macro);
    _CreateGeometryShader(blob);
}

void GeometryShader::LoadFromCSO(const std::filesystem::path& in_shaderPath)
{
    auto blob = ShaderUtil::LoadShaderFile(in_shaderPath, "", "");
    _CreateGeometryShader(blob);
}

void GeometryShader::InitFromCode(
    const std::string_view in_shaderCode,
    const std::string_view in_entryPoint,
    const ShaderMacros&    in_macro)
{
    auto blob = ShaderUtil::LoadShaderCode(in_shaderCode, in_entryPoint, "gs_5_0", in_macro);
    _CreateGeometryShader(blob);
}

void GeometryShader::Bind() const
{
    GetRenderer().SetGeometryShader(m_geometryShader.Get());
}

void GeometryShader::_CreateGeometryShader(const ComPtr<ID3DBlob>& in_blob)
{
    auto d = GetRenderer().GetDevice();

    // - Create Geometry Shader
    CheckD3D11Result(d->CreateGeometryShader(in_blob->GetBufferPointer(),
                                             in_blob->GetBufferSize(),
                                             nullptr,
                                             m_geometryShader.GetAddressOf()),
                     "CreateGeometryShader Fail.");
}

//===================================================
// Demain Shader
//===================================================

void DomainShader::LoadFromHLSL(
    const std::filesystem::path& in_shaderPath,
    const std::string_view       in_entryPoint,
    const ShaderMacros&          in_macro)
{
    auto blob = ShaderUtil::LoadShaderFile(in_shaderPath, in_entryPoint, "ds_5_0", in_macro);
    _CreateDomainShader(blob);
}

void DomainShader::LoadFromCSO(const std::filesystem::path& in_shaderPath)
{
    auto blob = ShaderUtil::LoadShaderFile(in_shaderPath, "", "ds_5_0");
    _CreateDomainShader(blob);
}

void DomainShader::InitFromCode(
    const std::string_view in_shaderCode,
    const std::string_view in_entryPoint,
    const ShaderMacros&    in_macro)
{
    auto blob = ShaderUtil::LoadShaderCode(in_shaderCode, in_entryPoint, "ds_5_0", in_macro);
    _CreateDomainShader(blob);
}

void DomainShader::Bind() const
{

    GetRenderer().SetDomainShader(m_domainShader.Get());
}

void DomainShader::_CreateDomainShader(const ComPtr<ID3DBlob>& in_blob)
{
    auto d = GetRenderer().GetDevice();

    // - Create Domain Shader
    CheckD3D11Result(d->CreateDomainShader(in_blob->GetBufferPointer(),
                                           in_blob->GetBufferSize(),
                                           nullptr,
                                           m_domainShader.GetAddressOf()),
                     "CreateDomainShader Fail.");
}

//===================================================
// Demain Shader
//===================================================

void HullShader::LoadFromHLSL(
    const std::filesystem::path& in_shaderPath,
    const std::string_view       in_entryPoint,
    const ShaderMacros&          in_macro)
{
    auto blob = ShaderUtil::LoadShaderFile(in_shaderPath, in_entryPoint, "ds_5_0", in_macro);
    _CreateHullShader(blob);
}

void HullShader::LoadFromCSO(const std::filesystem::path& in_shaderPath)
{
    auto blob = ShaderUtil::LoadShaderFile(in_shaderPath, "", "");
    _CreateHullShader(blob);
}

void HullShader::InitFromCode(
    const std::string_view in_shaderCode,
    const std::string_view in_entryPoint,
    const ShaderMacros&    in_macro)
{
    auto blob = ShaderUtil::LoadShaderCode(in_shaderCode, in_entryPoint, "ds_5_0", in_macro);
    _CreateHullShader(blob);
}

void HullShader::Bind() const
{
    GetRenderer().SetHullShader(m_hullShader.Get());
}

void HullShader::_CreateHullShader(const ComPtr<ID3DBlob>& in_blob)
{
    auto d = GetRenderer().GetDevice();

    // - Create Hull Shader
    CheckD3D11Result(d->CreateHullShader(in_blob->GetBufferPointer(),
                                         in_blob->GetBufferSize(),
                                         nullptr,
                                         m_hullShader.GetAddressOf()),
                     "CreateHullShader Fail.");
}

//===================================================
// Compute Shader
//===================================================

void ComputeShader::LoadFromHLSL(
    const std::filesystem::path& in_shaderPath,
    const std::string_view       in_entryPoint,
    const ShaderMacros&          in_macro)
{
    auto blob = ShaderUtil::LoadShaderFile(in_shaderPath, in_entryPoint, "cs_5_0", in_macro);
    _CreateComputeShader(blob);
}

void ComputeShader::LoadFromCSO(const std::filesystem::path& in_shaderPath)
{
    auto blob = ShaderUtil::LoadShaderFile(in_shaderPath, "", "");
    _CreateComputeShader(blob);
}

void ComputeShader::InitFromCode(
    const std::string_view in_shaderCode,
    const std::string_view in_entryPoint,
    const ShaderMacros&    in_macro)
{
    auto blob = ShaderUtil::LoadShaderCode(in_shaderCode, in_entryPoint, "cs_5_0");
    _CreateComputeShader(blob);
}

void ComputeShader::Dispatch(uint32 in_x, uint32 in_y, uint32 in_z) const
{
    GetRenderer().DispatchComputeShader(m_computeShader.Get(), in_x, in_y, in_z);
}

void ComputeShader::_CreateComputeShader(const ComPtr<ID3DBlob>& in_blob)
{
    auto d = GetRenderer().GetDevice();

    // - Create Compute Shader
    CheckD3D11Result(d->CreateComputeShader(in_blob->GetBufferPointer(),
                                            in_blob->GetBufferSize(),
                                            nullptr,
                                            m_computeShader.GetAddressOf()),
                     "CreateComputeShader Fail.");
}

}   // namespace crab