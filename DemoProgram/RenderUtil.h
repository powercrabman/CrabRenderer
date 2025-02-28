#pragma once

class RenderUtil
{
public:
    static Ref<Mesh> CreateMesh(
        const GeometryData& in_geo)
    {
        return Mesh::Create(
            Vertex3D::CreateVertices(in_geo),
            in_geo.indices,
            eTopology::TriangleList);
    }
};
