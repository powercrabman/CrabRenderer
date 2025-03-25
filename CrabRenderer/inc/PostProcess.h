#pragma once

namespace crab
{
class ImageFilterBase;

class PostProcess
{
public:
    void AddFilter(const Ref<ImageFilterBase>& in_filter);

    Ref<ImageFilterBase> GetLastFilter() const;
    void             ClearFilterList();

    void Render() const;

private:
    std::vector<Ref<ImageFilterBase>> m_filters;
};

}   // namespace crab