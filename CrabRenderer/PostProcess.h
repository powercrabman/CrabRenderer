#pragma once

#include "InputElements.h"

namespace crab
{

class Image2D;
class Mesh;
class ImageFilter;

struct PostProcessVertex
{
    Vec2 position;
    Vec2 texCoord;

    inline static InputElements s_inputElements = []()
    {
        InputElements elem;
        elem.Add("POSITION", 0, DXGI_FORMAT_R32G32_FLOAT)
            .Add("TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT);
        return elem;
    }();
};

class PostProcess
{
public:
    void AddFilter(const Ref<ImageFilter>& in_filter);

    Ref<ImageFilter> GetLastFilter() const;
    void             ClearFilterList();

    void Render();

private:
    Ref<Mesh>                     m_mesh;
    std::vector<Ref<ImageFilter>> m_filters;
};

}   // namespace crab