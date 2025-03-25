#include "InternalPCH.h"

#include "Shaders.h"

#include "D3D11Utils.h"
#include "InputLayout.h"
#include "Renderer.h"

namespace crab
{

//===================================================
// Vertex Shader
//===================================================

void VertexShader::LoadFromHLSL(
    const std::filesystem::path& in_shaderPath,
    const std::string_view       in_entryPoint,
    const ShaderMacros&          in_macro,
    const Ref<InputLayout>&      in_inputLayout)
{
    auto blob = d3d11::shader::LoadFromHLSL(in_shaderPath, in_entryPoint, "vs_5_0", in_macro);
    CreateVertexShader_Internal(blob, in_inputLayout);
}

void VertexShader::LoadFromCSO(const std::filesystem::path& in_shaderPath, const Ref<InputLayout>& in_inputLayout)
{
    auto blob = d3d11::shader::LoadFromCSO(in_shaderPath);
    CreateVertexShader_Internal(blob, in_inputLayout);
}

void VertexShader::CompileCode(
    const std::string_view  in_shaderCode,
    const std::string_view  in_entryPoint,
    const ShaderMacros&     in_macro,
    const Ref<InputLayout>& in_inputLayout)
{
    auto blob = d3d11::shader::CompileShaderCode(in_shaderCode, in_entryPoint, "vs_5_0", in_macro);
    CreateVertexShader_Internal(blob, in_inputLayout);
}

void VertexShader::Bind() const
{
    GetRenderer().SetVertexShader(m_vertexShader.Get(), m_shaderID);
    m_inputLayout->Bind();
}

void VertexShader::CreateVertexShader_Internal(
    const ComPtr<ID3DBlob>& in_blob,
    const Ref<InputLayout>& in_inputLayout)
{
    auto d = GetRenderer().GetDevice();

    if (d3d11::CheckOK(d->CreateVertexShader(in_blob->GetBufferPointer(),
                                             in_blob->GetBufferSize(),
                                             nullptr,
                                             m_vertexShader.GetAddressOf()),
                       "CreateVertexShader Fail."))
    {
        m_inputLayout = in_inputLayout;
        m_shaderID    = s_idGenerator.GenerateID();
    }
}

//===================================================
// Pixel Shader
//===================================================

void PixelShader::LoadFromHLSL(
    const std::filesystem::path& in_shaderPath,
    const std::string_view       in_entryPoint,
    const ShaderMacros&          in_macro)
{
    auto blob = d3d11::shader::LoadFromHLSL(in_shaderPath, in_entryPoint, "ps_5_0", in_macro);
    CreatePixelShader_Internal(blob);
}

void PixelShader::LoadFromCSO(const std::filesystem::path& in_shaderPath)
{
    auto blob = d3d11::shader::LoadFromCSO(in_shaderPath);
    CreatePixelShader_Internal(blob);
}

void PixelShader::CompileCode(
    const std::string_view in_shaderCode,
    const std::string_view in_entryPoint,
    const ShaderMacros&    in_macro)
{
    auto blob = d3d11::shader::CompileShaderCode(in_shaderCode, in_entryPoint, "ps_5_0", in_macro);
    CreatePixelShader_Internal(blob);
}

void PixelShader::Bind() const
{
    GetRenderer().SetPixelShader(m_pixelShader.Get(), m_shaderID);
}

void PixelShader::CreatePixelShader_Internal(const ComPtr<ID3DBlob>& in_blob)
{
    auto d = GetRenderer().GetDevice();

    // - Create Pixel Shader
    d3d11::CheckOK(d->CreatePixelShader(in_blob->GetBufferPointer(),
                                        in_blob->GetBufferSize(),
                                        nullptr,
                                        m_pixelShader.GetAddressOf()),
                   "CreatePixelShader Fail.");

    m_shaderID = s_idGenerator.GenerateID();
}

//===================================================
// Geometry Shader
//===================================================

void GeometryShader::LoadFromHLSL(
    const std::filesystem::path& in_shaderPath,
    const std::string_view       in_entryPoint,
    const ShaderMacros&          in_macro)
{
    auto blob = d3d11::shader::LoadFromHLSL(in_shaderPath, in_entryPoint, "gs_5_0", in_macro);
    CreateGeometryShader_Internal(blob);
}

void GeometryShader::LoadFromCSO(const std::filesystem::path& in_shaderPath)
{
    auto blob = d3d11::shader::LoadFromCSO(in_shaderPath);
    CreateGeometryShader_Internal(blob);
}

void GeometryShader::CompileCode(
    const std::string_view in_shaderCode,
    const std::string_view in_entryPoint,
    const ShaderMacros&    in_macro)
{
    auto blob = d3d11::shader::CompileShaderCode(in_shaderCode, in_entryPoint, "gs_5_0", in_macro);
    CreateGeometryShader_Internal(blob);
}

void GeometryShader::Bind() const
{
    GetRenderer().SetGeometryShader(m_geometryShader.Get(), m_shaderID);
}

void GeometryShader::CreateGeometryShader_Internal(const ComPtr<ID3DBlob>& in_blob)
{
    auto d = GetRenderer().GetDevice();

    // - Create Geometry Shader
    d3d11::CheckOK(d->CreateGeometryShader(in_blob->GetBufferPointer(),
                                           in_blob->GetBufferSize(),
                                           nullptr,
                                           m_geometryShader.GetAddressOf()),
                   "CreateGeometryShader Fail.");

    m_shaderID = s_idGenerator.GenerateID();
}

//===================================================
// Demain Shader
//===================================================

void DomainShader::LoadFromHLSL(
    const std::filesystem::path& in_shaderPath,
    const std::string_view       in_entryPoint,
    const ShaderMacros&          in_macro)
{
    auto blob = d3d11::shader::LoadFromHLSL(in_shaderPath, in_entryPoint, "ds_5_0", in_macro);
    CreateDomainShader_Internal(blob);
}

void DomainShader::LoadFromCSO(const std::filesystem::path& in_shaderPath)
{
    auto blob = d3d11::shader::LoadFromCSO(in_shaderPath);
    CreateDomainShader_Internal(blob);
}

void DomainShader::CompileCode(
    const std::string_view in_shaderCode,
    const std::string_view in_entryPoint,
    const ShaderMacros&    in_macro)
{
    auto blob = d3d11::shader::CompileShaderCode(in_shaderCode, in_entryPoint, "ds_5_0", in_macro);
    CreateDomainShader_Internal(blob);
}

void DomainShader::Bind() const
{

    GetRenderer().SetDomainShader(m_domainShader.Get(), m_shaderID);
}

void DomainShader::CreateDomainShader_Internal(const ComPtr<ID3DBlob>& in_blob)
{
    auto d = GetRenderer().GetDevice();

    // - Create Domain Shader
    d3d11::CheckOK(d->CreateDomainShader(in_blob->GetBufferPointer(),
                                         in_blob->GetBufferSize(),
                                         nullptr,
                                         m_domainShader.GetAddressOf()),
                   "CreateDomainShader Fail.");

    m_shaderID = s_idGenerator.GenerateID();
}

//===================================================
// Demain Shader
//===================================================

void HullShader::LoadFromHLSL(
    const std::filesystem::path& in_shaderPath,
    const std::string_view       in_entryPoint,
    const ShaderMacros&          in_macro)
{
    auto blob = d3d11::shader::LoadFromHLSL(in_shaderPath, in_entryPoint, "ds_5_0", in_macro);
    CreateHullShader_Internal(blob);
}

void HullShader::LoadFromCSO(const std::filesystem::path& in_shaderPath)
{
    auto blob = d3d11::shader::LoadFromCSO(in_shaderPath);
    CreateHullShader_Internal(blob);
}

void HullShader::CompileCode(
    const std::string_view in_shaderCode,
    const std::string_view in_entryPoint,
    const ShaderMacros&    in_macro)
{
    auto blob = d3d11::shader::CompileShaderCode(in_shaderCode, in_entryPoint, "ds_5_0", in_macro);
    CreateHullShader_Internal(blob);
}

void HullShader::Bind() const
{
    GetRenderer().SetHullShader(m_hullShader.Get(), m_shaderID);
}

void HullShader::CreateHullShader_Internal(const ComPtr<ID3DBlob>& in_blob)
{
    auto d = GetRenderer().GetDevice();

    // - Create Hull Shader
    d3d11::CheckOK(d->CreateHullShader(in_blob->GetBufferPointer(),
                                       in_blob->GetBufferSize(),
                                       nullptr,
                                       m_hullShader.GetAddressOf()),
                   "CreateHullShader Fail.");

    m_shaderID = s_idGenerator.GenerateID();
}

//===================================================
// Compute Shader
//===================================================

void ComputeShader::LoadFromHLSL(
    const std::filesystem::path& in_shaderPath,
    const std::string_view       in_entryPoint,
    const ShaderMacros&          in_macro)
{
    auto blob = d3d11::shader::LoadFromHLSL(in_shaderPath, in_entryPoint, "cs_5_0", in_macro);
    CreateComputeShader_Internal(blob);
}

void ComputeShader::LoadFromCSO(const std::filesystem::path& in_shaderPath)
{
    auto blob = d3d11::shader::LoadFromCSO(in_shaderPath);
    CreateComputeShader_Internal(blob);
}

void ComputeShader::CompileCode(
    const std::string_view in_shaderCode,
    const std::string_view in_entryPoint,
    const ShaderMacros&    in_macro)
{
    auto blob = d3d11::shader::CompileShaderCode(in_shaderCode, in_entryPoint, "cs_5_0", ShaderMacros::EmptyMacro());
    CreateComputeShader_Internal(blob);
}

void ComputeShader::Dispatch(uint32 in_x, uint32 in_y, uint32 in_z) const
{
    GetRenderer().DispatchComputeShader(m_computeShader.Get(), in_x, in_y, in_z);
}

void ComputeShader::CreateComputeShader_Internal(const ComPtr<ID3DBlob>& in_blob)
{
    auto d = GetRenderer().GetDevice();

    // - Create Compute Shader
    d3d11::CheckOK(d->CreateComputeShader(in_blob->GetBufferPointer(),
                                          in_blob->GetBufferSize(),
                                          nullptr,
                                          m_computeShader.GetAddressOf()),
                   "CreateComputeShader Fail.");
}

}   // namespace crab