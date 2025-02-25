#pragma once

namespace crab
{

class ImguiVendor
{
public:
    static void Init();
    static void BeginRender();
    static void EndRender();
    static void Shutdown();
};

}   // namespace crab