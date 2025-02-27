#include "CrabPch.h"

#include "GeometryFactory.h"

#include "Mesh.h"

#include <DirectXMesh.h>

namespace crab
{

GeometryData GeometryFactory::CreateQuad(float width, float height)
{
    float widthDiv2  = width * 0.5f;
    float heightDiv2 = height * 0.5f;

    GeometryData output = {};

    output.positions = {
        Vec3 { widthDiv2, heightDiv2, 0.f },
        Vec3 { -widthDiv2, heightDiv2, 0.f },
        Vec3 { -widthDiv2, -heightDiv2, 0.f },
        Vec3 { widthDiv2, -heightDiv2, 0.f },
    };

    output.texCoords = {
        Vec2 { 0.f, 0.f },
        Vec2 { 1.f, 0.f },
        Vec2 { 1.f, 1.f },
        Vec2 { 0.f, 1.f },
    };

    output.indices  = { 0, 1, 2, 0, 2, 3 };

    output.normals  = GeometryUtil::ComputeNormals(output.indices, output.positions);
    output.tangents = GeometryUtil::ComputeTangents(output.indices, output.positions, output.normals, output.texCoords);

    output.topology = eTopology::TriangleList;
    return output;
}

GeometryData GeometryFactory::CreateCircle(float radius, uint32 slices)
{
    GeometryData output = {};

    output.positions.reserve(slices + 1);
    output.texCoords.reserve(slices + 1);
    output.normals.reserve(slices + 1);
    output.tangents.reserve(slices + 1);
    output.bitangents.reserve(slices + 1);

    for (uint32 i = 0; i < slices; ++i)
    {
        float angle = PI_2 * i / static_cast<float>(slices);

        float cos, sin;
        FastCosSin(angle, cos, sin);

        float x = radius * cos;
        float y = radius * sin;

        output.positions.emplace_back(Vec3 { x, y, 0.f });
        output.texCoords.emplace_back(Vec2 { x, y });
    }

    output.positions.emplace_back(Vec3 { 0.f, 0.f, 0.f });
    output.texCoords.emplace_back(Vec2 { 0.f, 0.f });

    output.indices.reserve(slices * 3);
    for (uint32 i = 0; i < slices; ++i)
    {
        output.indices.emplace_back(slices);
        output.indices.emplace_back(i);
        output.indices.emplace_back((i + 1) % slices);
    }

    output.normals  = GeometryUtil::ComputeNormals(output.indices, output.positions);
    output.tangents = GeometryUtil::ComputeTangents(output.indices, output.positions, output.normals, output.texCoords);

    output.topology = eTopology::TriangleList;
    return output;
}

GeometryData GeometryFactory::CreateCube(float width, float height, float depth)
{
    float widthDiv2  = width * 0.5f;
    float heightDiv2 = height * 0.5f;
    float depthDiv2  = depth * 0.5f;

    GeometryData output = {};
    output.positions    = {
        // front
        Vec3 { -widthDiv2, heightDiv2, -depthDiv2 },
        Vec3 { widthDiv2, heightDiv2, -depthDiv2 },
        Vec3 { widthDiv2, -heightDiv2, -depthDiv2 },
        Vec3 { -widthDiv2, -heightDiv2, -depthDiv2 },

        // right
        Vec3 { widthDiv2, heightDiv2, -depthDiv2 },
        Vec3 { widthDiv2, heightDiv2, depthDiv2 },
        Vec3 { widthDiv2, -heightDiv2, depthDiv2 },
        Vec3 { widthDiv2, -heightDiv2, -depthDiv2 },

        // top
        Vec3 { -widthDiv2, heightDiv2, depthDiv2 },
        Vec3 { widthDiv2, heightDiv2, depthDiv2 },
        Vec3 { widthDiv2, heightDiv2, -depthDiv2 },
        Vec3 { -widthDiv2, heightDiv2, -depthDiv2 },

        // back
        Vec3 { widthDiv2, heightDiv2, depthDiv2 },
        Vec3 { -widthDiv2, heightDiv2, depthDiv2 },
        Vec3 { -widthDiv2, -heightDiv2, depthDiv2 },
        Vec3 { widthDiv2, -heightDiv2, depthDiv2 },

        // left
        Vec3 { -widthDiv2, heightDiv2, depthDiv2 },
        Vec3 { -widthDiv2, heightDiv2, -depthDiv2 },
        Vec3 { -widthDiv2, -heightDiv2, -depthDiv2 },
        Vec3 { -widthDiv2, -heightDiv2, depthDiv2 },

        // bottom
        Vec3 { -widthDiv2, -heightDiv2, -depthDiv2 },
        Vec3 { widthDiv2, -heightDiv2, -depthDiv2 },
        Vec3 { widthDiv2, -heightDiv2, depthDiv2 },
        Vec3 { -widthDiv2, -heightDiv2, depthDiv2 }
    };

    output.texCoords = {
        // front
        Vec2 { 0.f, 0.f },
        Vec2 { 1.f, 0.f },
        Vec2 { 1.f, 1.f },
        Vec2 { 0.f, 1.f },

        // right
        Vec2 { 0.f, 0.f },
        Vec2 { 1.f, 0.f },
        Vec2 { 1.f, 1.f },
        Vec2 { 0.f, 1.f },

        // top
        Vec2 { 0.f, 0.f },
        Vec2 { 1.f, 0.f },
        Vec2 { 1.f, 1.f },
        Vec2 { 0.f, 1.f },

        // back
        Vec2 { 0.f, 0.f },
        Vec2 { 1.f, 0.f },
        Vec2 { 1.f, 1.f },
        Vec2 { 0.f, 1.f },

        // left
        Vec2 { 0.f, 0.f },
        Vec2 { 1.f, 0.f },
        Vec2 { 1.f, 1.f },
        Vec2 { 0.f, 1.f },

        // bottom
        Vec2 { 0.f, 0.f },
        Vec2 { 1.f, 0.f },
        Vec2 { 1.f, 1.f },
        Vec2 { 0.f, 1.f },
    };

    // clang-format off
    output.indices = {
        // front
        0, 1, 2,
        0, 2, 3,

        // right
        4, 5, 6,
        4, 6, 7,

        // top
        8, 9, 10,
        8, 10, 11,

        // back
        12, 13, 14,
        12, 14, 15,

        // left
        16, 17, 18,
        16, 18, 19,

        // bottom
        20, 21, 22,
        20, 22, 23
    };
    // clang-format on

    output.normals  = GeometryUtil::ComputeNormals(output.indices, output.positions);
    output.tangents = GeometryUtil::ComputeTangents(output.indices, output.positions, output.normals, output.texCoords);

    output.topology = eTopology::TriangleList;
    return output;
}
GeometryData GeometryFactory::CreateSphere(float radius, uint32 slices, uint32 stacks)
{
    GeometryData output = {};
    output.positions.reserve((slices + 1) * (stacks + 1));
    output.texCoords.reserve((slices + 1) * (stacks + 1));
    output.normals.reserve((slices + 1) * (stacks + 1));
    output.tangents.reserve((slices + 1) * (stacks + 1));

    for (uint32 stack = 0; stack <= stacks; ++stack)
    {
        float phi = -PI * stack / static_cast<float>(stacks);

        float cosPhi, sinPhi;
        FastCosSin(phi, cosPhi, sinPhi);

        for (uint32 slice = 0; slice <= slices; ++slice)
        {
            float theta = -PI_2 * slice / static_cast<float>(slices);

            float cosTh, sinTh;
            FastCosSin(theta, cosTh, sinTh);

            float x = radius * sinPhi * cosTh;
            float y = radius * std::cos(phi);
            float z = radius * sinPhi * sinTh;

            Vec3 position = { x, y, z };
            Vec3 normal   = Normalize(position);
            Vec2 texCoord = {
                1.0f - static_cast<float>(slice) / static_cast<float>(slices),
                1.0f - static_cast<float>(stack) / static_cast<float>(stacks)
            };

            Vec3 tangent = { -sinTh, 0.f, cosTh };

            output.positions.push_back(position);
            output.texCoords.push_back(texCoord);
        }
    }

    output.indices.reserve(slices * stacks * 6);

    for (uint32 stack = 0; stack < stacks; ++stack)
    {
        for (uint32 slice = 0; slice < slices; ++slice)
        {
            uint32 first  = (stack * (slices + 1)) + slice;
            uint32 second = first + slices + 1;

            output.indices.push_back(first);
            output.indices.push_back(second);
            output.indices.push_back(first + 1);

            output.indices.push_back(second);
            output.indices.push_back(second + 1);
            output.indices.push_back(first + 1);
        }
    }

    output.normals  = GeometryUtil::ComputeNormals(output.indices, output.positions);
    output.tangents = GeometryUtil::ComputeTangents(output.indices, output.positions, output.normals, output.texCoords);

    output.topology = eTopology::TriangleList;
    return output;
}

GeometryData GeometryFactory::CreateGrid(float width, float height, uint32 repeatX, uint32 repeatY)
{
    float widthDiv2  = width * 0.5f;
    float heightDiv2 = height * 0.5f;

    GeometryData output = {};
    output.positions.reserve((repeatX + 1) * (repeatY + 1));
    output.texCoords.reserve((repeatX + 1) * (repeatY + 1));
    output.normals.reserve((repeatX + 1) * (repeatY + 1));
    output.tangents.reserve((repeatX + 1) * (repeatY + 1));

    for (uint32 y = 0; y <= repeatY; ++y)
    {
        float v = static_cast<float>(y) / static_cast<float>(repeatY);

        for (uint32 x = 0; x <= repeatX; ++x)
        {
            float u = static_cast<float>(x) / static_cast<float>(repeatX);

            float posX = -widthDiv2 + width * u;
            float posY = -heightDiv2 + height * v;

            output.positions.emplace_back(Vec3 { posX, posY, 0.f });
            output.texCoords.emplace_back(Vec2 { u, v });
        }
    }

    output.indices.reserve(repeatX * repeatY * 6);

    for (uint32 y = 0; y < repeatY; ++y)
    {
        for (uint32 x = 0; x < repeatX; ++x)
        {
            uint32 first  = (y * (repeatX + 1)) + x;
            uint32 second = first + repeatX + 1;

            output.indices.push_back(first);
            output.indices.push_back(second);
            output.indices.push_back(first + 1);

            output.indices.push_back(second);
            output.indices.push_back(second + 1);
            output.indices.push_back(first + 1);
        }
    }

    output.normals  = GeometryUtil::ComputeNormals(output.indices, output.positions);
    output.tangents = GeometryUtil::ComputeTangents(output.indices, output.positions, output.normals, output.texCoords);

    output.topology = eTopology::TriangleList;
    return output;
}

std::vector<Vec3> GeometryUtil::ComputeNormals(
    const std::vector<uint32>& in_indices,
    const std::vector<Vec3>&   in_positions)
{
    std::vector<Vec3> normals(in_positions.size(), Vec3::Zero);

    DirectX::ComputeNormals(
        in_indices.data(),
        in_indices.size() / 3,
        in_positions.data(),
        in_positions.size(),
        DirectX::CNORM_DEFAULT ,
        normals.data());

    return normals;
}

std::vector<Vec3> GeometryUtil::ComputeTangents(
    const std::vector<uint32>& in_indices,
    const std::vector<Vec3>&   in_positions,
    const std::vector<Vec3>&   in_normals,
    const std::vector<Vec2>&   in_texCoords)
{
    std::vector<Vec3> tangents(in_positions.size(), Vec3::Zero);
    std::vector<Vec3> bitangents(in_positions.size(), Vec3::Zero);   // this is

    DirectX::ComputeTangentFrame(
        in_indices.data(),
        in_indices.size() / 3,
        in_positions.data(),
        in_normals.data(),
        in_texCoords.data(),
        in_positions.size(),
        tangents.data(),
        bitangents.data());

    return tangents;
}

}   // namespace crab