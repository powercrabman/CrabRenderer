#pragma once

namespace crab
{

class ConstantBufferBase;
class InputLayout;

//===================================================
// ShaderMacros
//===================================================

class ShaderMacros
{
public:
    struct Macro
    {
        std::string name;
        std::string value;
    };

    ShaderMacros()                    = default;
    ShaderMacros(const ShaderMacros&) = default;
    ShaderMacros(ShaderMacros&&)      = default;
    ShaderMacros(const std::initializer_list<Macro>& in_macros)
        : m_macros(in_macros)
    {
    }

    void ClearList()
    {
        m_macros.clear();
    }

    ShaderMacros& Add(const std::string_view in_name, const std::string_view in_value)
    {
        m_macros.emplace_back(std::string(in_name), std::string(in_value));
        return *this;
    }

    std::vector<D3D_SHADER_MACRO> Get() const
    {
        if (m_macros.empty())
            return {};

        std::vector<D3D_SHADER_MACRO> output;
        output.reserve(m_macros.size() + 1);

        for (const auto& macro: m_macros)
            output.push_back({ macro.name.c_str(), macro.value.c_str() });

        output.push_back({ nullptr, nullptr });

        return output;
    }

    static ShaderMacros EmptyMacro() { return ShaderMacros {}; }

private:
    std::vector<Macro> m_macros;
};

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
                      const ShaderMacros&          in_macro,
                      const Ref<InputLayout>&      in_inputLayout);

    void LoadFromCSO(const std::filesystem::path& in_shaderPath,
                     const Ref<InputLayout>&      in_inputLayout);

    void CompileCode(const std::string_view  in_shaderCode,
                     const std::string_view  in_entryPoint,
                     const ShaderMacros&     in_macro,
                     const Ref<InputLayout>& in_inputLayout);

    //===================================================
    // Member
    //===================================================

    void Bind() const;

    Ref<InputLayout> GetInputLayout() const { return m_inputLayout; }

    static constexpr eShaderStage GetType() { return eShaderStage::VertexShader; }

private:
    void CreateVertexShader_Internal(
        const ComPtr<ID3DBlob>& in_blob,
        const Ref<InputLayout>& in_inputLayout);

    Ref<InputLayout>           m_inputLayout;
    ComPtr<ID3D11VertexShader> m_vertexShader;
    ComPtr<ID3DBlob>           m_blob;

    inline static RenderResourceIDGenerator s_idGenerator;
    VertexShaderID                          m_shaderID = 0;
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

    void CompileCode(const std::string_view in_shaderCode,
                     const std::string_view in_entryPoint,
                     const ShaderMacros&    in_macro = {});

    //===================================================
    // Member
    //===================================================

    static constexpr eShaderStage GetType() { return eShaderStage::PixelShader; }

    void Bind() const;

private:
    void CreatePixelShader_Internal(const ComPtr<ID3DBlob>& in_blob);

    ComPtr<ID3D11PixelShader> m_pixelShader;
    ComPtr<ID3DBlob>          m_blob;

    inline static RenderResourceIDGenerator s_idGenerator;
    PixelShaderID                           m_shaderID = 0;
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

    void CompileCode(const std::string_view in_shaderCode,
                     const std::string_view in_entryPoint,
                     const ShaderMacros&    in_macro = {});

    //===================================================
    // Member
    //===================================================
    static constexpr eShaderStage GetType() { return eShaderStage::GeometryShader; }

    void Bind() const;

private:
    void CreateGeometryShader_Internal(const ComPtr<ID3DBlob>& in_blob);

    ComPtr<ID3D11GeometryShader> m_geometryShader;
    ComPtr<ID3DBlob>             m_blob;

    inline static RenderResourceIDGenerator s_idGenerator;
    GeometryShaderID                        m_shaderID = 0;
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

    void CompileCode(const std::string_view in_shaderCode,
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
    void CreateDomainShader_Internal(const ComPtr<ID3DBlob>& in_blob);

    ComPtr<ID3D11DomainShader> m_domainShader;
    ComPtr<ID3DBlob>           m_blob;

    inline static RenderResourceIDGenerator s_idGenerator;
    DomainShaderID                          m_shaderID = 0;
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

    void CompileCode(const std::string_view in_shaderCode,
                     const std::string_view in_entryPoint,
                     const ShaderMacros&    in_macro = {});

    //===================================================
    // Member
    //===================================================
    static constexpr eShaderStage GetType() { return eShaderStage::HullShader; }

    void Bind() const;

private:
    void CreateHullShader_Internal(const ComPtr<ID3DBlob>& in_blob);

    ComPtr<ID3D11HullShader> m_hullShader;
    ComPtr<ID3DBlob>         m_blob;

    inline static RenderResourceIDGenerator s_idGenerator;
    HullShaderID                            m_shaderID = 0;
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

    void CompileCode(const std::string_view in_shaderCode,
                     const std::string_view in_entryPoint,
                     const ShaderMacros&    in_macro = {});

    //===================================================
    // Member
    //===================================================
    static constexpr eShaderStage GetType() { return eShaderStage::ComputeShader; }

    void Dispatch(uint32 in_x, uint32 in_y, uint32 in_z) const;

private:
    void CreateComputeShader_Internal(const ComPtr<ID3DBlob>& in_blob);

    ComPtr<ID3D11ComputeShader> m_computeShader;
    ComPtr<ID3DBlob>            m_blob;
};

}   // namespace crab
