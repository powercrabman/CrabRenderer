#pragma once

#define NOMINMAX

// STD & STL
#include <filesystem>

// Crab Common
#include "CrabCommon.h"

// 3rd parties
#include <magic_enum/magic_enum.hpp>
#include <imgui.h>

// D3D
#include <d3d11.h>
#include <wrl.h>
using Microsoft::WRL::ComPtr;
#pragma comment(lib, "d3d11.lib")

// D3D Base
#include "inc/D3D11ErrorHandler.h"
#include "inc/RenderDefinition.h"
#include "inc/RendererConfig.h"

// D3D Utils
#include "inc/D3D11Utils.h"
#include "inc/RenderResourceIDGenerator.h"

// D3D Resource
#include "inc/Buffers.h"
#include "inc/CrabVertex.h"
#include "inc/DepthBuffer.h"
#include "inc/InputLayout.h"
#include "inc/RenderStates.h"
#include "inc/RenderTarget.h"
#include "inc/ShaderResource.h"
#include "inc/Shaders.h"
#include "inc/SwapChain.h"
#include "inc/Viewport.h"
#include "inc/FrameBuffer.h"

// Helper
#include "inc/GeometryFactory.h"
#include "inc/RenderFactory.h"
#include "inc/RenderResourceNodes.h"
#include "inc/ShaderSharedData.h"
#include "inc/TextureLoader.h"

// Pipeline
#include "inc/DebugNormalPipeline.h"
#include "inc/PBRGeometryPipeline.h"
#include "inc/SkyboxPipeline.h"

// Post Process
#include "inc/BlurDownFilter.h"
#include "inc/BlurUpFilter.h"
#include "inc/CombineFilter.h"
#include "inc/PostProcess.h"
#include "inc/SamplingFilter.h"
#include "inc/ToneMapFilter.h"

// Global
#include "inc/GlobalConstants.h"
#include "inc/GlobalShader.h"
#include "inc/GlobalState.h"

// High Level
#include "inc/Draw2D.h"
#include "inc/ImguiGlue.h"
#include "inc/Material.h"
#include "inc/Mesh.h"
#include "inc/Model.h"
#include "inc/RayCast.h"
#include "inc/Snapshot.h"

// Renderer
#include "inc/Renderer.h"
