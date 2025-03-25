#pragma once

namespace crab
{

struct Viewport
{
    Viewport();
    Viewport(float in_width, float in_height);
    Viewport(uint32 in_width, uint32 in_height);
    Viewport(int32 in_width, int32 in_height);

    Viewport(float in_x,
             float in_y,
             float in_width,
             float in_height,
             float in_minDepth,
             float in_maxDepth);

    static Viewport CreateFullScreen();

    bool operator==(const Viewport& in_other) const
    {
        return ::memcmp(this, &in_other, sizeof(Viewport)) == 0;
    }

    bool operator!=(const Viewport& in_other) const
    {
        return !(*this == in_other);
    }

    void Bind() const;

    NODISCARD const D3D11_VIEWPORT* GetD3D11Viewport() const;

    float x;
    float y;
    float width;
    float height;
    float minDepth;
    float maxDepth;
};

static_assert(sizeof(Viewport) == sizeof(D3D11_VIEWPORT), "Viewport size mismatch");

}   // namespace crab