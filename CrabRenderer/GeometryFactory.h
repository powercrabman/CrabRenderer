#pragma once
#include "CrabVertex.h"

namespace crab
{

class GeometryFactory
{
public:
    static GeometryData CreateQuad(float width, float height);
    static GeometryData CreateCircle(float radius, uint32 slices);

    static GeometryData CreateCube(float width, float height, float depth);
    static GeometryData CreateSphere(float radius, uint32 slices, uint32 stacks);
    static GeometryData CreateGrid(float width, float height, uint32 repeatX, uint32 repeatY);
};

class GeometryUtil
{
public:
    static std::vector<Vec3> ComputeNormals(const std::vector<uint32>& in_indices,
                                            const std::vector<Vec3>&   in_positions);

    static std::vector<Vec3> ComputeTangents(const std::vector<uint32>& in_indices,
                                             const std::vector<Vec3>&   in_positions,
                                             const std::vector<Vec3>&   in_normals,
                                             const std::vector<Vec2>&   in_texCoords);
};

}   // namespace crab