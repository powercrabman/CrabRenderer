#include "CrabPch.h"

#include "Shaders.h"

#include "D11Renderer.h"

namespace crab
{

//===================================================
// Vertex Shader
//===================================================

Ref<VertexShader> VertexShader::CreateFromHLSL(
    const std::filesystem::path& in_shaderPath,
    const std::string_view       in_entryPoint,
    const InputElements&         in_inputElems,
    const ShaderMacros&          in_macros)
{
    auto vs   = CreateRef<VertexShader>();
    auto blob = ShaderUtil::LoadShaderFile(in_shaderPath, in_entryPoint, "vs_5_0", in_macros);
    vs->_CreateVertexShader(blob, in_inputElems);

    return vs;
}

Ref<VertexShader> VertexShader::CreateFromCSO(
    const std::filesystem::path& in_shaderPath,
    const InputElements&         in_inputElems)
{
    auto vs   = CreateRef<VertexShader>();
    auto blob = ShaderUtil::LoadShaderFile(in_shaderPath, "", "");
    vs->_CreateVertexShader(blob, in_inputElems);

    return vs;
}

Ref<VertexShader> VertexShader::CreateFromCode(
    const std::string_view in_shaderCode,
    const std::string_view in_entryPoint,
    const InputElements&   in_inputElems,
    const ShaderMacros&    in_macros)
{
    auto vs   = CreateRef<VertexShader>();
    auto blob = ShaderUtil::LoadShaderCode(in_shaderCode, in_entryPoint, "vs_5_0", in_macros);
    vs->_CreateVertexShader(blob, in_inputElems);

    return vs;
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

Ref<PixelShader> PixelShader::CreateFromHLSL(
    const std::filesystem::path& in_shaderPath,
    const std::string_view       in_entryPoint,
    const ShaderMacros&          in_macro)
{
    auto ps   = CreateRef<PixelShader>();
    auto blob = ShaderUtil::LoadShaderFile(in_shaderPath, in_entryPoint, "ps_5_0", in_macro);
    ps->_CreatePixelShader(blob);

    return ps;
}

Ref<PixelShader> PixelShader::CreateFromCSO(const std::filesystem::path& in_shaderPath)
{
    auto ps   = CreateRef<PixelShader>();
    auto blob = ShaderUtil::LoadShaderFile(in_shaderPath, "", "");
    ps->_CreatePixelShader(blob);

    return ps;
}

Ref<PixelShader> PixelShader::CreateFromCode(
    const std::string_view in_shaderCode,
    const std::string_view in_entryPoint,
    const ShaderMacros&    in_macro)
{
    auto ps   = CreateRef<PixelShader>();
    auto blob = ShaderUtil::LoadShaderCode(in_shaderCode, in_entryPoint, "ps_5_0", in_macro);
    ps->_CreatePixelShader(blob);

    return ps;
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

Ref<GeometryShader> GeometryShader::CreateFromHLSL(
    const std::filesystem::path& in_shaderPath,
    const std::string_view       in_entryPoint,
    const ShaderMacros&          in_macro)
{
    auto gs   = CreateRef<GeometryShader>();
    auto blob = ShaderUtil::LoadShaderFile(in_shaderPath, in_entryPoint, "gs_5_0", in_macro);
    gs->_CreateGeometryShader(blob);

    return gs;
}

Ref<GeometryShader> GeometryShader::CreateFromCSO(const std::filesystem::path& in_shaderPath)
{
    auto gs   = CreateRef<GeometryShader>();
    auto blob = ShaderUtil::LoadShaderFile(in_shaderPath, "", "");
    gs->_CreateGeometryShader(blob);

    return gs;
}

Ref<GeometryShader> GeometryShader::CreateFromCode(
    const std::string_view in_shaderCode,
    const std::string_view in_entryPoint,
    const ShaderMacros&    in_macro)
{
    auto gs   = CreateRef<GeometryShader>();
    auto blob = ShaderUtil::LoadShaderCode(in_shaderCode, in_entryPoint, "gs_5_0", in_macro);
    gs->_CreateGeometryShader(blob);

    return gs;
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

Ref<DomainShader> DomainShader::CreateFromHLSL(
    const std::filesystem::path& in_shaderPath,
    const std::string_view       in_entryPoint,
    const ShaderMacros&          in_macro)
{
    auto gs   = CreateRef<DomainShader>();
    auto blob = ShaderUtil::LoadShaderFile(in_shaderPath, in_entryPoint, "ds_5_0", in_macro);
    gs->_CreateDomainShader(blob);

    return gs;
}

Ref<DomainShader> DomainShader::CreateFromCSO(const std::filesystem::path& in_shaderPath)
{
    auto gs   = CreateRef<DomainShader>();
    auto blob = ShaderUtil::LoadShaderFile(in_shaderPath, "", "ds_5_0");
    gs->_CreateDomainShader(blob);

    return gs;
}

Ref<DomainShader> DomainShader::CreateFromCode(
    const std::string_view in_shaderCode,
    const std::string_view in_entryPoint,
    const ShaderMacros&    in_macro)
{
    auto gs   = CreateRef<DomainShader>();
    auto blob = ShaderUtil::LoadShaderCode(in_shaderCode, in_entryPoint, "ds_5_0", in_macro);
    gs->_CreateDomainShader(blob);

    return gs;
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

Ref<HullShader> HullShader::CreateFromHLSL(
    const std::filesystem::path& in_shaderPath,
    const std::string_view       in_entryPoint,
    const ShaderMacros&          in_macro)
{
    auto gs   = CreateRef<HullShader>();
    auto blob = ShaderUtil::LoadShaderFile(in_shaderPath, in_entryPoint, "ds_5_0", in_macro);
    gs->_CreateHullShader(blob);

    return gs;
}

Ref<HullShader> HullShader::CreateFromCSO(const std::filesystem::path& in_shaderPath)
{
    auto gs   = CreateRef<HullShader>();
    auto blob = ShaderUtil::LoadShaderFile(in_shaderPath, "", "");
    gs->_CreateHullShader(blob);

    return gs;
}

Ref<HullShader> HullShader::CreateFromCode(
    const std::string_view in_shaderCode,
    const std::string_view in_entryPoint,
    const ShaderMacros&    in_macro)
{
    auto gs   = CreateRef<HullShader>();
    auto blob = ShaderUtil::LoadShaderCode(in_shaderCode, in_entryPoint, "ds_5_0", in_macro);
    gs->_CreateHullShader(blob);

    return gs;
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

Ref<ComputeShader> ComputeShader::CreateFromHLSL(
    const std::filesystem::path& in_shaderPath,
    const std::string_view       in_entryPoint,
    const ShaderMacros&          in_macro)
{
    auto gs   = CreateRef<ComputeShader>();
    auto blob = ShaderUtil::LoadShaderFile(in_shaderPath, in_entryPoint, "cs_5_0", in_macro);
    gs->_CreateComputeShader(blob);

    return gs;
}

Ref<ComputeShader> ComputeShader::CreateFromCSO(const std::filesystem::path& in_shaderPath)
{
    auto gs   = CreateRef<ComputeShader>();
    auto blob = ShaderUtil::LoadShaderFile(in_shaderPath, "", "");
    gs->_CreateComputeShader(blob);

    return gs;
}

Ref<ComputeShader> ComputeShader::CreateFromCode(
    const std::string_view in_shaderCode,
    const std::string_view in_entryPoint,
    const ShaderMacros&    in_macro)
{
    auto gs   = CreateRef<ComputeShader>();
    auto blob = ShaderUtil::LoadShaderCode(in_shaderCode, in_entryPoint, "cs_5_0", in_macro);
    gs->_CreateComputeShader(blob);

    return gs;
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