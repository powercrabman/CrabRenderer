#pragma once

#include "CrabComponents.h"

#include <ImGuizmo.h>

namespace crab
{

enum class eGizmoControlMode
{
    Translate = ImGuizmo::OPERATION::TRANSLATE,
    Rotate    = ImGuizmo::OPERATION::ROTATE,
    Scale     = ImGuizmo::OPERATION::SCALE
};

enum class eGizmoSpace
{
    Local = ImGuizmo::MODE::LOCAL,
    World = ImGuizmo::MODE::WORLD,
};

// this is vert thin wrapper of ImGuizmo
// todo : world/local space

class GizmoController
{
public:
    static void BeginGizmo(
        const Mat4&       in_view,
        const Mat4&       in_proj,
        eProjectionType   in_projType,
        eGizmoControlMode in_mode,
        Vec3&             in_position,
        Quat&             in_rotation,
        Vec3&             in_scale);

    static void BeginGizmo(
        const Mat4&       in_view,
        const Mat4&       in_proj,
        eProjectionType   in_projType,
        eGizmoControlMode in_mode,
        const Rect&       in_viewRect,
        Vec3&             inout_position,
        Quat&             inout_rotation,
        Vec3&             inout_scale)
    {
        ImGuizmo::SetRect(
            static_cast<float>(in_viewRect.position.x),
            static_cast<float>(in_viewRect.position.y),
            static_cast<float>(in_viewRect.size.x),
            static_cast<float>(in_viewRect.size.y));

        ImGuizmo::SetDrawlist();
        ImGuizmo::SetOrthographic(in_projType == eProjectionType::Orthographic);

        Mat4 world = Mat4::CreateScale(inout_scale) * Mat4::CreateFromQuaternion(inout_rotation) * Mat4::CreateTranslation(inout_position);

        if (ImGuizmo::Manipulate(
                reinterpret_cast<const float*>(&in_view),
                reinterpret_cast<const float*>(&in_proj),
                static_cast<ImGuizmo::OPERATION>(in_mode),
                static_cast<ImGuizmo::MODE>(eGizmoSpace::World),
                reinterpret_cast<float*>(&world)))
        {
            world.Decompose(inout_scale, inout_rotation, inout_position);
        }
    }

    static bool IsUsing()
    {
        return ImGuizmo::IsUsing();
    }

    static bool IsHovering()
    {
        return ImGuizmo::IsOver();
    }

    static bool IsUsingAny()
    {
        return ImGuizmo::IsUsingAny();
    }

    static void DrawGrid(
        const Mat4& in_view,
        const Mat4& in_proj,
        const Vec3& in_gridPos,
        const Vec3& in_gridRot,
        const Vec3& in_gridScale,
        const float in_girdSize);
};

}   // namespace crab