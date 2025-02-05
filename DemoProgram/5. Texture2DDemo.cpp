#include "pch.h"

#include "5. Texture2DDemo.h"

#include "GUIs.h"

void Texture2DDemo::Init()
{
    m_mesh = MeshFactory2D::CreateQuad(Vec2 { 1.f, 1.f });
    m_tex  = Texture::Create(
        "images.png",
        D11_SamplerState::Create(eSamplerFilter::Linear, eSamplerAddress::Wrap));

    const char* shaderCode = R"(
        cbuffer CB_Transform : register(b0)
        {
            row_major float4x4 world;
        }

        cbuffer CB_Camera : register(b1)
        {
            row_major float4x4 viewProj;
        }

        struct VSInput
        {
            float2 position : POSITION;
            float2 texCoord : TEXCOORD;
            float4 color    : COLOR;
        };

        struct PSInput
        {
            float4 position : SV_POSITION;
            float2 texCoord : TEXCOORD;
            float4 color    : COLOR;
        };

        PSInput VSmain(VSInput input)
        {
            PSInput output;
            output.position = mul(float4(input.position, 0.f, 1.f), world);
            output.position = mul(output.position, viewProj);
            output.color = input.color;
            output.texCoord = input.texCoord;

            return output;
        }

        texture2D texSample : register(t0);
        sampler samplerState : register(s0);
        
        float4 PSmain(PSInput input) : SV_TARGET
        {
            float4 texColor = texSample.Sample(samplerState, input.texCoord);
            return texColor * input.color;
        }
    )";

    D11_InputElements elem;
    elem.Add("POSITION", 0, eDataFormat::Float2)
        .Add("TEXCOORD", 0, eDataFormat::Float2)
        .Add("COLOR", 0, eDataFormat::Float4);

    m_vs = D11_VertexShader::CreateFromString(shaderCode, "VSmain", elem);
    m_ps = D11_PixelShader::CreateFromString(shaderCode, "PSmain");

    // - Constant Buffer
    m_cbTransform = D11_ConstantBuffer<CB_Transform>::Create();
    m_cbCamera    = D11_ConstantBuffer<CB_Camera>::Create();
}

void Texture2DDemo::OnEnter()
{
    auto* dx = D11_API;
    _CreateRasterizerState();
    dx->SetConstantBuffer(m_cbCamera, 1, eShaderFlags_VertexShader);
    dx->SetConstantBuffer(m_cbTransform, 0, eShaderFlags_VertexShader);
}

void Texture2DDemo::OnExit()
{
}

void Texture2DDemo::OnUpdate(TimeStamp& in_ts)
{
}

void Texture2DDemo::OnRender(TimeStamp& in_ts)
{
    auto* dx = D11_API;
    dx->SetBackBufferToFrameBuffer();
    dx->ClearFrameBuffer(color::BLACK);
    dx->SetTopology(eTopology::TriangleList);

    dx->SetVertexBuffer(m_mesh->vertexBuffer);
    dx->SetIndexBuffer(m_mesh->indexBuffer);

    dx->SetVertexShader(m_vs);
    dx->SetPixelShader(m_ps);

    dx->SetTexture(m_tex, 0, eShaderFlags_PixelShader);

    // - Update Constant Buffer
    m_cbTransform->UpdateData({ m_transform.GetWorld() });

    // - Update Camera Buffer
    m_cbCamera->UpdateData({ m_cmr.GetViewProj(m_cmrTransform) });

    dx->DrawIndexed(m_mesh->indexBuffer->indexCount);
}

void Texture2DDemo::OnRenderGUI(TimeStamp& in_ts)
{
    DrawDemoInspector(GetName(),
                      [&]()
                      {
                          ImGui::SeparatorText("Transform");
                          DrawTransformInspector(m_transform);

                          ImGui::SeparatorText("Camera");
                          DrawCameraInspector(m_cmrTransform,
                                              m_cmr);

                          ImGui::SeparatorText("Rasterizer State");
                          if (ImGui::Checkbox("Wireframe", &m_wireframe))
                              m_rsDirty = true;

                          ImGui::SameLine();

                          if (ImGui::Checkbox("Cull Enable", &m_cullenable))
                              m_rsDirty = true;

                          if (m_rsDirty)
                          {
                              _CreateRasterizerState();
                              m_rsDirty = false;
                          }
                      });
}

void Texture2DDemo::OnEvent(CrabEvent& in_event)
{
}

void Texture2DDemo::_CreateRasterizerState()
{
    m_rsState = D11_RasterizerState::Create(
        m_cullenable ? eCullMode::Back : eCullMode::None,
        m_wireframe ? eFillMode::Wireframe : eFillMode::Solid,
        eFrontFace::ClockWise);

    D11_API->SetRasterizerState(m_rsState);
}
