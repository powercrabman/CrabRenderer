#pragma once
#include "Components.h"
#include "D11Enums.h"
#include "ModelUtil.h"

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
                Entity e  = in_scene->CreateEntity();
                auto&  mr = e.GetOrCreateComponent<ModelRenderer>();

                mr.model = CreateModel(path);
                e.CreateComponent<TAG("PBRPass")>();
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
        ImGui::SetNextWindowSize(ImVec2 { 300, 600 }, ImGuiCond_FirstUseEver);
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

                    if (entity.HasComponent<LabelComponent>())
                        label = entity.GetComponent<LabelComponent>().name;
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

                if (m_selectedEntity.HasComponent<LabelComponent>())
                    ImGui::Text("Tag: %s", m_selectedEntity.GetComponent<LabelComponent>().name.c_str());

                DrawTransformComponentInspector(m_selectedEntity.GetTransform());

                _DrawComponent<CameraControlComponent>("Camera Control",
                                                       [&]()
                                                       {
                                                           auto& cmrCont = m_selectedEntity.GetComponent<CameraControlComponent>();
                                                           ImGui::DragFloat("Move Speed", &cmrCont.moveSpeed, 0.01f);
                                                           ImGui::DragFloat("Rotate Speed", &cmrCont.rotSpeed, 0.01f);
                                                       });

                _DrawComponent<LightComponent>("Light",
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
                                               });

                _DrawComponent<SkyboxRenderer>("Skybox Renderer",
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

                                                   ImGui::RadioButton("Environment", reinterpret_cast<int*>(&skybox.mappingType), 0);
                                                   ImGui::SameLine();
                                                   ImGui::RadioButton("Irradiance", reinterpret_cast<int*>(&skybox.mappingType), 1);
                                                   ImGui::SameLine();
                                                   ImGui::RadioButton("Specular", reinterpret_cast<int*>(&skybox.mappingType), 2);
                                               });

                _DrawComponent<CameraComponent>("Camera Component",
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

                _DrawComponent<ModelRenderer>("Model Renderer",
                                              [&]()
                                              {
                                                  auto& modelRenderer = m_selectedEntity.GetComponent<ModelRenderer>();
                                                  auto& model         = modelRenderer.model;
                                                  if (model)
                                                  {

                                                      uint32 id = 0;
                                                      ImGui::Text("Node count: %d", model->GetNodes().size());
                                                      ImGui::Separator();

                                                      for (auto& node: model->GetNodesRef())
                                                      {
                                                          auto& mesh = node.mesh;
                                                          auto& mat  = node.material;

                                                          ImGui::Text("Node name: %s", node.name.c_str());

                                                          ImGui::PushID(id++);

                                                          ImGui::Text("Mesh");
                                                          if (mesh)
                                                              _MeshInspector(mesh);
                                                          else
                                                              ImGui::Text("No Mesh");

                                                          ImGui::Separator();

                                                          ImGui::Text("Material");

                                                          if (mat)
                                                              _MaterialInspector(mat);
                                                          else
                                                              ImGui::Text("No Material");

                                                          ImGui::PopID();
                                                      }
                                                  }
                                              });

                _DrawComponent<PlanarMirrorComponent>("Plana Mirror",
                                                      [&]() {
                                                          auto& mirror = m_selectedEntity.GetComponent<PlanarMirrorComponent>();
                                                          if (mirror.mirrorMesh)
                                                          {
                                                              ImGui::Text("Mirror Mesh");
                                                              _MeshInspector(mirror.mirrorMesh);
                                                          }
                                                          else
                                                          {
                                                              ImGui::Text("No Mirror Mesh");
                                                          }

                                                          if (mirror.mirrorMaterial)
                                                          {
                                                              ImGui::Text("Mirror Material");
                                                              _MaterialInspector(mirror.mirrorMaterial);
                                                          }
                                                          else
                                                          {
                                                              ImGui::Text("No Mirror Material");
                                                          }

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

private:
    void _MeshInspector(Ref<Mesh>& in_mesh)
    {
        ImGui::Text("Primitive Topology: %s", ToString(in_mesh->GetTopology()));
        ImGui::Text("Vertex Count: %d", in_mesh->GetVertexCount());
        ImGui::Text("Vertex Stride: %d byte", in_mesh->GetVertexStride());
        ImGui::Text("Index Count: %d", in_mesh->GetIndexCount());
        ImGui::Text("Index Stride: %d byte", in_mesh->GetIndexStride());
    }

    void _MaterialInspector(Ref<Material>& in_material)
    {
        auto mat = in_material->GetMaterialData();
        if (ImGui::ColorEdit3("Base Color", reinterpret_cast<float*>(&mat.baseColor)))
            in_material->SetBaseColor(mat.baseColor);
        if (ImGui::ColorEdit3("Emissive", reinterpret_cast<float*>(&mat.emissive)))
            in_material->SetEmissive(mat.emissive);
        if (ImGui::DragFloat("Metallic", &mat.metallic, 0.01f, 0.f, 1.f))
            in_material->SetMetallic(mat.metallic);
        if (ImGui::DragFloat("Roughness", &mat.roughness, 0.01f, 0.f, 1.f))
            in_material->SetRoughness(mat.roughness);
        if (ImGui::DragFloat("Displacement Strength", &mat.displacementStrength, 0.01f, 0.f, 1.f))
            in_material->SetDisplacementStrength(mat.displacementStrength);
        if (ImGui::ColorEdit3("Diffuse", reinterpret_cast<float*>(&mat.diffuse)))
            in_material->SetDiffuse(mat.diffuse);
        if (ImGui::ColorEdit3("Specular", reinterpret_cast<float*>(&mat.specular)))
            in_material->SetSpecular(mat.specular);
        if (ImGui::ColorEdit3("Ambient", reinterpret_cast<float*>(&mat.ambient)))
            in_material->SetAmbient(mat.ambient);
        if (ImGui::DragFloat("Shininess", &mat.shininess, 0.01f, 0.f, 100.f))
            in_material->SetSharpness(mat.shininess);
        if (ImGui::DragFloat("Alpha", &mat.alpha, 0.01f, 0.f, 1.f))
            in_material->SetAlpha(mat.alpha);
        ImGui::Separator();
        if (_DrawTextureLoadReleaseItem("Base Color", mat.baseColorImage, true))
            in_material->SetBaseColorImage(mat.baseColorImage);
        ImGui::Separator();
        if (_DrawTextureLoadReleaseItem("Normal", mat.normalImage))
            in_material->SetNormalImage(mat.normalImage);
        if (ImGui::RadioButton("OpenGL", reinterpret_cast<int*>(&mat.normalMapType), 0))
            in_material->SetNormalMapType(eNormalMapType::OpenGL);
        ImGui::SameLine();
        if (ImGui::RadioButton("DirectX", reinterpret_cast<int*>(&mat.normalMapType), 1))
            in_material->SetNormalMapType(eNormalMapType::DirectX);
        ImGui::Separator();
        if (_DrawTextureLoadReleaseItem("Metallic", mat.metallicImage))
            in_material->SetMetallicImage(mat.metallicImage);

        ImGui::Separator();

        if (_DrawTextureLoadReleaseItem("Roughness", mat.roughnessImage))
            in_material->SetRoughnessImage(mat.roughnessImage);

        ImGui::Separator();

        if (_DrawTextureLoadReleaseItem("AO", mat.aoImage))
            in_material->SetAOImage(mat.aoImage);
        ImGui::Separator();
        if (_DrawTextureLoadReleaseItem("Emissive", mat.emissiveImage))
            in_material->SetEmissiveImage(mat.emissiveImage);
        ImGui::Separator();
        if (_DrawTextureLoadReleaseItem("Displacement", mat.displacementImage))
            in_material->SetHeightImage(mat.displacementImage);
    }

    template<typename Ty, typename Func>
    void _DrawComponent(const std::string_view in_label, const Func& in_func)
    {
        if (m_selectedEntity.HasComponent<Ty>())
        {
            if (ImGui::CollapsingHeader(in_label.data(), ImGuiTreeNodeFlags_DefaultOpen))
            {
                in_func();
            }
        }
    }

    void _DrawSkyboxLoadReleaseItem(const char* in_label, Ref<CubemapImage>& in_tex)
    {
        Ref<Image2D> image = in_tex->GetImage();

        ImGui::Text(in_label);
        if (!in_tex)
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

    bool _DrawTextureLoadReleaseItem(const char* in_label, Ref<Image2D>& in_tex, bool in_reverseToneMapping = false)
    {
        ImGui::Text(in_label);

        if (in_tex)
            ImGui::Image(reinterpret_cast<ImTextureID>(in_tex->GetSRV()), { 64, 64 });
        else
            ImGui::Text("No Texture");

        if (ImGui::Button(fmt::format("Load##{}", in_label).c_str()))
        {
            auto path = GetPathFromFileDialog();

            if (!path.empty())
                in_tex = Image2D::CreateFromFile(path, true, in_reverseToneMapping);

            return true;
        }

        ImGui::SameLine();

        if (ImGui::Button(fmt::format("Release##{}", in_label).c_str()))
        {
            in_tex = nullptr;
            return true;
        }

        return false;
    }

    Entity m_selectedEntity = Entity::s_null;
};
