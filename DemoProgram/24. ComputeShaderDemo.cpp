#include "pch.h"

#include "24. ComputeShaderDemo.h"

ComputeShaderDemo::ComputeShaderDemo()
{
}

ComputeShaderDemo::~ComputeShaderDemo()
{
}

void ComputeShaderDemo::Init()
{
    auto [width, height] = GetAppWindow().GetWindowSize();

    auto tex = ID3D11Texture2DUtil::CreateTexture2D(
        width,
        height,
        eFormat::Float16x4,
        D3D11_USAGE_DEFAULT,
        eBindFlags_ShaderResource | eBindFlags_UnorderedAccess,
        0);

    m_uav     = ID3D11UnorderedAccessViewUtil::CreateTexture2DUAV(tex.Get());
    m_texture = Texture2D::Create(tex.Get());

    struct Vertex
    {
        Vec2 position;
        Vec2 uv;
    };

    InputElements elems;
    elems
        .Add("POSITION", 0, eFormat::Float32x2)
        .Add("TEXCOORD", 0, eFormat::Float32x2);

    std::vector<Vertex> vertices = {
        Vertex { Vec2 { -1, 1 }, Vec2 { 0, 0 } },
        Vertex { Vec2 { 1, 1 }, Vec2 { 1, 0 } },
        Vertex { Vec2 { 1, -1 }, Vec2 { 1, 1 } },
        Vertex { Vec2 { -1, -1 }, Vec2 { 0, 1 } }
    };
    std::vector<uint32> indices = { 0, 1, 2, 0, 2, 3 };

    m_vb = VertexBuffer::Create(vertices);
    m_ib = IndexBuffer::Create(indices);

    m_vs = VertexShader::CreateFromHLSL("ComputeDemoVS.hlsl", "main", elems);
    m_ps = PixelShader::CreateFromHLSL("ComputeDemoPS.hlsl", "main");
    m_cs = ComputeShader::CreateFromHLSL("ComputeDemoCS.hlsl", "main");
}

void ComputeShaderDemo::OnEnter()
{
}

void ComputeShaderDemo::OnExit()
{
}

void ComputeShaderDemo::OnUpdate(TimeStamp& in_ts)
{
    if (Input::IsKeyPressed(eKey::Escape))
        GetApplication().Quit();
}

void ComputeShaderDemo::OnRender(TimeStamp& in_ts)
{
    auto* dx = D11_API;

    dx->BindBackBuffer(dx->GetDepthBuffer());
    dx->ClearBackBuffer(color::BLACK);
    dx->ClearDepthBuffer(true, 1, true, 0);

    m_cs->Bind();
    dx->SetUnorderedAccessView(m_uav.Get(), 0);

    auto [width, height] = GetAppWindow().GetWindowSize();
    dx->ComputeShaderDispatch(
        static_cast<uint32>(std::ceilf(width / 32.f)),
        static_cast<uint32>(std::ceilf(height / 32.f)),
        1);

    dx->ReleaseUnorderedAccessViews();

    m_texture->Bind(0, eShaderFlags_PixelShader);

    dx->SetTopology(eTopology::TriangleList);
    m_vs->Bind();
    m_ps->Bind();

    m_vb->Bind();
    m_ib->Bind();

    GetRenderStateStorage()->LinearWrap()->Bind(0, eShaderFlags_PixelShader);

    dx->DrawIndexed(m_ib->GetIndexCount(), 0, 0);

    dx->SetShaderResourceView(nullptr, 0, eShaderFlags_PixelShader);
}

void ComputeShaderDemo::OnRenderGUI(TimeStamp& in_ts)
{
}

void ComputeShaderDemo::OnEvent(CrabEvent& in_event)
{
}