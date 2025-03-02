#include "CrabPch.h"

#include "SceneManager.h"

#include "CrabComponents.h"
#include "EventDispatcher.h"
#include "Scene.h"

#include "Script.h"

namespace crab
{

SceneManager::SceneManager()
{
}

void SceneManager::_EnterScene() const
{
    if (m_currentScene)
    {
        if (!m_currentScene->IsInited())
        {
            m_currentScene->Init();
            m_currentScene->m_isInit = true;
        }

        m_currentScene->OnEnter();
    }
}

SceneManager::~SceneManager()
{
}

Scene* SceneManager::FindSceneByName(std::string_view in_name)
{
    auto it = m_scenes.find(std::string(in_name));

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

void SceneManager::DeleteSceneByName(std::string_view in_name)
{
    DeleteScene(FindSceneByName(in_name));
}

void SceneManager::DeleteScene(const Scene* in_scene)
{
    if (m_currentScene == in_scene)
    {
        m_currentScene->OnExit();
        m_currentScene = nullptr;
    }

    auto it = m_scenes.find(std::string(in_scene->GetName()));
    if (it != m_scenes.end())
    {
        m_scenes.erase(it);
        std::erase(m_sceneNames, in_scene->GetName());
    }
    else
    {
        CRAB_DEBUG_BREAK("Scene not found!");
    }
}

void SceneManager::ChangeSceneByName(std::string_view in_name)
{
    ChangeScene(FindSceneByName(in_name));
}

void SceneManager::ChangeScene(Scene* in_scene)
{
    if (m_currentScene)
    {
        m_currentScene->OnExit();
    }
    m_currentScene = in_scene;
    _EnterScene();
}

void SceneManager::OnUpdate(TimeStamp& in_ts) const
{
    if (m_currentScene)
    {
        // script update
        m_currentScene->GetView<ScriptComponent>().each(
            [&](const ScriptComponent& in_script)
            {
                in_script.script->OnUpdate(in_ts);
            });

        m_currentScene->OnUpdate(in_ts);
    }
}

void SceneManager::OnRender(TimeStamp& in_ts) const
{
    if (m_currentScene)
    {
        m_currentScene->OnRender(in_ts);
        m_currentScene->OnPostRender(in_ts);
    }
}

void SceneManager::OnRenderGUI(TimeStamp& in_ts) const
{
    if (m_currentScene)
    {
        m_currentScene->OnRenderGUI(in_ts);
    }
}

void SceneManager::OnEvent(CrabEvent& in_event)
{
    if (m_currentScene)
    {
        m_currentScene->OnEvent(in_event);

        EventDispatcher dispatcher(in_event);

        HANDLE_EVENT(AppClose_CoreEvent,
                     [&](const AppClose_CoreEvent& e)
                     {
                         for (auto& scene: m_scenes | std::views::values)
                         {
                             scene->OnExit();
                             scene->ClearRegistry();
                             scene.reset();
                         }
                     });
    }
}

}   // namespace crab