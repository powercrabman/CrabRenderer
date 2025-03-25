#pragma once

namespace crab
{

using eApplicationCreateFlags = uint32;

enum eApplicationCreateFlags_
{
    eApplicationCreateFlags_None = 0,

    eApplicationCreateFlags_Window_CenterAlign = BIT(0),

    eApplicationCreateFlags_Renderer_EnableDepthBuffer        = BIT(1),
    eApplicationCreateFlags_Renderer_EnableHDR                = BIT(2),
    eApplicationCreateFlags_Renderer_EnableMSAA               = BIT(3),
    eApplicationCreateFlags_Renderer_EnableVSync              = BIT(4),
    eApplicationCreateFlags_Renderer_LoadDefaultShaderFromCSO = BIT(5),   // not implemented
};

struct ProjectDirectories
{
    std::filesystem::path projectDirectory;
    std::filesystem::path engineDirectory;
    std::filesystem::path shaderDirectory;
};

struct ApplicationCreateInfo
{

    std::string applicationName = "Crab Renderer";

    std::string windowTitle = "Crab Renderer";

    uint32 windowWidth  = 1280;
    uint32 windowHeight = 720;

    int32 windowPosX = 0;
    int32 windowPosY = 0;

    eApplicationCreateFlags flags = eApplicationCreateFlags_None;

    eFormat backBufferFormat            = eFormat::UNorm8_4;
    eFormat backBufferDepthBufferFormat = eFormat::Depth_UNorm24_Stencil_UInt8;
    eFormat HDRBackBufferFormat         = eFormat::Float16_4;

    ProjectDirectories projectDirectories;
};

struct AppWindowInitInfo
{
    std::string windowTitle = "Crab Renderer";

    uint32 windowWidth  = 1280;
    uint32 windowHeight = 720;

    int32 windowPosX = 0;
    int32 windowPosY = 0;

    bool centerAlign = false;
};

}   // namespace crab
