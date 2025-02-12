#pragma once
namespace crab
{

class D11_Texture2D;
class D11_DepthBuffer;

//===================================================
// RenderTarget
//===================================================

class D11_RenderTarget
{
public:
    // - Factory
    static Ref<D11_RenderTarget> Create(ID3D11Texture2D* in_texture);

    void Bind() const;
    void Bind(const Ref<D11_DepthBuffer>& in_depthBuffer) const;

    void Clear(const Color& in_color) const;

    ID3D11RenderTargetView* Get() const { return m_renderTargetView.Get(); }
    Ref<D11_Texture2D>      GetTexture() const { return m_texture; }
    eD11_Format             GetFormat() const { return m_format; }

private:
    ComPtr<ID3D11RenderTargetView> m_renderTargetView;
    Ref<D11_Texture2D>             m_texture;
    eD11_Format                    m_format = eD11_Format::Unknown;
};

//===================================================
// RenderTargetList
//===================================================

class D11_RenderTargetList
{
public:
    D11_RenderTargetList()  = default;
    ~D11_RenderTargetList() = default;

    void                  ClearList();
    D11_RenderTargetList& Add(Ref<D11_RenderTarget> in_renderTarget);

    Ref<D11_RenderTarget> GetRenderTarget(uint32 in_index) const;

    void Bind() const;
    void Bind(const Ref<D11_DepthBuffer>& in_depthBuffer) const;

private:
    std::vector<Ref<D11_RenderTarget>>   m_rtvs;
    std::vector<ID3D11RenderTargetView*> m_renderTargetViews;
};

}   // namespace crab