#pragma once

#define NOMINMAX

// STD & STL
#include <filesystem>

// Crab Common
#include "CrabCommon.h"

// 3rd parties
#include <magic_enum/magic_enum.hpp>

// D3D
#include <d3d11.h>
#include <wrl.h>
using Microsoft::WRL::ComPtr;

// D3D Base
#include "D3D11ErrorHandler.h"
#include "RenderDefinition.h"
#include "RendererConfig.h"
#include "RenderResourceIDGenerator.h"

// D3D Utils
#include "D3D11Utils.h"

// D3D Resource
#include "Buffers.h"
#include "Viewport.h"
#include "CrabVertex.h"

// High Level Resource
#include "ShaderSharedData.h"
