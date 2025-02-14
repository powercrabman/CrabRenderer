#pragma once

namespace crab
{

class D11Renderer;

class Renderer
{
public:
    // Core
    static void Init(const RendererSetting& in_setting);
    static void Shutdown();
    static void OnEvent(CrabEvent& in_event);

    static void* GetNativeRenderingAPI();

    // Render
    static void BeginRender();
    static void EndRender();
    static void Present();

private:
    static Scope<D11Renderer> m_renderer;
};

// Helper (Temp)

#define D11_API \
    static_cast<D11Renderer*>(Renderer::GetNativeRenderingAPI())

}   // namespace crab