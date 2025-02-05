#include "pch.h"

#include "3. ConstantBufferDemo.h"
#include "GUIs.h"

void ConstantBufferDemo::Init()
{
    std::vector<Vertex> vb = {
        Vertex { Vec2 { -0.5f, 0.f }, Vec4 { 1.f, 0.f, 0.f, 1.f } },
        Vertex { Vec2 { 0.f, 0.5f }, Vec4 { 0.f, 1.f, 0.f, 1.f } },
        Vertex { Vec2 { 0.5f, 0.f }, Vec4 { 0.f, 0.f, 1.f, 1.f } },
    };

    std::vector<uint32> ib = {
        0, 1, 2
    };

    m_vertexBuffer = D11_VertexBuffer::Create(vb);
    m_indexBuffer  = D11_IndexBuffer::Create(ib);

    const char* shaderCode = R"(
        cbuffer CB_Transform : register(b0)
        {
            row_major float4x4 world;
        }

        struct VSInput
        {
            float2 position : POSITION;
            float4 color    : COLOR;
        };

        struct PSInput
        {
            float4 position : SV_POSITION;
            float4 color    : COLOR;
        };

        PSInput VSmain(VSInput input)
        {
            PSInput output;
            output.position = mul(float4(input.position, 0.f, 1.f), world);
            output.color = input.color;
            return output;
        }
        
        float4 PSmain(PSInput input) : SV_TARGET
        {
            return input.color;
        }
    )";

    D11_InputElements elem;
    elem.Add("POSITION", 0, eDataFormat::Float2)
        .Add("COLOR", 0, eDataFormat::Float4);

    m_vs = D11_VertexShader::CreateFromString(shaderCode, "VSmain", elem);
    m_ps = D11_PixelShader::CreateFromString(shaderCode, "PSmain");

    // - Constant Buffer
    m_cbTransform = D11_ConstantBuffer<CB_Transform>::Create();
}

void ConstantBufferDemo::OnEnter()
{
    auto* dx = D11_API;
    dx->SetConstantBuffer(m_cbTransform, 0, eShaderFlags_VertexShader);
}

void ConstantBufferDemo::OnExit()
{
}

void ConstantBufferDemo::OnUpdate(TimeStamp& in_ts)
{
}

void ConstantBufferDemo::OnRender(TimeStamp& in_ts)
{
    auto* dx = D11_API;
    dx->SetBackBufferToFrameBuffer();
    dx->ClearFrameBuffer(color::BLACK);
    dx->SetTopology(eTopology::TriangleList);

    dx->SetVertexBuffer(m_vertexBuffer);
    dx->SetIndexBuffer(m_indexBuffer);

    dx->SetVertexShader(m_vs);
    dx->SetPixelShader(m_ps);

    m_cbTransform->UpdateData(CB_Transform{m_transform.GetWorld()});

    dx->DrawIndexed(m_indexBuffer->indexCount);
}

void ConstantBufferDemo::OnRenderGUI(TimeStamp& in_ts)
{
    DrawDemoInspector(GetName(),
                      [&]()
                      {
                          ImGui::SeparatorText("Transform");
                          DrawTransformInspector(m_transform);
                      });
}

void ConstantBufferDemo::OnEvent(CrabEvent& in_event)
{
}
