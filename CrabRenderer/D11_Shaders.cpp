#include "CrabPch.h"

#include "D11_Shaders.h"

#include "D11_Renderer.h"

namespace crab
{

//===================================================
// Vertex Shader
//===================================================

Ref<D11_VertexShader> D11_VertexShader::CreateFromFile(const std::filesystem::path& in_shaderPath,
                                                       const std::string_view       in_entryPoint,
                                                       const D11_InputElements&     in_inputElems)
{
    auto                  d  = D11_API->GetDevice();
    Ref<D11_VertexShader> vs = CreateRef<D11_VertexShader>();

    uint32 compileFlags = 0;
#ifdef _DEBUG
    compileFlags |= D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
    compileFlags |= D3DCOMPILE_OPTIMIZATION_LEVEL3;
#endif

    // - Compile Shader
    ComPtr<ID3DBlob> errorBlob;
    D11_ASSERT(D3DCompileFromFile(in_shaderPath.c_str(),
                                  nullptr,
                                  D3D_COMPILE_STANDARD_FILE_INCLUDE,
                                  in_entryPoint.data(),
                                  "vs_5_0",
                                  compileFlags,
                                  0,
                                  vs->blob.GetAddressOf(),
                                  errorBlob.GetAddressOf()),
               "D3DCompileFromFile Fail.");

    if (errorBlob)
    {
        CRAB_DEBUG_BREAK_V("Error Blob: {0}", (char*)errorBlob->GetBufferPointer());
        return nullptr;
    }

    // - Create Vertex Shader
    D11_ASSERT(d->CreateVertexShader(vs->blob->GetBufferPointer(),
                                     vs->blob->GetBufferSize(),
                                     nullptr,
                                     vs->vertexShader.GetAddressOf()),
               "CreateVertexShader Fail.");

    // - Create Input Layout
    D11_ASSERT(d->CreateInputLayout(in_inputElems.elements.data(),
                                    (uint32)in_inputElems.elements.size(),
                                    vs->blob->GetBufferPointer(),
                                    vs->blob->GetBufferSize(),
                                    vs->inputLayout.GetAddressOf()),
               "CreateInputLayout Fail.");
    return vs;
}

Ref<D11_VertexShader> D11_VertexShader::CreateFromString(const std::string_view in_shaderCode, const std::string_view in_entryPoint, const D11_InputElements& in_inputElems)
{
    auto                  d  = D11_API->GetDevice();
    Ref<D11_VertexShader> vs = CreateRef<D11_VertexShader>();

    uint32 compileFlags = 0;
#ifdef _DEBUG
    compileFlags |= D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
    compileFlags |= D3DCOMPILE_OPTIMIZATION_LEVEL3;
#endif

    // - Compile Shader
    ComPtr<ID3DBlob> errorBlob;
    D11_ASSERT(D3DCompile(in_shaderCode.data(),
                          in_shaderCode.size(),
                          nullptr,
                          nullptr,
                          D3D_COMPILE_STANDARD_FILE_INCLUDE,
                          in_entryPoint.data(),
                          "vs_5_0",
                          compileFlags,
                          0,
                          vs->blob.GetAddressOf(),
                          errorBlob.GetAddressOf()),
               "D3DCompile Fail.");

    if (errorBlob)
    {
        CRAB_DEBUG_BREAK_V("Error Blob: {0}", (char*)errorBlob->GetBufferPointer());
        return nullptr;
    }

    // - Create Vertex Shader
    D11_ASSERT(d->CreateVertexShader(vs->blob->GetBufferPointer(),
                                     vs->blob->GetBufferSize(),
                                     nullptr,
                                     vs->vertexShader.GetAddressOf()),
               "CreateVertexShader Fail.");

    // - Create Input Layout
    D11_ASSERT(d->CreateInputLayout(in_inputElems.elements.data(),
                                    (uint32)in_inputElems.elements.size(),
                                    vs->blob->GetBufferPointer(),
                                    vs->blob->GetBufferSize(),
                                    vs->inputLayout.GetAddressOf()),
               "CreateInputLayout Fail.");
    return vs;
}

//===================================================
// Pixel Shader
//===================================================

Ref<D11_PixelShader> D11_PixelShader::CreateFromFile(const std::filesystem::path& in_shaderPath,
                                                     const std::string_view       in_entryPoint)
{
    auto                 d  = D11_API->GetDevice();
    Ref<D11_PixelShader> ps = CreateRef<D11_PixelShader>();

    uint32 compileFlags = 0;
#ifdef _DEBUG
    compileFlags |= D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
    compileFlags |= D3DCOMPILE_OPTIMIZATION_LEVEL3;
#endif

    // - Compile Shader
    ComPtr<ID3DBlob> errorBlob;
    D11_ASSERT(D3DCompileFromFile(in_shaderPath.c_str(),
                                  nullptr,
                                  D3D_COMPILE_STANDARD_FILE_INCLUDE,
                                  in_entryPoint.data(),
                                  "ps_5_0",
                                  compileFlags,
                                  0,
                                  ps->blob.GetAddressOf(),
                                  errorBlob.GetAddressOf()),
               "D3DCompileFromFile Fail.");

    if (errorBlob)
    {
        CRAB_DEBUG_BREAK_V("Error Blob: {0}", (char*)errorBlob->GetBufferPointer());
        return nullptr;
    }

    // - Create Pixel Shader
    D11_ASSERT(d->CreatePixelShader(ps->blob->GetBufferPointer(),
                                    ps->blob->GetBufferSize(),
                                    nullptr,
                                    ps->pixelShader.GetAddressOf()),
               "CreatePixelShader Fail.");
    return ps;
}

crab::Ref<crab::D11_PixelShader> D11_PixelShader::CreateFromString(const std::string_view in_shaderCode, const std::string_view in_entryPoint)
{
    auto                 d  = D11_API->GetDevice();
    Ref<D11_PixelShader> ps = CreateRef<D11_PixelShader>();

    uint32 compileFlags = 0;
#ifdef _DEBUG
    compileFlags |= D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
    compileFlags |= D3DCOMPILE_OPTIMIZATION_LEVEL3;
#endif

    // - Compile Shader
    ComPtr<ID3DBlob> errorBlob;
    D11_ASSERT(D3DCompile(in_shaderCode.data(),
                          in_shaderCode.size(),
                          nullptr,
                          nullptr,
                          D3D_COMPILE_STANDARD_FILE_INCLUDE,
                          in_entryPoint.data(),
                          "ps_5_0",
                          compileFlags,
                          0,
                          ps->blob.GetAddressOf(),
                          errorBlob.GetAddressOf()),
               "D3DCompile Fail.");

    if (errorBlob)
    {
        CRAB_DEBUG_BREAK_V("Error Blob: {0}", (char*)errorBlob->GetBufferPointer());
        return nullptr;
    }
    // - Create Pixel Shader
    D11_ASSERT(d->CreatePixelShader(ps->blob->GetBufferPointer(),
                                    ps->blob->GetBufferSize(),
                                    nullptr,
                                    ps->pixelShader.GetAddressOf()),
               "CreatePixelShader Fail.");
    return ps;
}

//===================================================
// Compute Shader
//===================================================

Ref<D11_ComputeShader> D11_ComputeShader::CreateFromFile(const std::filesystem::path& in_shaderPath, const std::string_view in_entryPoint)
{
    auto                   d  = D11_API->GetDevice();
    Ref<D11_ComputeShader> cs = CreateRef<D11_ComputeShader>();

    uint32 compileFlags = 0;
#ifdef _DEBUG
    compileFlags |= D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
    compileFlags |= D3DCOMPILE_OPTIMIZATION_LEVEL3;
#endif

    // - Compile Shader
    ComPtr<ID3DBlob> errorBlob;
    D11_ASSERT(D3DCompileFromFile(in_shaderPath.c_str(),
                                  nullptr,
                                  D3D_COMPILE_STANDARD_FILE_INCLUDE,
                                  in_entryPoint.data(),
                                  "cs_5_0",
                                  compileFlags,
                                  0,
                                  cs->blob.GetAddressOf(),
                                  errorBlob.GetAddressOf()),
               "D3DCompileFromFile Fail.");

    if (errorBlob)
    {
        CRAB_DEBUG_BREAK_V("Error Blob: {0}", (char*)errorBlob->GetBufferPointer());
        return nullptr;
    }

    // - Create Compute Shader
    D11_ASSERT(d->CreateComputeShader(cs->blob->GetBufferPointer(),
                                      cs->blob->GetBufferSize(),
                                      nullptr,
                                      cs->computeShader.GetAddressOf()),
               "CreateComputeShader Fail.");
    return cs;
}

Ref<D11_ComputeShader> D11_ComputeShader::CreateFromString(const std::string_view in_shaderCode, const std::string_view in_entryPoint)
{
    auto                   d  = D11_API->GetDevice();
    Ref<D11_ComputeShader> cs = CreateRef<D11_ComputeShader>();

    uint32 compileFlags = 0;
#ifdef _DEBUG
    compileFlags |= D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
    compileFlags |= D3DCOMPILE_OPTIMIZATION_LEVEL3;
#endif

    // - Compile Shader
    ComPtr<ID3DBlob> errorBlob;
    D11_ASSERT(D3DCompile(in_shaderCode.data(),
                          in_shaderCode.size(),
                          nullptr,
                          nullptr,
                          D3D_COMPILE_STANDARD_FILE_INCLUDE,
                          in_entryPoint.data(),
                          "cs_5_0",
                          compileFlags,
                          0,
                          cs->blob.GetAddressOf(),
                          errorBlob.GetAddressOf()),
               "D3DCompile Fail.");

    if (errorBlob)
    {
        CRAB_DEBUG_BREAK_V("Error Blob: {0}", (char*)errorBlob->GetBufferPointer());
        return nullptr;
    }
    // - Create Compute Shader
    D11_ASSERT(d->CreateComputeShader(cs->blob->GetBufferPointer(),
                                      cs->blob->GetBufferSize(),
                                      nullptr,
                                      cs->computeShader.GetAddressOf()),
               "CreateComputeShader Fail.");
    return cs;
}

crab::Ref<crab::D11_GeometryShader> D11_GeometryShader::CreateFromFile(const std::filesystem::path& in_shaderPath, const std::string_view in_entryPoint)
{
    auto                    d            = D11_API->GetDevice();
    Ref<D11_GeometryShader> gs           = CreateRef<D11_GeometryShader>();
    uint32                  compileFlags = 0;

#ifdef _DEBUG
    compileFlags |= D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
    compileFlags |= D3DCOMPILE_OPTIMIZATION_LEVEL3;
#endif

    // - Compile Shader
    ComPtr<ID3DBlob> errorBlob;
    D11_ASSERT(D3DCompileFromFile(in_shaderPath.c_str(),
                                  nullptr,
                                  D3D_COMPILE_STANDARD_FILE_INCLUDE,
                                  in_entryPoint.data(),
                                  "gs_5_0",
                                  compileFlags,
                                  0,
                                  gs->blob.GetAddressOf(),
                                  errorBlob.GetAddressOf()),
               "D3DCompileFromFile Fail.");

    if (errorBlob)
    {
        CRAB_DEBUG_BREAK_V("Error Blob: {0}", (char*)errorBlob->GetBufferPointer());
        return nullptr;
    }

    // - Create Geometry Shader
    D11_ASSERT(d->CreateGeometryShader(gs->blob->GetBufferPointer(),
                                       gs->blob->GetBufferSize(),
                                       nullptr,
                                       gs->geometryShader.GetAddressOf()),
               "CreateGeometryShader Fail.");
    return gs;
}

crab::Ref<crab::D11_GeometryShader> D11_GeometryShader::CreateFromString(const std::string_view in_shaderCode, const std::string_view in_entryPoint)
{
    auto                    d            = D11_API->GetDevice();
    Ref<D11_GeometryShader> gs           = CreateRef<D11_GeometryShader>();
    uint32                  compileFlags = 0;

#ifdef _DEBUG
    compileFlags |= D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
    compileFlags |= D3DCOMPILE_OPTIMIZATION_LEVEL3;
#endif

    // - Compile Shader
    ComPtr<ID3DBlob> errorBlob;
    D11_ASSERT(D3DCompile(in_shaderCode.data(),
                          in_shaderCode.size(),
                          nullptr,
                          nullptr,
                          D3D_COMPILE_STANDARD_FILE_INCLUDE,
                          in_entryPoint.data(),
                          "gs_5_0",
                          compileFlags,
                          0,
                          gs->blob.GetAddressOf(),
                          errorBlob.GetAddressOf()),
               "D3DCompile Fail.");

    if (errorBlob)
    {
        CRAB_DEBUG_BREAK_V("Error Blob: {0}", (char*)errorBlob->GetBufferPointer());
        return nullptr;
    }
    // - Create Geometry Shader
    D11_ASSERT(d->CreateGeometryShader(gs->blob->GetBufferPointer(),
                                       gs->blob->GetBufferSize(),
                                       nullptr,
                                       gs->geometryShader.GetAddressOf()),
               "CreateGeometryShader Fail.");
    return gs;
}

}   // namespace crab