#pragma once

namespace crab
{

class RenderTarget;

class Snapshot
{
public:
    Snapshot();
    ~Snapshot();

    void Capture(const Ref<RenderTarget>& in_rt);

    // you should contain filename and extension
    void Save(const std::filesystem::path& in_path);

    Vec4 GetPixelColorFloat(const Int2& in_pos) const;
    Int4 GetPixelColorUInt(const Int2& in_pos) const;

private:
    ComPtr<ID3D11Texture2D> m_texture;
    DXGI_FORMAT             m_format;
    uint32                  m_width;
    uint32                  m_height;
};

}   // namespace crab
