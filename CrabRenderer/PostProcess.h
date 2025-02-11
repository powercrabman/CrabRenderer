#pragma once

#include "D11_Inputlayout.h"

namespace crab
{

class D11_Texture2D;
class D11_Mesh;
class ImageFilter;

struct PostProcessVertex
{
    Vec2 position;
    Vec2 texCoord;

    inline static D11_InputElements s_inputElements = []()
    {
        D11_InputElements elem;
        elem.Add("POSITION", 0, DXGI_FORMAT_R32G32_FLOAT)
            .Add("TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT);
        return elem;
    }();
};

class PostProcess
{
public:
    static Ref<PostProcess> Create();

    void AddFilter(const Ref<ImageFilter>&   in_filter);

    Ref<ImageFilter> GetLastFilter() const;
    void             ClearFilterList();

    void Render();

private:
    Ref<D11_Mesh> m_mesh;

    std::vector<Ref<ImageFilter>> m_filters;
};

}   // namespace crab