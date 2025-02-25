#pragma once

// - Macros
#define NOMINMAX

// - STD
#include <algorithm>
#include <filesystem>
#include <memory>

// - STL
#include <array>
#include <string>
#include <string_view>
#include <vector>

// - DirectX
#include <DirectXTex.h>
#include <DirectXTex.inl>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <d3dcompiler.inl>
#include <directxtk/SimpleMath.h>
#include <wrl.h>
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "DirectXTK.lib")
#pragma comment(lib, "DirectXTex.lib")

// - SDL
#if defined(_DEBUG)
#    pragma comment(lib, "SDL2d.lib")
#    pragma comment(lib, "SDL2maind.lib")
#else
#    pragma comment(lib, "SDL2.lib")
#    pragma comment(lib, "SDL2main.lib")
#endif

// - imgui
#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_sdl2.h>
#include <imgui_internal.h>
#include <imgui_stdlib.h>

// - magic_enum
#include <magic_enum/magic_enum.hpp>

// - fmt
#include <fmt/core.h>

// - entt
#include <entt/entt.hpp>

// - Base
#include "CTTI.h"
#include "CrabColors.h"
#include "CrabMacro.h"
#include "CrabMath.h"
#include "CrabTypes.h"
#include "D11Enums.h"
#include "InitSettings.h"
#include "Log.h"
#include "ErrorHandler.h"

// - Data Structure
#include "SCircularQueue.h"

// - Application
#include "Application.h"
#include "EntryPoint.h"
