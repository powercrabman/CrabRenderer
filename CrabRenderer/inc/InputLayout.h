#pragma once

namespace crab
{

//===================================================
// Input Elements
//===================================================

struct InputElement
{
    std::string_view semantic;
    uint32           semanticIndex;
    eFormat          format;
};

//===================================================
// InputLayout
//===================================================

class InputLayout
{
public:
    void Init(const std::vector<InputElement>& in_inputElements);

    void Bind() const;

private:
    ComPtr<ID3D11InputLayout> m_inputLayout;

    static std::string_view s_shaderCode;

    inline static RenderResourceIDGenerator s_idGenerator;
    InputLayoutID                           m_id = 0;
};

//===================================================
// InputLayout Builder
//===================================================

class InputLayoutBuilder
{
public:
    InputLayoutBuilder& AddElement(std::string_view in_semantic,
                                   uint32           in_semanticIndex,
                                   eFormat          in_format);

    Ref<InputLayout> Build() const;

    void Reset();

private:
    std::vector<InputElement> m_elems;
};

}   // namespace crab