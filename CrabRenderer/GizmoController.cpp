#include "CrabPch.h"

#include "GizmoController.h"

#include "AppWindow.h"

namespace crab
{

void GizmoController::BeginGizmo(const Mat4& in_view, const Mat4& in_proj, eProjectionType in_projType, eGizmoControlMode in_mode, Vec3& in_position, Quat& in_rotation, Vec3& in_scale)
{
    auto [width, height] = GetAppWindow().GetWindowSize();

    BeginGizmo(
        in_view,
        in_proj,
        in_projType,
        in_mode,
        Rect { 0, 0, static_cast<uint32>(width), static_cast<uint32>(height) },
        in_position,
        in_rotation,
        in_scale);
}

void GizmoController::DrawGrid(const Mat4& in_view, const Mat4& in_proj, const Vec3& in_gridPos, const Vec3& in_gridRot, const Vec3& in_gridScale, const float in_girdSize)
{
    Mat4 world = Mat4::CreateTranslation(in_gridPos) * Mat4::CreateFromYawPitchRoll(in_gridRot.y, in_gridRot.x, in_gridRot.z) * Mat4::CreateScale(in_gridScale);
    ImGuizmo::DrawGrid(
        reinterpret_cast<const float*>(&world),
        reinterpret_cast<const float*>(&in_view),
        reinterpret_cast<const float*>(&in_proj),
        in_girdSize);
}

}   // namespace crab