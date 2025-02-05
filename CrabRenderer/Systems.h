#pragma once

namespace crab
{
class Scene;
struct MeshRenderer;
struct Transform;

static void MeshRendererSystem(Scene*                                                in_scene,
                               const std::function<void(MeshRenderer&, Transform&)>& in_userFunc_or_null);

}   // namespace crab