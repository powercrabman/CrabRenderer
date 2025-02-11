#pragma once

using DrawInspectorFlags = uint32;

inline void DrawDemoInspector(
    const std::string_view       in_sceneName,
    const std::function<void()>& in_drawFunc)
{
    ImGui::Begin("Inspector", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::Text("Current scene: %s", in_sceneName.data());

    const std::vector<std::string_view>& sceneNames = GetSceneManager().GetSceneNames();
    if (ImGui::BeginCombo("Scenes", in_sceneName.data()))
    {
        for (const auto& name: sceneNames)
        {
            bool isSelected = in_sceneName == name;
            if (ImGui::Selectable(name.data(), isSelected))
            {
                GetSceneManager().ChnageSceneByName(name);
            }
            if (isSelected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }

    ImGui::SeparatorText("Performance Checker");
    const TimeStamp& ts = GetApplication().GetTimeStamp();
    ImGui::Text("Delta time: %.3f ms", ts.deltaTime * 1'000);
    ImGui::Text("Total time: %.3f sec", ts.totalTime);
    ImGui::Text("Frame per sec: %d fps", ts.framePerSec);
    ImGui::Text("Refresh rate: %.3f Hz", 1.f / ts.deltaTime);

    ImGui::SeparatorText("IO");
    auto [mx, my] = Input::GetMousePos();
    ImGui::Text("Mouse Pos: %d, %d", mx, my);

    auto [dx, dy] = Input::GetMouseDeltaPos();
    ImGui::Text("Mouse Delta: %d, %d", dx, dy);

    if (in_drawFunc)
    {
        in_drawFunc();
    }

    ImGui::End();
}

inline void DrawDemoInspector(const std::string_view in_sceneName)
{
    DrawDemoInspector(in_sceneName, nullptr);
}

inline void DrawTransformInspector(Transform&         out_trans,
                                   DrawInspectorFlags in_flags = 0)
{
    Vec3 rotate = out_trans.quaternion.ToEuler();
    rotate *= RAD2DEG;

    ImGui::DragFloat3("Position", (float*)&out_trans.position, 0.01f);
    if (ImGui::DragFloat3("Rotation", (float*)&rotate, 0.1f, 0.f, 0.f, "%.3f Deg"))
    {
        rotate *= DEG2RAD;
        Quat q = Quat::CreateFromYawPitchRoll(rotate.y, rotate.x, rotate.z);
        out_trans.quaternion = q;

    }
    ImGui::DragFloat3("Scale", (float*)&out_trans.scale, 0.01f, 0.01f, 0.f);
}

inline void DrawCameraInspector(Transform&       out_trans,
                                CameraComponent& out_cmr)
{

    // Camera Type
    if (ImGui::RadioButton("Orthographic", out_cmr.projectionType == eProjectionType::Orthographic))
        out_cmr.projectionType = eProjectionType::Orthographic;

    ImGui::SameLine();

    if (ImGui::RadioButton("Perspective", out_cmr.projectionType == eProjectionType::Perspective))
        out_cmr.projectionType = eProjectionType::Perspective;

    // Camera Transform
    Vec3 cmrRotation = out_trans.quaternion.ToEuler();
    cmrRotation *= RAD2DEG;

    ImGui::DragFloat3("Camera Position", (float*)&out_trans.position, 0.01f);

    if (ImGui::DragFloat3("Camera Rotation", (float*)&cmrRotation, 0.1f, 0.f, 0.f, "%.3f Deg"))
    {
        cmrRotation *= DEG2RAD;
        Quat q = Quat::CreateFromYawPitchRoll(cmrRotation.y, cmrRotation.x, cmrRotation.z);
        out_trans.quaternion = q;
    }

    if (out_cmr.projectionType == eProjectionType::Orthographic)
    {
        ImGui::DragFloat("Orthographic Aspect", &out_cmr.aspect, 0.01f);
        ImGui::DragFloat("Orthographic Near Plane", &out_cmr.nearZ, 0.01f);
        ImGui::DragFloat("Orthographic Far Plane", &out_cmr.farZ, 0.01f);

        if (out_cmr.nearZ >= out_cmr.farZ)
            out_cmr.nearZ = out_cmr.farZ - 0.1f;

        if (out_cmr.farZ <= out_cmr.nearZ)
            out_cmr.farZ = out_cmr.nearZ + 0.1f;

        if (ImGui::SmallButton("Suit Aspect"))
        {
            out_cmr.aspect = GetApplication().GetAppWindow().GetAspect();
        }
    }
    else
    {
        float fovDeg = out_cmr.fov * RAD2DEG;
        if (ImGui::DragFloat("Perspective Fov", &fovDeg, 0.1f))
        {
            out_cmr.fov = fovDeg * DEG2RAD;
            out_cmr.fov = std::clamp(out_cmr.fov, 0.1f, 179.9f);
        }

        ImGui::DragFloat("Perspective Aspect", &out_cmr.aspect, 0.01f);
        ImGui::DragFloat("Perspective Near Plane", &out_cmr.nearZ, 0.01f);
        ImGui::DragFloat("Perspective Far Plane", &out_cmr.farZ, 0.01f);

        if (out_cmr.nearZ >= out_cmr.farZ)
            out_cmr.nearZ = out_cmr.farZ - 0.1f;

        if (out_cmr.farZ <= out_cmr.nearZ)
            out_cmr.farZ = out_cmr.nearZ + 0.1f;

        if (ImGui::SmallButton("Suit Aspect"))
        {
            out_cmr.aspect = GetApplication().GetAppWindow().GetAspect();
        }
    }
}

