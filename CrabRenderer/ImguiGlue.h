#pragma once

namespace crab
{

class ImguiGlue
{
public:
    static void Init(
        SDL_Window*          in_sdlWindow,
        ID3D11Device*        in_device,
        ID3D11DeviceContext* in_context);

    static void BeginRender();
    static void EndRender();
    static void Shutdown();
};

}   // namespace crab