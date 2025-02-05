#pragma once

#include "VertexType.h"

namespace crab
{

struct D11_VertexBuffer;
struct D11_IndexBuffer;

//===================================================
// Mesh
//===================================================

template<typename Vertex>
struct MeshData
{
    void Push(const MeshData& in_meshData);

    std::vector<Vertex> m_vertices;
    std::vector<uint32> m_indices;
};

struct D11_Mesh
{
    template<typename Vertex>
    static Ref<D11_Mesh> Create(const std::vector<Vertex>& in_vertices,
                                const std::vector<uint32>& in_indices);

    template<typename Vertex>
    static Ref<D11_Mesh> Create(const MeshData<Vertex>& in_meshData);

    static Ref<D11_Mesh> Create(const Ref<D11_VertexBuffer>& in_vb,
                                const Ref<D11_IndexBuffer>&  in_ib);

    Ref<D11_VertexBuffer> vertexBuffer;
    Ref<D11_IndexBuffer>  indexBuffer;
};

//===================================================
// Mesh Factory
//===================================================

struct MeshFactory2D
{
    using Vertex        = Vertex_Pos2D_Tex_Color;
    using ColorArray    = std::array<Vec4, 4>;
    using TexCoordArray = std::array<Vec2, 4>;

    // Quad
    static Ref<D11_Mesh> CreateQuad(const Vec2& in_size, const TexCoordArray& in_texCoord, const ColorArray& in_color);
    static Ref<D11_Mesh> CreateQuad(const Vec2& in_size, const TexCoordArray& in_texCoord);
    static Ref<D11_Mesh> CreateQuad(const Vec2& in_size, const ColorArray& in_color);
    static Ref<D11_Mesh> CreateQuad(const Vec2& in_size, const Color& in_color);
    static Ref<D11_Mesh> CreateQuad(const Vec2& in_size);

    // Circle
    static Ref<D11_Mesh> CreateCircle(float in_radius, uint32 in_slices);
};

struct D11_Model;

struct MeshFactory3D
{
    using Vertex = Vertex_Pos3D_Normal_Tex_Color;

    // Cube
    using CubeColorArray    = std::array<Vec4, 24>;
    using CubeTexCoordArray = std::array<Vec2, 24>;

    static MeshData<Vertex> CreateCubeMeshData(const Vec3& in_size, const CubeTexCoordArray& in_texCoord, const CubeColorArray& in_color);
    static MeshData<Vertex> CreateCubeMeshData(const Vec3& in_size);
    static Ref<D11_Mesh>    CreateCube(const Vec3& in_size, const CubeTexCoordArray& in_texCoord, const CubeColorArray& in_color);
    static Ref<D11_Mesh>    CreateCube(const Vec3& in_size, const CubeTexCoordArray& in_texCoord);
    static Ref<D11_Mesh>    CreateCube(const Vec3& in_size, const CubeColorArray& in_color);
    static Ref<D11_Mesh>    CreateCube(const Vec3& in_size, const Color& in_color);
    static Ref<D11_Mesh>    CreateCube(const Vec3& in_size);
    static Ref<D11_Mesh>    CreateCubeMap(const Vec3& in_size);

    // Cylinder
    static Ref<D11_Mesh> CreateCylinder(float in_radius, float in_height, uint32 in_slices, uint32 in_stacks);

    // Sphere
    static MeshData<Vertex> CreateSphereMeshData(float in_radius, uint32 in_slices, uint32 in_stacks);
    static Ref<D11_Mesh>    CreateSphere(float in_radius, uint32 in_slices, uint32 in_stacks);
    static Ref<D11_Mesh>    CreateSphereMap(float in_radius, uint32 in_slices, uint32 in_stacks);

    // Cone
    static Ref<D11_Mesh> CreateCone(float in_radius, float in_height, uint32 in_slices);

    // Torus
    static Ref<D11_Mesh> CreateTorus(float in_radius, float in_tubeRadius, uint32 in_slices, uint32 in_stacks);

    // Grid
    static Ref<D11_Mesh> CreateGrid(const Vec2& in_size, uint32 in_rows, uint32 in_cols);

    // Normal mesh

    static MeshData<Vertex> CreateNormalLinesMeshData(const MeshData<Vertex>& in_meshData);
    static Ref<D11_Mesh>    CreateNormalLines(const MeshData<Vertex>& in_meshData);
    static Ref<D11_Mesh>    CreateNormalLines(const std::vector<MeshData<Vertex>>& in_meshDatas);
};

//===================================================
// Inline
//===================================================

template<typename Vertex>
inline Ref<D11_Mesh> D11_Mesh::Create(const std::vector<Vertex>& in_vertices, const std::vector<uint32>& in_indices)
{
    auto mesh          = CreateRef<D11_Mesh>();
    mesh->vertexBuffer = D11_VertexBuffer::Create(in_vertices);
    mesh->indexBuffer  = D11_IndexBuffer::Create(in_indices);
    return mesh;
}

template<typename Vertex>
static Ref<D11_Mesh> D11_Mesh::Create(const MeshData<Vertex>& in_meshData)
{
    return D11_Mesh::Create(in_meshData.m_vertices, in_meshData.m_indices);
}

template<typename Vertex>
void MeshData<Vertex>::Push(const MeshData& in_meshData)
{
    m_vertices.insert(m_vertices.end(), in_meshData.m_vertices.begin(), in_meshData.m_vertices.end());
    m_indices.insert(m_indices.end(), in_meshData.m_indices.begin(), in_meshData.m_indices.end());
}

}   // namespace crab
