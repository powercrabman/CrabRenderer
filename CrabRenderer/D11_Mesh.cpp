#include "CrabPch.h"

#include "D11_Mesh.h"

#include "D11_Buffers.h"
#include "D11_Model.h"
#include "D11_Renderer.h"

namespace crab
{

Ref<D11_Mesh> D11_Mesh::Create(const Ref<D11_VertexBuffer>& in_vb,
                               const Ref<D11_IndexBuffer>&  in_ib)
{
    auto mesh            = CreateRef<D11_Mesh>();
    mesh->m_vertexBuffer = in_vb;
    mesh->m_indexBuffer  = in_ib;
    return mesh;
}

void D11_Mesh::Draw() const
{
    auto* dx = D11_API;
    m_vertexBuffer->Bind();
    m_indexBuffer->Bind();
    dx->DrawIndexed(m_indexBuffer->GetIndexCount());
}

Ref<D11_Mesh> MeshFactory2D::CreateQuad(const Vec2& in_size, const TexCoordArray& in_texCoord, const ColorArray& in_color)
{
    auto [wDiv2, hDiv2] = in_size * 0.5f;

    std::vector<Vertex> vertices = {
        Vertex { Vec2 { -wDiv2, hDiv2 }, in_texCoord[0], in_color[0] },
        Vertex { Vec2 { wDiv2, hDiv2 }, in_texCoord[1], in_color[1] },
        Vertex { Vec2 { wDiv2, -hDiv2 }, in_texCoord[2], in_color[2] },
        Vertex { Vec2 { -wDiv2, -hDiv2 }, in_texCoord[3], in_color[3] }
    };

    std::vector<uint32> indices = {
        0, 1, 2, 0, 2, 3
    };

    auto vb = D11_VertexBuffer::Create(vertices);
    auto ib = D11_IndexBuffer::Create(indices);
    return D11_Mesh::Create(vb, ib);
}

Ref<D11_Mesh> MeshFactory2D::CreateQuad(const Vec2& in_size, const TexCoordArray& in_texCoord)
{
    return CreateQuad(in_size,
                      in_texCoord,
                      ColorArray {
                          Vec4 { 1.f, 1.f, 1.f, 1.f },
                          Vec4 { 1.f, 1.f, 1.f, 1.f },
                          Vec4 { 1.f, 1.f, 1.f, 1.f },
                          Vec4 { 1.f, 1.f, 1.f, 1.f } });
}

Ref<D11_Mesh> MeshFactory2D::CreateQuad(const Vec2& in_size, const ColorArray& in_color)
{
    return CreateQuad(in_size,
                      TexCoordArray {
                          Vec2 { 0.f, 0.f },
                          Vec2 { 1.f, 0.f },
                          Vec2 { 1.f, 1.f },
                          Vec2 { 0.f, 1.f } },
                      in_color);
}

Ref<D11_Mesh> MeshFactory2D::CreateQuad(const Vec2& in_size)
{
    TexCoordArray texCoord = {
        Vec2 { 0.f, 0.f },
        Vec2 { 1.f, 0.f },
        Vec2 { 1.f, 1.f },
        Vec2 { 0.f, 1.f }
    };

    ColorArray color = {
        Vec4 { 1.f, 1.f, 1.f, 1.f },
        Vec4 { 1.f, 1.f, 1.f, 1.f },
        Vec4 { 1.f, 1.f, 1.f, 1.f },
        Vec4 { 1.f, 1.f, 1.f, 1.f }
    };

    return CreateQuad(in_size, texCoord, color);
}

Ref<D11_Mesh> MeshFactory2D::CreateQuad(const Vec2& in_size, const Color& in_color)
{
    return CreateQuad(in_size,
                      TexCoordArray {
                          Vec2 { 0.f, 0.f },
                          Vec2 { 1.f, 0.f },
                          Vec2 { 1.f, 1.f },
                          Vec2 { 0.f, 1.f } },
                      ColorArray { in_color, in_color, in_color, in_color });
}

Ref<D11_Mesh> MeshFactory2D::CreateCircle(float in_radius, uint32 in_slices)
{
    std::vector<Vertex> vertices;
    std::vector<uint32> indices;
    vertices.push_back(Vertex { Vec2 { 0.f, 0.f }, Vec2 { 0.5f, 0.5f }, Vec4 { 1.f, 1.f, 1.f, 1.f } });
    float angle = PI_2 / in_slices;
    for (uint32 i = 0; i < in_slices; ++i)
    {
        float cos, sin;
        FastCosSin(angle * i, cos, sin);
        float x = in_radius * cos;
        float y = in_radius * sin;
        vertices.push_back(Vertex { Vec2 { x, y }, Vec2 { 0.5f + x / in_radius * 0.5f, 0.5f + y / in_radius * 0.5f }, Vec4 { 1.f, 1.f, 1.f, 1.f } });
    }
    for (uint32 i = 1; i < in_slices; ++i)
    {
        indices.push_back(0);
        indices.push_back(i);
        indices.push_back(i + 1);
    }
    indices.push_back(0);
    indices.push_back(in_slices);
    indices.push_back(1);
    auto vb = D11_VertexBuffer::Create(vertices);
    auto ib = D11_IndexBuffer::Create(indices);
    return D11_Mesh::Create(vb, ib);
}

MeshData<MeshFactory3D::Vertex> MeshFactory3D::CreateCubeMeshData(const Vec3& in_size, const CubeTexCoordArray& in_texCoord, const CubeColorArray& in_color)
{
    float wDiv2 = in_size.x * 0.5f;
    float hDiv2 = in_size.y * 0.5f;
    float dDiv2 = in_size.z * 0.5f;

    std::vector<Vertex> vertices = {
        // front
        Vertex { Vec3 { -wDiv2, hDiv2, -dDiv2 }, { 0.f, 0.f, -1.f }, in_texCoord[0], in_color[0] },
        Vertex { Vec3 { wDiv2, hDiv2, -dDiv2 }, { 0.f, 0.f, -1.f }, in_texCoord[1], in_color[1] },
        Vertex { Vec3 { wDiv2, -hDiv2, -dDiv2 }, { 0.f, 0.f, -1.f }, in_texCoord[2], in_color[2] },
        Vertex { Vec3 { -wDiv2, -hDiv2, -dDiv2 }, { 0.f, 0.f, -1.f }, in_texCoord[3], in_color[3] },

        // right
        Vertex { Vec3 { wDiv2, hDiv2, -dDiv2 }, { 1.f, 0.f, 0.f }, in_texCoord[4], in_color[4] },
        Vertex { Vec3 { wDiv2, hDiv2, dDiv2 }, { 1.f, 0.f, 0.f }, in_texCoord[5], in_color[5] },
        Vertex { Vec3 { wDiv2, -hDiv2, dDiv2 }, { 1.f, 0.f, 0.f }, in_texCoord[6], in_color[6] },
        Vertex { Vec3 { wDiv2, -hDiv2, -dDiv2 }, { 1.f, 0.f, 0.f }, in_texCoord[7], in_color[7] },

        // top
        Vertex { Vec3 { -wDiv2, hDiv2, dDiv2 }, { 0.f, 1.f, 0.f }, in_texCoord[8], in_color[8] },
        Vertex { Vec3 { wDiv2, hDiv2, dDiv2 }, { 0.f, 1.f, 0.f }, in_texCoord[9], in_color[9] },
        Vertex { Vec3 { wDiv2, hDiv2, -dDiv2 }, { 0.f, 1.f, 0.f }, in_texCoord[10], in_color[10] },
        Vertex { Vec3 { -wDiv2, hDiv2, -dDiv2 }, { 0.f, 1.f, 0.f }, in_texCoord[11], in_color[11] },

        // back
        Vertex { Vec3 { wDiv2, hDiv2, dDiv2 }, { 0.f, 0.f, 1.f }, in_texCoord[12], in_color[12] },
        Vertex { Vec3 { -wDiv2, hDiv2, dDiv2 }, { 0.f, 0.f, 1.f }, in_texCoord[13], in_color[13] },
        Vertex { Vec3 { -wDiv2, -hDiv2, dDiv2 }, { 0.f, 0.f, 1.f }, in_texCoord[14], in_color[14] },
        Vertex { Vec3 { wDiv2, -hDiv2, dDiv2 }, { 0.f, 0.f, 1.f }, in_texCoord[15], in_color[15] },

        // left
        Vertex { Vec3 { -wDiv2, hDiv2, dDiv2 }, { -1.f, 0.f, 0.f }, in_texCoord[16], in_color[16] },
        Vertex { Vec3 { -wDiv2, hDiv2, -dDiv2 }, { -1.f, 0.f, 0.f }, in_texCoord[17], in_color[17] },
        Vertex { Vec3 { -wDiv2, -hDiv2, -dDiv2 }, { -1.f, 0.f, 0.f }, in_texCoord[18], in_color[18] },
        Vertex { Vec3 { -wDiv2, -hDiv2, dDiv2 }, { -1.f, 0.f, 0.f }, in_texCoord[19], in_color[19] },

        // bottom
        Vertex { Vec3 { -wDiv2, -hDiv2, -dDiv2 }, { 0.f, -1.f, 0.f }, in_texCoord[20], in_color[20] },
        Vertex { Vec3 { wDiv2, -hDiv2, -dDiv2 }, { 0.f, -1.f, 0.f }, in_texCoord[21], in_color[21] },
        Vertex { Vec3 { wDiv2, -hDiv2, dDiv2 }, { 0.f, -1.f, 0.f }, in_texCoord[22], in_color[22] },
        Vertex { Vec3 { -wDiv2, -hDiv2, dDiv2 }, { 0.f, -1.f, 0.f }, in_texCoord[23], in_color[23] }
    };

    // clang-format off
    std::vector<uint32> indices = {
        // front
        0, 1, 2, 0, 2, 3,
        // right
        4, 5, 6, 4, 6, 7,
        // top
        8, 9, 10, 8, 10, 11,
        // back
        12, 13, 14, 12, 14, 15,
        // left
        16, 17, 18, 16, 18, 19,
        // bottom
        20, 21, 22, 20, 22, 23
    };
    // clang-format on

    return MeshData<Vertex> { vertices, indices };
}

MeshData<MeshFactory3D::Vertex> MeshFactory3D::CreateCubeMeshData(const Vec3& in_size)
{
    CubeTexCoordArray texCoord;

    for (size_t i = 0; i < texCoord.size() / 4; i++)
    {
        texCoord[i * 4]     = Vec2 { 0.f, 0.f };
        texCoord[i * 4 + 1] = Vec2 { 1.f, 0.f };
        texCoord[i * 4 + 2] = Vec2 { 1.f, 1.f };
        texCoord[i * 4 + 3] = Vec2 { 0.f, 1.f };
    }

    CubeColorArray colors;

    for (size_t i = 0; i < colors.size(); i++)
        colors[i] = Vec4 { 1.f, 1.f, 1.f, 1.f };

    return CreateCubeMeshData(in_size, texCoord, colors);
}

Ref<D11_Mesh> MeshFactory3D::CreateCube(const Vec3& in_size, const CubeTexCoordArray& in_texCoord, const CubeColorArray& in_color)
{
    return D11_Mesh::Create(CreateCubeMeshData(in_size, in_texCoord, in_color));
}   // namespace crab

Ref<D11_Mesh> MeshFactory3D::CreateCube(const Vec3& in_size, const CubeTexCoordArray& in_texCoord)
{
    CubeColorArray colors;
    for (size_t i = 0; i < colors.size(); i++)
        colors[i] = Vec4 { 1.f, 1.f, 1.f, 1.f };

    return CreateCube(in_size, in_texCoord, colors);
}

Ref<D11_Mesh> MeshFactory3D::CreateCube(const Vec3& in_size, const CubeColorArray& in_color)
{
    CubeTexCoordArray texCoord;

    for (size_t i = 0; i < texCoord.size() / 4; i++)
    {
        texCoord[i * 4]     = Vec2 { 0.f, 0.f };
        texCoord[i * 4 + 1] = Vec2 { 1.f, 0.f };
        texCoord[i * 4 + 2] = Vec2 { 1.f, 1.f };
        texCoord[i * 4 + 3] = Vec2 { 0.f, 1.f };
    }

    return CreateCube(in_size, texCoord, in_color);
}

Ref<D11_Mesh> MeshFactory3D::CreateCube(const Vec3& in_size, const Color& in_color)
{
    CubeTexCoordArray texCoord;
    for (size_t i = 0; i < texCoord.size() / 4; i++)
    {
        texCoord[i * 4]     = Vec2 { 0.f, 0.f };
        texCoord[i * 4 + 1] = Vec2 { 1.f, 0.f };
        texCoord[i * 4 + 2] = Vec2 { 1.f, 1.f };
        texCoord[i * 4 + 3] = Vec2 { 0.f, 1.f };
    }

    CubeColorArray colors;
    for (size_t i = 0; i < colors.size(); i++)
        colors[i] = in_color;

    return CreateCube(in_size, texCoord, colors);
}

Ref<D11_Mesh> MeshFactory3D::CreateCube(const Vec3& in_size)
{
    CubeTexCoordArray texCoord;
    for (size_t i = 0; i < texCoord.size() / 4; i++)
    {
        texCoord[i * 4]     = Vec2 { 0.f, 0.f };
        texCoord[i * 4 + 1] = Vec2 { 1.f, 0.f };
        texCoord[i * 4 + 2] = Vec2 { 1.f, 1.f };
        texCoord[i * 4 + 3] = Vec2 { 0.f, 1.f };
    }
    CubeColorArray colors;
    for (size_t i = 0; i < colors.size(); i++)
        colors[i] = Vec4 { 1.f, 1.f, 1.f, 1.f };

    return CreateCube(in_size, texCoord, colors);
}

Ref<crab::D11_Mesh> MeshFactory3D::CreateCubeMap(const Vec3& in_size)
{
    auto meshData = CreateCubeMeshData(in_size);
    std::reverse(meshData.m_indices.begin(), meshData.m_indices.end());
    return D11_Mesh::Create(meshData);
}

Ref<D11_Mesh> MeshFactory3D::CreateCylinder(float in_radius, float in_height, uint32 in_slices, uint32 in_stacks)
{
    std::vector<Vertex> vertices;
    std::vector<uint32> indices;

    // Create vertices
    for (uint32 i = 0; i <= in_stacks; i++)
    {
        float stackHeight = in_height * i / in_stacks;
        float stackRadius = in_radius - stackHeight;
        for (uint32 j = 0; j <= in_slices; j++)
        {
            float phi = j * PI_2 / in_slices;
            float cos, sin;
            FastCosSin(phi, cos, sin);

            float x        = stackRadius * cos;
            float y        = stackHeight;
            float z        = stackRadius * sin;
            Vec3  position = Vec3 { x, y, z };
            Vec3  normal   = Normalize(position);
            Vec2  texCoord = Vec2 { static_cast<float>(j) / in_slices, static_cast<float>(i) / in_stacks };
            Vec4  color    = Vec4 { 1.f, 1.f, 1.f, 1.f };
            vertices.push_back(Vertex { position, normal, texCoord, Vec4 { 1.f, 1.f, 1.f, 1.f } });
        }
    }

    // Create indices
    for (uint32 i = 0; i < in_stacks; i++)
    {
        for (uint32 j = 0; j < in_slices; j++)
        {
            uint32 v0 = i * (in_slices + 1) + j;
            uint32 v1 = v0 + 1;
            uint32 v2 = (i + 1) * (in_slices + 1) + j;
            uint32 v3 = v2 + 1;
            indices.push_back(v0);
            indices.push_back(v2);
            indices.push_back(v1);
            indices.push_back(v1);
            indices.push_back(v2);
            indices.push_back(v3);
        }
    }

    auto vb = D11_VertexBuffer::Create(vertices);
    auto ib = D11_IndexBuffer::Create(indices);
    return D11_Mesh::Create(vb, ib);
}

MeshData<MeshFactory3D::Vertex> MeshFactory3D::CreateSphereMeshData(float in_radius, uint32 in_slices, uint32 in_stacks)
{
    std::vector<Vertex> vertices;
    std::vector<uint32> indices;

    // Create vertices
    for (uint32 i = 0; i <= in_stacks; i++)
    {
        float theta = i * PI / in_stacks;
        float cos, sin;
        FastCosSin(theta, cos, sin);
        float y = in_radius * cos;
        float r = in_radius * sin;

        for (uint32 j = 0; j <= in_slices; j++)
        {
            float phi = j * PI_2 / in_slices;
            float x, z;
            FastCosSin(phi, x, z);
            x *= r;
            z *= r;
            Vec3 position = Vec3 { x, y, z };
            Vec3 normal   = Normalize(position);
            Vec2 texCoord = Vec2 { static_cast<float>(j) / in_slices, static_cast<float>(i) / in_stacks };
            Vec4 color    = Vec4 { 1.f, 1.f, 1.f, 1.f };
            vertices.push_back(Vertex { position, normal, texCoord, Vec4 { 1.f, 1.f, 1.f, 1.f } });
        }
    }

    // Create indices (Clockwise order)
    for (uint32 i = 0; i < in_stacks; i++)
    {
        for (uint32 j = 0; j < in_slices; j++)
        {
            uint32 v0 = i * (in_slices + 1) + j;
            uint32 v1 = v0 + 1;
            uint32 v2 = (i + 1) * (in_slices + 1) + j;
            uint32 v3 = v2 + 1;

            indices.push_back(v0);
            indices.push_back(v1);
            indices.push_back(v2);

            indices.push_back(v1);
            indices.push_back(v3);
            indices.push_back(v2);
        }
    }

    return MeshData<Vertex> { vertices, indices };
}

Ref<D11_Mesh> MeshFactory3D::CreateSphere(float in_radius, uint32 in_slices, uint32 in_stacks)
{
    return D11_Mesh::Create(CreateSphereMeshData(in_radius, in_slices, in_stacks));
}

Ref<D11_Mesh> MeshFactory3D::CreateSphereMap(float in_radius, uint32 in_slices, uint32 in_stacks)
{
    auto meshData = CreateSphereMeshData(in_radius, in_slices, in_stacks);
    std::reverse(meshData.m_indices.begin(), meshData.m_indices.end());
    return D11_Mesh::Create(meshData);
}

Ref<D11_Mesh> MeshFactory3D::CreateCone(float in_radius, float in_height, uint32 in_slices)
{
    std::vector<Vertex> vertices;
    std::vector<uint32> indices;

    // Create vertices
    for (uint32 i = 0; i <= in_slices; i++)
    {
        float phi = i * PI_2 / in_slices;
        float cos, sin;
        FastCosSin(phi, cos, sin);
        float x        = in_radius * cos;
        float z        = in_radius * sin;
        Vec3  position = Vec3 { x, 0.f, z };
        Vec3  normal   = Normalize({ x, in_radius, z });
        Vec2  texCoord = Vec2 { static_cast<float>(i) / in_slices, 0.f };
        Vec4  color    = Vec4 { 1.f, 1.f, 1.f, 1.f };
        vertices.push_back(Vertex { position, normal, texCoord, Vec4 { 1.f, 1.f, 1.f, 1.f } });
    }
    vertices.push_back(Vertex { Vec3 { 0.f, in_height, 0.f }, Normalize({ 0.f, in_radius, 0.f }), Vec2 { 0.5f, 1.f }, Vec4 { 1.f, 1.f, 1.f, 1.f } });

    // Create indices
    for (uint32 i = 0; i < in_slices; i++)
    {
        indices.push_back(i);
        indices.push_back(i + 1);
        indices.push_back(in_slices);
    }

    auto vb = D11_VertexBuffer::Create(vertices);
    auto ib = D11_IndexBuffer::Create(indices);
    return D11_Mesh::Create(vb, ib);
}

Ref<D11_Mesh> MeshFactory3D::CreateTorus(float in_radius, float in_tubeRadius, uint32 in_slices, uint32 in_stacks)
{
    std::vector<Vertex> vertices;
    std::vector<uint32> indices;

    // Create vertices
    for (uint32 i = 0; i <= in_stacks; i++)
    {
        float theta    = i * PI_2 / in_stacks;
        float cosTheta = cos(theta);
        float sinTheta = sin(theta);
        for (uint32 j = 0; j <= in_slices; j++)
        {
            float phi    = j * PI_2 / in_slices;
            float cosPhi = cos(phi);
            float sinPhi = sin(phi);

            float x = (in_radius + in_tubeRadius * cosTheta) * cosPhi;
            float y = in_tubeRadius * sinTheta;
            float z = (in_radius + in_tubeRadius * cosTheta) * sinPhi;

            Vertex v;
            v.color = Vec4 { 1.f, 1.f, 1.f, 1.f };
            v.normal = Normalize({ x, y, z });
            v.position = Vec3 { x, y, z };
            v.texCoord = Vec2 { static_cast<float>(j) / in_slices, static_cast<float>(i) / in_stacks };
            vertices.push_back(v);
        }
    }

    // Create indices
    for (uint32 i = 0; i < in_stacks; i++)
    {
        for (uint32 j = 0; j < in_slices; j++)
        {
            uint32 v0 = i * (in_slices + 1) + j;
            uint32 v1 = i * (in_slices + 1) + (j + 1);
            uint32 v2 = (i + 1) * (in_slices + 1) + j;
            uint32 v3 = (i + 1) * (in_slices + 1) + (j + 1);

            indices.push_back(v0);
            indices.push_back(v2);
            indices.push_back(v1);

            indices.push_back(v1);
            indices.push_back(v2);
            indices.push_back(v3);
        }
    }

    auto vb = D11_VertexBuffer::Create(vertices);
    auto ib = D11_IndexBuffer::Create(indices);

    return D11_Mesh::Create(vb, ib);
}

Ref<D11_Mesh> MeshFactory3D::CreateGrid(const Vec2& in_size, uint32 in_rows, uint32 in_cols)
{
    std::vector<Vertex> vertices;
    std::vector<uint32> indices;
    float               widthPerCell  = in_size.x / float(in_cols);
    float               heightPerCell = in_size.y / float(in_rows);

    // Create vertices
    for (uint32 i = 0; i < in_rows; i++)
    {
        for (uint32 j = 0; j < in_cols; j++)
        {
            Vertex v1;
            v1.position = Vec3 { j * widthPerCell, 0.f, (i + 1) * heightPerCell };
            v1.normal   = Vec3 { 0.f, 1.f, 0.f };
            v1.texCoord = Vec2 { 0.f, 0.f };
            v1.color    = Vec4 { 1.f, 1.f, 1.f, 1.f };

            Vertex v2;
            v2.position = Vec3 { (j + 1) * widthPerCell, 0.f, (i + 1) * heightPerCell };
            v2.normal   = Vec3 { 0.f, 1.f, 0.f };
            v2.texCoord = Vec2 { 1.f, 0.f };
            v2.color    = Vec4 { 1.f, 1.f, 1.f, 1.f };

            Vertex v3;
            v3.position = Vec3 { (j + 1) * widthPerCell, 0.f, i * heightPerCell };
            v3.normal   = Vec3 { 0.f, 1.f, 0.f };
            v3.texCoord = Vec2 { 1.f, 1.f };
            v3.color    = Vec4 { 1.f, 1.f, 1.f, 1.f };

            Vertex v4;
            v4.position = Vec3 { j * widthPerCell, 0.f, i * heightPerCell };
            v4.normal   = Vec3 { 0.f, 1.f, 0.f };
            v4.texCoord = Vec2 { 0.f, 1.f };
            v4.color    = Vec4 { 1.f, 1.f, 1.f, 1.f };

            vertices.insert(vertices.end(), { v1, v2, v3, v4 });
        }
    }

    // Create indices
    for (uint32 i = 0; i < in_rows * in_cols; i++)
    {
        uint32 v0 = i * 4;
        uint32 v1 = v0 + 1;
        uint32 v2 = v0 + 2;
        uint32 v3 = v0 + 3;
        indices.insert(indices.end(), { v0, v1, v2, v0, v2, v3 });
    }

    auto vb = D11_VertexBuffer::Create(vertices);
    auto ib = D11_IndexBuffer::Create(indices);

    return D11_Mesh::Create(vb, ib);
}

MeshData<MeshFactory3D::Vertex> MeshFactory3D::CreateNormalLinesMeshData(const MeshData<Vertex>& in_meshData)
{
    std::vector<Vertex> vertices;
    std::vector<uint32> indices;

    for (const auto& v: in_meshData.m_vertices)
    {
        Vertex v1;
        v1.position   = v.position;
        v1.texCoord.x = 0.f;
        v1.color      = Vec4(1.f, 0.f, 0.f, 1.f);
        v1.normal     = v.normal;
        vertices.push_back(v1);

        Vertex v2;
        v2.position   = v.position;
        v2.texCoord.x = 1.f;
        v2.color      = Vec4(1.f, 1.f, 0.f, 1.f);
        v2.normal     = v.normal;
        vertices.push_back(v2);

        indices.push_back((uint32)indices.size());
        indices.push_back((uint32)indices.size());
    }

    MeshData<Vertex> meshData = {
        .m_vertices = vertices,
        .m_indices  = indices,
    };

    return meshData;
}

Ref<D11_Mesh> MeshFactory3D::CreateNormalLines(const MeshData<Vertex>& in_meshData)
{
    return D11_Mesh::Create(CreateNormalLinesMeshData(in_meshData));
}

Ref<D11_Mesh> MeshFactory3D::CreateNormalLines(const std::vector<MeshData<Vertex>>& in_meshDatas)
{
    MeshData<Vertex> meshData;
    for (const auto& md: in_meshDatas)
        meshData.Push(md);
    return CreateNormalLines(meshData);
}

}   // namespace crab