#pragma once
#include "Singleton.h"

namespace crab
{

class Scene;
struct TimeStamp;

class SceneManager : public Singleton<SceneManager>
{
    friend class Singleton<SceneManager>;

public:
    ~SceneManager();

    template<typename Ty>
    Scene* CreateScene();

    template<typename Ty>
    Scene* FindScene();
    Scene* FindSceneByName(std::string_view in_name);

    template<typename Ty>
    void DeleteScene();
    void DeleteSceneByName(std::string_view in_name);
    void DeleteScene(const Scene* in_scene);

    template<typename Ty>
    void ChangeScene();
    void ChangeSceneByName(std::string_view in_name);
    void ChangeScene(Scene* in_scene);

    void OnUpdate(TimeStamp& in_ts) const;
    void OnRender(TimeStamp& in_ts) const;
    void OnRenderGUI(TimeStamp& in_ts) const;

    void OnEvent(CrabEvent& in_event);

    Scene*                               GetCurrentScene() const { return m_currentScene; }
    const std::vector<std::string_view>& GetSceneNames() const { return m_sceneNames; }

private:
    SceneManager();

    void _EnterScene() const;

    std::unordered_map<std::string, Scope<Scene>> m_scenes;
    std::vector<std::string_view>                 m_sceneNames;
    Scene*                                        m_currentScene;
};

template<typename Ty>
void crab::SceneManager::ChangeScene()
{
    ChangeScene(FindScene<Ty>());
}

template<typename Ty>
void crab::SceneManager::DeleteScene()
{
    DeleteScene(FindScene<Ty>());
}

template<typename Ty>
Scene* crab::SceneManager::CreateScene()
{
    static_assert(IS_BASE_OF(Scene, Ty), "Ty must be derived from Scene!");

    auto scene             = CreateScope<Ty>();
    scene->m_sceneTypeInfo = TypeInfo::Get<Ty>();
    std::string sceneName  = ExtractStringInTypeName(scene->m_sceneTypeInfo.name);
    scene->m_sceneName     = sceneName;

    auto [it, _] = m_scenes.emplace(scene->GetName(), std::move(scene));
    m_sceneNames.push_back(it->first);
    CRAB_ASSERT(_, "Scene already exists!");
    return it->second.get();
}

//===================================================
// Inline
//===================================================

inline SceneManager& GetSceneManager()
{
    return SceneManager::GetInstance();
}

template<typename Ty>
Scene* crab::SceneManager::FindScene()
{
    static_assert(IS_BASE_OF(Scene, Ty), "Ty must be derived from Scene!");

    std::string sceneName = ExtractStringInTypeName(TypeInfo::Get<Ty>().name);
    auto        it        = m_scenes.find(sceneName);

    if (it != m_scenes.end())
    {
        return it->second.get();
    }
    else
    {
        CRAB_DEBUG_BREAK("Scene not found!");
        return nullptr;
    }
}

}   // namespace crab