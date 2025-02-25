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
                GetSceneManager().ChangeSceneByName(name);
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

inline std::filesystem::path GetPathFromFileDialog();

inline void DrawDemoInspector(const std::string_view in_sceneName)
{
    DrawDemoInspector(in_sceneName, nullptr);
}

inline bool DrawTransformComponentInspector(TransformComponent& in_transform)
{
    bool retVal = false;
    Vec3 euler  = in_transform.rotate.ToEuler() * RAD2DEG;

    ImGui::PushID(0);

    ImGui::Text("Position");
    if (ImGui::DragFloat3("##Position", &in_transform.position.x, 0.01f))
        retVal = true;

    ImGui::SameLine();
    if (ImGui::Button("Reset##Position"))
    {
        in_transform.position = Vec3::Zero;
        retVal                = true;
    }

    ImGui::Text("Rotation (Pitch Yaw Roll)");
    if (ImGui::DragFloat3("##Rotation", &euler.x, 0.1f, 0, 0, "%.3f Deg"))
    {
        euler *= DEG2RAD;
        in_transform.rotate = Quat::CreateFromYawPitchRoll(euler.y, euler.x, euler.z);
        retVal              = true;
    }
    ImGui::SameLine();
    if (ImGui::Button("Reset##Rotation"))
    {
        in_transform.rotate = Quat::Identity;
        retVal              = true;
    }

    ImGui::Text("Scale");
    if (ImGui::DragFloat3("##Scale", &in_transform.scale.x, 0.01f))
        retVal = true;
    ;

    ImGui::SameLine();
    if (ImGui::Button("Reset##Scale"))
    {
        in_transform.scale = Vec3::One;
        retVal             = true;
    }

    ImGui::PopID();

    return retVal;
}

inline void DrawVoidContext(Scene* in_scene)
{
    if (ImGui::BeginPopupContextVoid())
    {
        ImGui::Text("Demo Systems");

        ImGui::Separator();

        if (ImGui::MenuItem("Load Model"))
        {
            std::vector<std::filesystem::path> filterPattenr = {
                "*.obj", "*.fbx"
            };

            std::filesystem::path path =
                FileSystem::OpenFileDialog(L"File model", "", filterPattenr, L"");

            if (!path.empty())
            {
                ModelLoader loader;
                loader.Load(path);

                CRAB_DEBUG_BREAK("todo..");
                // Entity e      = in_scene->CreateEntity();
                // auto&  mr     = e.GetOrCreateComponent<MeshRenderer>();
                // mr.mesh       = loader.CreateMergedMesh();
                // mr.transConst = ConstantBuffer<TransformConstant>::Create();
                //
                // auto& mat      = e.GetOrCreateComponent<PBRMaterialComponent>();
                // mat.pbrConst   = ConstantBuffer<PBRMaterialConstant>::Create();
                // mat.pbrVSConst = ConstantBuffer<PBRVSConstant>::Create();
            }
        }

        ImGui::EndPopup();
    }
}

class SceneHierarchy
{
public:
    void DrawSceneHierarchy(Scene* in_scene)
    {
        ImGui::SetNextWindowSize(ImVec2 { 300, 600 });
        ImGui::Begin("Scene Hierarchy", nullptr);

        ImGui::Text("Scene: %s", in_scene->GetName());

        ImGui::Separator();

        auto view = in_scene->GetView<IDComponent>();

        ImGui::Text("Entities count: %d", view.size());

        if (ImGui::BeginChild("Entity view", { 0.f, 300.f }, ImGuiChildFlags_Border))
        {
            ImGuiListClipper clipper;
            clipper.Begin(view.size());

            while (clipper.Step())
            {
                for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++)
                {
                    entt::entity e = *(view.begin() + i);
                    Entity       entity { in_scene, e };

                    std::string label;

                    if (entity.HasComponent<TagComponent>())
                        label = entity.GetComponent<TagComponent>().name;
                    else
                        label = fmt::format("Entity {}", entity.GetID().id);

                    if (ImGui::Selectable(label.c_str(), entity == m_selectedEntity))
                        m_selectedEntity = entity;
                }
            }

            ImGui::EndChild();
        }

        if (ImGui::BeginChild("Entity Inspector", {}, ImGuiChildFlags_Border))
        {
            if (m_selectedEntity.IsValid())
            {
                ImGui::Text("Entity: %d", m_selectedEntity.GetID().id);

                if (m_selectedEntity.HasComponent<TagComponent>())
                    ImGui::Text("Tag: %s", m_selectedEntity.GetComponent<TagComponent>().name.c_str());

                DrawTransformComponentInspector(m_selectedEntity.GetTransform());

                DrawComponent<CameraControlComponent>("Camera Control",
                                                      [&]()
                                                      {
                                                          auto& cmrCont = m_selectedEntity.GetComponent<CameraControlComponent>();
                                                          ImGui::DragFloat("Move Speed", &cmrCont.moveSpeed, 0.01f);
                                                          ImGui::DragFloat("Rotate Speed", &cmrCont.rotSpeed, 0.01f);
                                                      });

                DrawComponent<LightComponent>("Light",
                                              [&]()
                                              {
                                                  auto& light = m_selectedEntity.GetComponent<LightComponent>();
                                                  ImGui::ColorEdit3("Radiance", &light.lightRadiance.x);
                                                  ImGui::DragFloat("FallOff Start", &light.fallOffStart, 0.01f, 0.f, light.fallOffEnd);
                                                  ImGui::DragFloat("FallOff End", &light.fallOffEnd, 0.01f, light.fallOffStart);
                                                  ImGui::DragFloat("Spot Power", &light.spotPower, 0.1f);
                                                  ImGui::DragFloat("Strength", &light.lightStrength, 0.01f);
                                                  ImGui::RadioButton("Directional", reinterpret_cast<int*>(&light.type), 1);
                                                  ImGui::SameLine();
                                                  ImGui::RadioButton("Point", reinterpret_cast<int*>(&light.type), 2);
                                                  ImGui::SameLine();
                                                  ImGui::RadioButton("Spot", reinterpret_cast<int*>(&light.type), 3);

                                                  ImGui::Checkbox("Use Shadow Map", &light.useShadowMap);
                                              });

                DrawComponent<PBRMaterialComponent>("PBR Material",
                                                    [&]()
                                                    {
                                                        auto& mat = m_selectedEntity.GetComponent<PBRMaterialComponent>();
                                                        ImGui::ColorEdit3("Albedo", &mat.albedo.x);
                                                        ImGui::ColorEdit3("Emissive", &mat.emissive.x);
                                                        ImGui::DragFloat("Metallic", &mat.metallic, 0.01f, 0.f, 1.f);
                                                        ImGui::DragFloat("Roughness", &mat.roughness, 0.01f, 0.f, 1.f);
                                                        ImGui::DragFloat("Displacement Scale", &mat.displacementScale, 0.01f);

                                                        _DrawTextureLoadReleaseItem("Albedo Map", mat.albedoMap);
                                                        ImGui::Separator();
                                                        _DrawTextureLoadReleaseItem("Normal Map", mat.normalMap);
                                                        ImGui::Text("Normal map type");
                                                        ImGui::RadioButton("OpenGL", reinterpret_cast<int*>(&mat.normalMapType), 0);
                                                        ImGui::SameLine();
                                                        ImGui::RadioButton("DirectX", reinterpret_cast<int*>(&mat.normalMapType), 1);
                                                        ImGui::Separator();

                                                        _DrawTextureLoadReleaseItem("AO Map", mat.aoMap);
                                                        ImGui::Separator();
                                                        _DrawTextureLoadReleaseItem("Roughness Map", mat.roughnessMap);
                                                        ImGui::Separator();
                                                        _DrawTextureLoadReleaseItem("Metallic Map", mat.metallicMap);
                                                        ImGui::Separator();
                                                        _DrawTextureLoadReleaseItem("Emissive Map", mat.emissiveMap);
                                                        ImGui::Separator();
                                                        _DrawTextureLoadReleaseItem("Displacement Map", mat.displacementMap);
                                                    });

                DrawComponent<SkyboxRenderer>("Skybox Renderer",
                                              [&]()
                                              {
                                                  auto& skybox = m_selectedEntity.GetComponent<SkyboxRenderer>();

                                                  _DrawSkyboxLoadReleaseItem("Environment", skybox.envCubemap);
                                                  ImGui::Separator();
                                                  _DrawSkyboxLoadReleaseItem("Specular", skybox.specCubemap);
                                                  ImGui::Separator();
                                                  _DrawSkyboxLoadReleaseItem("Diffuse", skybox.irrCubemap);
                                                  ImGui::Separator();
                                                  _DrawTextureLoadReleaseItem("BRDF", skybox.brdfImage);
                                              });

                DrawComponent<CameraComponent>("Camera Component",
                                               [&]()
                                               {
                                                   auto& cmr = m_selectedEntity.GetComponent<CameraComponent>();
                                                   ImGui::Text("Projection Type");
                                                   ImGui::RadioButton("Orthographic", reinterpret_cast<int*>(&cmr.projectionType), 0);
                                                   ImGui::SameLine();
                                                   ImGui::RadioButton("Perspective", reinterpret_cast<int*>(&cmr.projectionType), 1);
                                                   ImGui::DragFloat("Aspect", &cmr.aspect, 0.01f);
                                                   ImGui::DragFloat("Near Z", &cmr.nearZ, 0.01f, 0.01f, cmr.farZ);
                                                   ImGui::DragFloat("Far Z", &cmr.farZ, 0.01f, cmr.nearZ, 10000.f);
                                                   ImGui::DragFloat("FOV", &cmr.fov, 0.01f);
                                               });
            }
            else
            {
                ImGui::Text("Select an entity to view its properties");
            }

            ImGui::EndChild();
        }

        ImGui::End();
    }

    template<typename Ty, typename Func>
    void DrawComponent(const std::string_view in_label, const Func& in_func)
    {
        if (m_selectedEntity.HasComponent<Ty>())
        {
            if (ImGui::CollapsingHeader(in_label.data(), ImGuiTreeNodeFlags_DefaultOpen))
            {
                in_func();
            }
        }
    }

private:
    void _DrawSkyboxLoadReleaseItem(const char* in_label, Ref<CubemapImage>& in_tex)
    {
        Ref<Image2D> image = in_tex->GetImage();

        ImGui::Text(in_label);
        if (in_tex)
            ImGui::Image(reinterpret_cast<ImTextureID>(image->GetSRV()), { 64, 64 });
        else
            ImGui::Text("No Texture");
        if (ImGui::Button(fmt::format("Load##{}", in_label).c_str()))
        {
            auto path = GetPathFromFileDialog();

            if (!path.empty())
                in_tex = CubemapImage::Create(path);
        }
        ImGui::SameLine();
        if (ImGui::Button(fmt::format("Release##{}", in_label).c_str()))
            in_tex = nullptr;
    }

    void _DrawTextureLoadReleaseItem(const char* in_label, Ref<Image2D>& in_tex)
    {
        ImGui::Text(in_label);
        if (in_tex)
            ImGui::Image(reinterpret_cast<ImTextureID>(in_tex->GetSRV()), { 64, 64 });
        else
            ImGui::Text("No Texture");
        if (ImGui::Button(fmt::format("Load##{}", in_label).c_str()))
        {
            auto path = GetPathFromFileDialog();

            in_tex = Image2D::CreateFromFile(path, true);
        }
        ImGui::SameLine();
        if (ImGui::Button(fmt::format("Release##{}", in_label).c_str()))
            in_tex = nullptr;
    }

    Entity m_selectedEntity = Entity::s_null;
};
