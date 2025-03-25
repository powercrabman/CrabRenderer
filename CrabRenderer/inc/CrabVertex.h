#pragma once

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

    inline static Vertex2D CreateVertex(const VertexData& in_data)
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

    inline static Vertex3D CreateVertex(const VertexData& in_data)
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

    std::array<float, SKIN_COUNT> boneWeight;
    std::array<eBlendWriteFlags, SKIN_COUNT> boneIndices;
};

}   // namespace crab