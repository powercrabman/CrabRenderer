#pragma once

#include "InputElements.h"

namespace crab
{

/*!
 * this support simple vertex types for 2D and 3D
 * if you need more complex vertex types, you should define your own vertex types
 * For this reason, there is no problem with handling your own vertices either.
 * This is just sample.
 */

struct VertexData
{
    Vec3 position  = Vec3::Zero;
    Vec3 normal    = Vec3::Zero;
    Vec2 texCoord  = Vec2::Zero;
    Vec3 tangent   = Vec3::Zero;
    Vec3 bitangent = Vec3::Zero;
    Vec3 color     = Vec3::Zero;
};

struct GeometryData
{
    std::vector<VertexData> subDatas;   // this is
    std::vector<uint32>     indices;
    eTopology               topology;
};

// This Vertex Is STANDARD
struct Vertex2D
{
    Vec2 position;
    Vec2 texCoord;

    inline static InputElements s_elements = []()
    {
        InputElements elements = {};
        elements
            .Add("POSITION", 0, eFormat::Float32_2)
            .Add("TEXCOORD", 0, eFormat::Float32_2);

        return elements;
    }();

    inline static Vertex2D CreateVertex(
        const VertexData& in_data)
    {
        Vertex2D vertex = {};
        vertex.position = Vec2(in_data.position.x, in_data.position.y);
        vertex.texCoord = in_data.texCoord;
        return vertex;
    }
};

// This Vertex Is STANDARD
struct Vertex3D
{
    Vec3 position;
    Vec3 normal;
    Vec2 texCoord;
    Vec3 tangent;

    inline static InputElements s_elements = []()
    {
        InputElements elements = {};
        elements
            .Add("POSITION", 0, eFormat::Float32_3)
            .Add("NORMAL", 0, eFormat::Float32_3)
            .Add("TEXCOORD", 0, eFormat::Float32_2)
            .Add("TANGENT", 0, eFormat::Float32_3);

        return elements;
    }();

    inline static Vertex3D CreateVertex(
        const VertexData& in_data)
    {
        Vertex3D vertex = {};
        vertex.position = in_data.position;
        vertex.normal   = in_data.normal;
        vertex.texCoord = in_data.texCoord;
        vertex.tangent  = in_data.tangent;
        return vertex;
    }
};

// This Vertex Is STANDARD 2
struct SkinnedVertex3D
{
    Vec3 position;
    Vec3 normal;
    Vec2 texCoord;
    Vec3 tangent;

    std::array<float, CRAB_SKIN_MAX> boneWeight;
    std::array<uint8, CRAB_SKIN_MAX> boneIndices;
};

}   // namespace crab