#pragma once
#include "D11Enums.h"
#include "InputElements.h"

namespace crab
{

/*!
 * this support simple vertex types for 2D and 3D
 * if you need more complex vertex types, you should define your own vertex types
 * For this reason, there is no problem with handling your own vertices either.
 * This is just sample.
 */

struct GeometryData
{
    std::vector<Vec3> positions;
    std::vector<Vec3> normals;
    std::vector<Vec2> texCoords;
    std::vector<Vec3> tangents;
    std::vector<Vec3> colors;

    std::vector<uint32> indices;
    eTopology           topology;
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
            .Add("POSITION", 0, eFormat::Float32x2)
            .Add("TEXCOORD", 0, eFormat::Float32x2);

        return elements;
    }();

    static std::vector<Vertex2D> CreateVertices(
        const std::vector<Vec2>& in_positions,
        const std::vector<Vec2>& in_texCoords)
    {
        std::vector<Vertex2D> vertices;
        vertices.reserve(in_positions.size());
        for (uint32 i = 0; i < in_positions.size(); i++)
        {
            Vertex2D vertex;
            vertex.position = in_positions[i];
            vertex.texCoord = in_texCoords[i];
            vertices.push_back(vertex);
        }
        return vertices;
    }

    static std::vector<Vertex2D> CreateVertices(
        const std::vector<Vec3>& in_positions,
        const std::vector<Vec2>& in_texCoords)
    {
        std::vector<Vertex2D> vertices;
        vertices.reserve(in_positions.size());
        for (uint32 i = 0; i < in_positions.size(); i++)
        {
            Vertex2D vertex;
            vertex.position = Vec2(in_positions[i].x, in_positions[i].y);
            vertex.texCoord = in_texCoords[i];
            vertices.push_back(vertex);
        }
        return vertices;
    }

    static std::vector<Vertex2D> CreateVertices(
        const GeometryData& in_geoData)
    {
        std::vector<Vertex2D> vertices;
        vertices.reserve(in_geoData.positions.size());
        for (uint32 i = 0; i < in_geoData.positions.size(); i++)
        {
            Vertex2D vertex;
            vertex.position = Vec2(in_geoData.positions[i].x, in_geoData.positions[i].y);
            vertex.texCoord = in_geoData.texCoords[i];
            vertices.push_back(vertex);
        }
        return vertices;
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
            .Add("POSITION", 0, eFormat::Float32x3)
            .Add("NORMAL", 0, eFormat::Float32x3)
            .Add("TEXCOORD", 0, eFormat::Float32x2)
            .Add("TANGENT", 0, eFormat::Float32x3);

        return elements;
    }();

    static std::vector<Vertex3D> CreateVertices(
        const std::vector<Vec3>& in_positions,
        const std::vector<Vec3>& in_normals,
        const std::vector<Vec2>& in_texCoords,
        const std::vector<Vec3>& in_tangents)
    {
        std::vector<Vertex3D> vertices;
        vertices.reserve(in_positions.size());
        for (uint32 i = 0; i < in_positions.size(); i++)
        {
            Vertex3D vertex;
            vertex.position = in_positions[i];
            vertex.normal   = in_normals[i];
            vertex.texCoord = in_texCoords[i];
            vertex.tangent  = in_tangents[i];
            vertices.push_back(vertex);
        }
        return vertices;
    }

    static std::vector<Vertex3D> CreateVertices(
        const GeometryData& in_geoData)
    {
        std::vector<Vertex3D> vertices;
        vertices.reserve(in_geoData.positions.size());
        for (uint32 i = 0; i < in_geoData.positions.size(); i++)
        {
            Vertex3D vertex;
            vertex.position = in_geoData.positions[i];
            vertex.normal   = in_geoData.normals[i];
            vertex.texCoord = in_geoData.texCoords[i];
            vertex.tangent  = in_geoData.tangents[i];
            vertices.push_back(vertex);
        }
        return vertices;
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

    static std::vector<SkinnedVertex3D> CreateVertices(
        const std::vector<Vec3>&                             in_positions,
        const std::vector<Vec3>&                             in_normals,
        const std::vector<Vec2>&                             in_texCoords,
        const std::vector<Vec3>&                             in_tangents,
        const std::vector<std::array<float, CRAB_SKIN_MAX>>& in_boneWeights,
        const std::vector<std::array<uint8, CRAB_SKIN_MAX>>& in_boneIndices)
    {
        std::vector<SkinnedVertex3D> vertices;
        vertices.reserve(in_positions.size());
        for (uint32 i = 0; i < in_positions.size(); i++)
        {
            SkinnedVertex3D vertex;
            vertex.position    = in_positions[i];
            vertex.normal      = in_normals[i];
            vertex.texCoord    = in_texCoords[i];
            vertex.tangent     = in_tangents[i];
            vertex.boneWeight  = in_boneWeights[i];
            vertex.boneIndices = in_boneIndices[i];
            vertices.push_back(vertex);
        }
        return vertices;
    }

    static std::vector<SkinnedVertex3D> CreateVertices(
        const GeometryData& in_geoData)
    {
        std::vector<SkinnedVertex3D> vertices;
        vertices.reserve(in_geoData.positions.size());
        for (uint32 i = 0; i < in_geoData.positions.size(); i++)
        {
            SkinnedVertex3D vertex;
            vertex.position    = in_geoData.positions[i];
            vertex.normal      = in_geoData.normals[i];
            vertex.texCoord    = in_geoData.texCoords[i];
            vertex.tangent     = in_geoData.tangents[i];
            vertex.boneWeight  = { 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f };
            vertex.boneIndices = { 0, 0, 0, 0, 0, 0, 0, 0 };
            vertices.push_back(vertex);
        }
        return vertices;
    }
};

}   // namespace crab