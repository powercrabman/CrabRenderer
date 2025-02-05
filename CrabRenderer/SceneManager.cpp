#include "CrabPch.h"

#include "SceneManager.h"

#include "Scene.h"

namespace crab
{

SceneManager::SceneManager()
{
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

void SceneManager::DeleteScene(Scene* in_scene)
{
    if (m_currentScene == in_scene)
    {
        m_currentScene->_OnExit();
        m_currentScene = nullptr;
    }

    auto it = m_scenes.find(std::string(in_scene->GetName()));
    if (it != m_scenes.end())
    {
        m_scenes.erase(it);
        m_sceneNames.erase(std::remove(m_sceneNames.begin(),
                                       m_sceneNames.end(),
                                       in_scene->GetName()),
                           m_sceneNames.end());
    }
    else
    {
        CRAB_DEBUG_BREAK("Scene not found!");
    }
}

void SceneManager::ChnageSceneByName(std::string_view in_name)
{
    ChangeScene(FindSceneByName(in_name));
}

void SceneManager::ChangeScene(Scene* in_scene)
{
    if (m_currentScene)
    {
        m_currentScene->_OnExit();
    }
    m_currentScene = in_scene;
    m_currentScene->_OnEnter();
}

void SceneManager::OnUpdate(TimeStamp& in_ts)
{
    if (m_currentScene)
    {
        m_currentScene->_OnUpdate(in_ts);
    }
}

void SceneManager::OnRender(TimeStamp& in_ts)
{
    if (m_currentScene)
    {
        m_currentScene->_OnRender(in_ts);
    }
}

void SceneManager::OnRenderGUI(TimeStamp& in_ts)
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
        m_currentScene->_OnEvent(in_event);
    }
}

}   // namespace crab