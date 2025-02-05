#include "CrabPch.h"

#include "Scene.h"

#include "Components.h"
#include "Entity.h"

namespace crab
{

entt::registry& Scene::GetRegistry()
{
    return m_registry;
}

crab::Entity Scene::CreateEntity(uint32 in_id)
{
    entt::entity e = m_registry.create(entt::entity(in_id));
    Entity       entity { this, e };
    entity.AddComponent<Transform>(Transform {});

    return entity;
}

crab::Entity Scene::CreateEntity()
{
    entt::entity e = m_registry.create();
    Entity       entity { this, e };
    entity.AddComponent<Transform>(Transform {});

    return entity;
}

void Scene::_Init()
{
    if (!m_isInit)
    {
        // Init something

        Init();
        m_isInit = true;
    }
}

void Scene::_OnEnter()
{
    // - Init (if not init)
    _Init();

    OnEnter();
}

void Scene::_OnExit()
{
    OnExit();

    // Shutdown something
}

void Scene::_OnUpdate(TimeStamp& in_ts)
{
    // update derived scene
    OnUpdate(in_ts);
}

void Scene::_OnRender(TimeStamp& in_ts)
{
    // render derived scene
    OnRender(in_ts);
}

void Scene::_OnEvent(CrabEvent& in_event)
{
    // pass event to derived scene
    OnEvent(in_event);
}

}   // namespace crab
