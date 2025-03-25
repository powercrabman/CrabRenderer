#include "InternalPCH.h"

#include "InputLayout.h"

#include "RenderFactory.h"
#include "Renderer.h"
#include "Shaders.h"

namespace crab
{

std::string_view InputLayout::s_shaderCode = R"(
        float4 main(VS_INPUT input) : SV_POSITION
        {
            return float4(0.f,0.f,0.f,0.f);
        }
    )";

//===================================================

void InputLayout::Init(const std::vector<InputElement>& in_inputElements)
{
    std::vector<D3D11_INPUT_ELEMENT_DESC> inputElements;
    inputElements.reserve(in_inputElements.size());

    std::stringstream ss;

    ss << "struct VS_INPUT\n"
          "{\n";

    for (uint32 i = 0; i < in_inputElements.size(); ++i)
    {
        const InputElement& elem = in_inputElements[i];
        inputElements.emplace_back(
            elem.semantic.data(),
            elem.semanticIndex,
            d3d11::GetFormat(elem.format),
            0,
            D3D11_APPEND_ALIGNED_ELEMENT,
            D3D11_INPUT_PER_VERTEX_DATA,
            0);

        ss << d3d11::ConvertHLSLFormatString(elem.format);
        ss << " element" << i << " : ";
        ss << elem.semantic.data();
        ss << elem.semanticIndex;
        ss << ";\n";
    }

    ss << "};\n";

    ss << s_shaderCode;

    ComPtr<ID3DBlob> shaderBlob =
        d3d11::shader::CompileShaderCode(ss.str(), "main", "vs_5_0", ShaderMacros::EmptyMacro());

    if (d3d11::CheckFail(GetRenderer().GetDevice()->CreateInputLayout(
                             inputElements.data(),
                             static_cast<uint32>(in_inputElements.size()),
                             shaderBlob->GetBufferPointer(),
                             shaderBlob->GetBufferSize(),
                             m_inputLayout.GetAddressOf()),
                         "CreateInputLayout Fail."))
    {
        DEBUG_BREAK(" CreateInputLayout Fail.");
    }

    m_id = s_idGenerator.GenerateID();
}

void InputLayout::Bind() const
{
    GetRenderer().SetInputLayout(m_inputLayout.Get(), m_id);
}

InputLayoutBuilder& InputLayoutBuilder::AddElement(
    std::string_view in_semantic,
    uint32           in_semanticIndex,
    eFormat          in_format)
{
    m_elems.emplace_back(InputElement {
        .semantic      = in_semantic,
        .semanticIndex = in_semanticIndex,
        .format        = in_format });

    return *this;
}

Ref<InputLayout> InputLayoutBuilder::Build() const
{
    Ref<InputLayout> layout = CreateRef<InputLayout>();
    layout->Init(m_elems);
    return layout;
}

void InputLayoutBuilder::Reset()
{
    m_elems.clear();
}

}   // namespace crab